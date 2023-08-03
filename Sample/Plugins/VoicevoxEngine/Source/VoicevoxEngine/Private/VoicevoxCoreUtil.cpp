// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOX COREのAPIを呼び出すUtilクラスをまとめたCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxCoreUtil.h"
#include "JsonObjectConverter.h"

#if PLATFORM_MAC
#include "VoicevoxCore/osx/VoicevoxCore/voicevox_core.h"
#else
#include "VoicevoxCore/voicevox_core.h"
#endif

DEFINE_LOG_CATEGORY(LogVoicevoxEngine);

/**
 * @brief VOICEVOX CORE 初期化
 */
bool FVoicevoxCoreUtil::Initialize(const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
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
 * @brief VOICEVOX CORE 終了処理
 */
void FVoicevoxCoreUtil::Finalize()
{
	voicevox_finalize();
	bIsInit = false;
}

/**
 * @brief VOICEVOX COREのバージョンを取得する
 */
FString FVoicevoxCoreUtil::GetVoicevoxVersion()
{
	return UTF8_TO_TCHAR(voicevox_get_version());
}

/**
 * @brief ハードウェアアクセラレーションがGPUモードか判定する
 */
bool FVoicevoxCoreUtil::IsGpuMode()
{
	return voicevox_is_gpu_mode();
}

/**
 * @brief サポートデバイス情報をjsonで取得する
 */
FString FVoicevoxCoreUtil::GetSupportedDevicesJson()
{
	return UTF8_TO_TCHAR(voicevox_get_supported_devices_json());
}

/**
 * @brief サポートデバイス情報を取得する
 */
FVoicevoxSupportedDevices FVoicevoxCoreUtil::GetSupportedDevices()
{
	FVoicevoxSupportedDevices Devices = {};
	// 初期化が行われていない場合はJSON変換時にクラッシュするため、Empty状態で返却する
	if (!bIsInit) return Devices;
	FJsonObjectConverter::JsonObjectStringToUStruct(GetSupportedDevicesJson(), &Devices, 0, 0);
	return Devices;
}

/**
 * @brief VOICEVOX COREのモデルをロード実行
 */
bool FVoicevoxCoreUtil::LoadModel(const int64 SpeakerId)
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
 * @brief VOICEVOX COREのtext to speechを実行
 */
TArray<uint8> FVoicevoxCoreUtil::RunTextToSpeech(const int64 SpeakerId, const FString& Message, const bool bKana, const bool bEnableInterrogativeUpspeak)
{
	TArray<uint8> PCMData;
	PCMData.Empty();
	
	// スピーカーモデルがロードされていない場合はロードを実行する
	if (LoadModel(SpeakerId))
	{
		uint8* OutputWAV = nullptr;
		VoicevoxTtsOptions Options;
		Options.kana = bKana;
		Options.enable_interrogative_upspeak = bEnableInterrogativeUpspeak;
		uintptr_t OutPutSize = 0;
		
		if (const VoicevoxResultCode Result = voicevox_tts(TCHAR_TO_UTF8(*Message), SpeakerId, Options, &OutPutSize, &OutputWAV);
			Result != VOICEVOX_RESULT_OK)
		{
			const FString ResultMessage = UTF8_TO_TCHAR(voicevox_error_result_to_message(Result));
			const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX TTS Error:%s"), *ResultMessage);
			ShowVoicevoxErrorMessage(ErrorMessage);
		}
		else
		{
			PCMData.Init(0, OutPutSize);
			FMemory::Memcpy(PCMData.GetData(), OutputWAV, OutPutSize);
			WavFree(OutputWAV);
		}
	}

	return PCMData;
}

/**
 * @brief AudioQuery を取得する。
 */
char* FVoicevoxCoreUtil::GetAudioQuery(const int64 SpeakerId, const FString& Message, const bool bKana)
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
			return nullptr;	
		}

		return Output;
	}

	return nullptr;
}

/**
 * @brief AudioQuery を取得する。
 */
