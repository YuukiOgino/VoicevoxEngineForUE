// Copyright Yuuki Ogino. All Rights Reserved.

#include "Subsystems/CoreSubsystem.h"
#include "JsonObjectConverter.h"
#include "voicevox_core.h"

UCoreSubsystem::UCoreSubsystem()
{
}

/**
 * @brief 音声合成するための初期化を行う。VOICEVOXのAPIを正しく実行するには先に初期化が必要
 */
bool UCoreSubsystem::CoreInitialize(const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
{
#if PLATFORM_WINDOWS
	const FString PlatformFolderName = TEXT("Win64");
#elif PLATFORM_MAC
	const FString PlatformFolderName = TEXT("Mac");
#else
	const FString PlatformFolderName = "";
#endif

	if (PlatformFolderName.IsEmpty())
	{
		const FString ErrorMessage = TEXT("VOICEVOX Initialize Error:Not covered Platform");
		ShowVoicevoxErrorMessage(ErrorMessage);
		return false;
	}

	const FString JtalkPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries"), PlatformFolderName, OPEN_JTALK_DIC_NAME));

	VoicevoxInitializeOptions Option;
	Option.acceleration_mode = bUseGPU ? VOICEVOX_ACCELERATION_MODE_GPU : VOICEVOX_ACCELERATION_MODE_CPU;
	Option.cpu_num_threads = CPUNumThreads;
	Option.load_all_models = bLoadAllModels;
	Option.open_jtalk_dict_dir = TCHAR_TO_UTF8(*JtalkPath);

	if (const VoicevoxResultCode Result = voicevox_initialize(Option); Result != VOICEVOX_RESULT_OK)
	{
		const FString LastMessage = UTF8_TO_TCHAR(voicevox_error_result_to_message(Result));
		const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX Initialize Error:%s"), *LastMessage);
		ShowVoicevoxErrorMessage(ErrorMessage);
		return false;
	}

	bIsInit = true;
	return true;
}

/**
 * @brief 終了処理を行う。以降VOICEVOXのAPIを利用するためには再度Initializeメソッドを行う必要がある。
 */
void UCoreSubsystem::Finalize()
{
	voicevox_finalize();
	bIsInit = false;
}

/**
 * @brief モデルをロードする。
 */
bool UCoreSubsystem::LoadModel(const int64 SpeakerId)
{
	// 重い処理のため、スピーカーモデルがロードされていない場合のみロードを実行する
	if (!voicevox_is_model_loaded(SpeakerId))
	{
		if (const VoicevoxResultCode Result = voicevox_load_model(SpeakerId); Result != VOICEVOX_RESULT_OK)
		{
			const FString LastMessage = UTF8_TO_TCHAR(voicevox_error_result_to_message(Result));
			const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX voicevox_load_model Error:%s"), *LastMessage);
			ShowVoicevoxErrorMessage(ErrorMessage);
			return false;
		}
		return true;
	}
	
	return true;
}

/**
 * @fn
 * VOICEVOX COREに該当のスピーカーモデルが存在するか
 * @brief 使用するCOREにスピーカーモデルが存在するか
 * @param SpeakerId 話者番号
 * @return 存在したらtrue、無い場合はfalse
 */
bool UCoreSubsystem::IsModel(const int64 SpeakerId)
{
	for (TArray<FVoicevoxMeta> List = GetMetaList(); auto [Name, Styles, Speaker_uuid, Version] : List)
	{
		for (auto Style : Styles)
		{
			if (SpeakerId == Style.Id)
			{
				return true;
			}
		}
	}
	
	return false;
}

/**
 * @fn
 * VOICEVOX COREのvoicevox_audio_queryを取得
 * @brief AudioQuery を取得する。
 * @param[in] SpeakerId 話者番号
 * @param[in] Message 音声データに変換するtextデータ
 * @param[in] bKana aquestalk形式のkanaとしてテキストを解釈する
 * @return AudioQueryをjsonでフォーマット後、構造体へ変換したもの。
 * @details
 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
 */
FVoicevoxAudioQuery UCoreSubsystem::GetAudioQuery(int64 SpeakerId, const FString& Message, bool bKana)
{
	FVoicevoxAudioQuery AudioQuery{};
	// 初期化が行われていない場合はJSON変換時にクラッシュするため、Empty状態で返却する
	if (bIsInit)
	{
		// スピーカーモデルがロードされていない場合はロードを実行する
		if (LoadModel(SpeakerId))
		{
			char* Output = nullptr;
			VoicevoxAudioQueryOptions Options;
			Options.kana = bKana;
			if (const VoicevoxResultCode Result = voicevox_audio_query(TCHAR_TO_UTF8(*Message), SpeakerId, Options, &Output);
				Result != VOICEVOX_RESULT_OK)
			{
				const FString ResultMessage = UTF8_TO_TCHAR(voicevox_error_result_to_message(Result));
				const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX TTS Error:%s"), *ResultMessage);
				ShowVoicevoxErrorMessage(ErrorMessage);
			}
			else
			{
				FJsonObjectConverter::JsonObjectStringToUStruct(UTF8_TO_TCHAR(Output), &AudioQuery, 0, 0);
				voicevox_audio_query_json_free(Output);
			}
		}
	}
	
	return AudioQuery;
}

/**
 * @brief VOICEVOX COREの名前取得
 */
FString UCoreSubsystem::GetVoicevoxCoreName()
{
	return VOICEVOX_CORE_NAME;
}

/**
 * @fn
 * メタ情報を取得する
 * @brief 話者名や話者IDのリストを取得する
 * @return メタ情報が格納されたjson形式の構造体
 */
TArray<FVoicevoxMeta> UCoreSubsystem::GetMetaList()
{
	TArray<FVoicevoxMeta> List;
	FJsonObjectConverter::JsonArrayStringToUStruct(UTF8_TO_TCHAR(voicevox_get_metas_json()), &List, 0, 0);
	return List;
}

/**
 * @brief サポートデバイス情報を取得する
 * @return サポートデバイス情報の構造体
 */
FVoicevoxSupportedDevices UCoreSubsystem::GetSupportedDevices()
{
	FVoicevoxSupportedDevices Devices = {};
	// 初期化が行われていない場合はJSON変換時にクラッシュするため、Empty状態で返却する
	if (!bIsInit) return Devices;
	FJsonObjectConverter::JsonObjectStringToUStruct(UTF8_TO_TCHAR(voicevox_get_supported_devices_json()), &Devices, 0, 0);
	return Devices;
}

/**
 * @brief VOICEVOX COREのバージョンを取得する
 * @return SemVerでフォーマットされたバージョン
 */
FString UCoreSubsystem::GetVoicevoxVersion()
{
	return UTF8_TO_TCHAR(voicevox_get_version());
}

/**
 * @brief ハードウェアアクセラレーションがGPUモードか判定する
 * @return GPUモードならtrue、そうでないならfalse
 */
bool UCoreSubsystem::IsGpuMode()
{
	return voicevox_is_gpu_mode();
}