FVoicevoxAudioQuery FVoicevoxCoreUtil::GetAudioQueryList(const int64 SpeakerId, const FString& Message, const bool bKana)
{
	FVoicevoxAudioQuery AudioQuery{};
	// 初期化が行われていない場合はJSON変換時にクラッシュするため、Empty状態で返却する
	if (bIsInit)
	{
		char* Q = GetAudioQuery(SpeakerId, Message, bKana);
		FJsonObjectConverter::JsonObjectStringToUStruct(UTF8_TO_TCHAR(Q), &AudioQuery, 0, 0);
		AudioQueryFree(Q);
	}
	
	return AudioQuery;
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> FVoicevoxCoreUtil::RunSynthesis(const char* AudioQueryJson, const int64 SpeakerId, bool bEnableInterrogativeUpspeak)
{
	TArray<uint8> PCMData;
	PCMData.Empty();
	
	// スピーカーモデルがロードされていない場合はロードを実行する
	if (LoadModel(SpeakerId))
	{
		uint8* OutputWAV = nullptr;
		VoicevoxSynthesisOptions Options;
		Options.enable_interrogative_upspeak = bEnableInterrogativeUpspeak;
		uintptr_t OutPutSize = 0;
		if (const VoicevoxResultCode Result = voicevox_synthesis(AudioQueryJson, SpeakerId, Options, &OutPutSize, &OutputWAV);
			Result != VOICEVOX_RESULT_OK)
		{
			const FString ResultMessage = UTF8_TO_TCHAR(voicevox_error_result_to_message(Result));
			const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX TTS Error:%s"), *ResultMessage);
			ShowVoicevoxErrorMessage(ErrorMessage);
		}
		else
		{
			PCMData.Init(0, OutPutSize);
			FMemory::Memcpy(PCMData.GetData(), OutputWAV, OutPutSize);
			WavFree(OutputWAV);
		}
	}

	return PCMData;
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> FVoicevoxCoreUtil::RunSynthesis(const FVoicevoxAudioQuery& AudioQueryJson, const int64 SpeakerId, const bool bEnableInterrogativeUpspeak)
{
	FString OutputJson = "";
	FJsonObjectConverter::UStructToJsonObjectString(AudioQueryJson, OutputJson, 0, 0, 0, nullptr, false);
	
	TArray<uint8> OutputWAV = RunSynthesis(TCHAR_TO_UTF8(*OutputJson), SpeakerId, bEnableInterrogativeUpspeak);
	return OutputWAV;
}

/**
 * @brief VOICEVOX COREのvoicevox_ttsで生成した音声データを開放
 */
void FVoicevoxCoreUtil::WavFree(uint8* Wav)
{
	voicevox_wav_free(Wav);
}

/**
 * @brief jsonフォーマットされた AudioQuery データのメモリを解放する
 */
void FVoicevoxCoreUtil::AudioQueryFree(char* QueryJson)
{
	voicevox_audio_query_json_free(QueryJson);
}

/**
 * @brief 話者名や話者IDのリストを取得する
 */
FString FVoicevoxCoreUtil::Metas()
{
	return UTF8_TO_TCHAR(voicevox_get_metas_json());
}

/**
 * @brief 話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> FVoicevoxCoreUtil::GetMetaList()
{
	TArray<FVoicevoxMeta> List;
	// 初期化が行われていない場合はJSON変換時にクラッシュするため、Empty状態で返却する
	if (!bIsInit) return List;
	FJsonObjectConverter::JsonArrayStringToUStruct(Metas(), &List, 0, 0);
	return List;
}

/** 
 * @brief 音素列から、音素ごとの長さを求める
 */
TArray<float> FVoicevoxCoreUtil::GetPhonemeLength(const int64 Length, TArray<int64> PhonemeList, const int64 SpeakerID)
{
	TArray<float> Output;
	uintptr_t OutPutSize = 0;
	float* OutputPredictDurationData = nullptr;
	if (const VoicevoxResultCode Result = voicevox_predict_duration(Length, PhonemeList.GetData(), SpeakerID, &OutPutSize, &OutputPredictDurationData); Result != VOICEVOX_RESULT_OK)
	{
		const FString LastMessage = UTF8_TO_TCHAR(voicevox_error_result_to_message(Result));
		const FString Message = FString::Printf(TEXT("VOICEVOX voicevox_predict_duration Error:%s"), *LastMessage);
		ShowVoicevoxErrorMessage(Message);
	}
	else
	{
		Output.Init(0, OutPutSize);
		FMemory::Memcpy(Output.GetData(), OutputPredictDurationData, OutPutSize);
		voicevox_predict_duration_data_free(OutputPredictDurationData);
	}
	return Output;
}

/**
 * @brief モーラごとの音素列とアクセント情報から、モーラごとの音高を求める
 */
TArray<float> FVoicevoxCoreUtil::FindPitchEachMora(const int64 Length, TArray<int64> VowelPhonemeList, TArray<int64> ConsonantPhonemeList,
                                                   TArray<int64> StartAccentList, TArray<int64> EndAccentList,
                                                   TArray<int64> StartAccentPhraseList, TArray<int64> EndAccentPhraseList,
                                                   const int64 SpeakerID)
{
	TArray<float> Output;
	uintptr_t OutPutSize = 0;
	float* OutputPredictIntonationData = nullptr;
	if (const VoicevoxResultCode Result = voicevox_predict_intonation(Length, VowelPhonemeList.GetData(), ConsonantPhonemeList.GetData(),
	                                            StartAccentList.GetData(), EndAccentList.GetData(), StartAccentPhraseList.GetData(),
	                                            EndAccentPhraseList.GetData(), SpeakerID, &OutPutSize, &OutputPredictIntonationData); Result != VOICEVOX_RESULT_OK)
	{
		const FString LastMessage = UTF8_TO_TCHAR(voicevox_error_result_to_message(Result));
		const FString Message = FString::Printf(TEXT("VOICEVOX voicevox_predict_intonation Error:%s"), *LastMessage);
		ShowVoicevoxErrorMessage(Message);
	}
	else
	{
		Output.Init(0, OutPutSize);
		FMemory::Memcpy(Output.GetData(), OutputPredictIntonationData, OutPutSize);
		voicevox_predict_intonation_data_free(OutputPredictIntonationData);
	}
	
	return Output;
}

/**
 * @brief フレームごとの音素と音高から、波形を求める
 */
TArray<float> FVoicevoxCoreUtil::DecodeForward(const int64 Length, const int64 PhonemeSize, TArray<float> F0, TArray<float> Phoneme, const int64 SpeakerID)
{
	TArray<float> Output;
	uintptr_t OutPutSize = 0;
	float* OutputDecodeData = nullptr; 
	if (const VoicevoxResultCode Result = voicevox_decode(Length, PhonemeSize, F0.GetData(), Phoneme.GetData(), SpeakerID, &OutPutSize, &OutputDecodeData); Result != VOICEVOX_RESULT_OK)
	{
		const FString LastMessage = UTF8_TO_TCHAR(voicevox_error_result_to_message(Result));
		const FString Message = FString::Printf(TEXT("VOICEVOX voicevox_decode Error:%s"), *LastMessage);
		ShowVoicevoxErrorMessage(Message);
	}
	else
	{
		Output.Init(0, OutPutSize);
		FMemory::Memcpy(Output.GetData(), OutputDecodeData, OutPutSize);
		voicevox_decode_data_free(OutputDecodeData);
	}
	
	return Output;
}

/**
 * @brief VOICEVOXから受信したエラーメッセージを表示
 */
void FVoicevoxCoreUtil::ShowVoicevoxErrorMessage(const FString& MessageFormat)
{
	UE_LOG(LogVoicevoxEngine, Error, TEXT("%s"), *MessageFormat);
	const FColor Col = FColor::Red;
	const FVector2D Scl = FVector2D(1.0f, 1.0f);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, Col, *MessageFormat, true, Scl);
}