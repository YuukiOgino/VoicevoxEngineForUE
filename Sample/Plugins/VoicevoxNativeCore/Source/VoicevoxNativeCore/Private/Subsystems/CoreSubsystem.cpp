// Copyright Yuuki Ogino. All Rights Reserved.

#include "Subsystems/CoreSubsystem.h"
#include "JsonObjectConverter.h"
#include <voicevox_core.h>
#include "Interfaces/IPluginManager.h"

//--------------------------------
// override
//--------------------------------
	
/**
 * @brief Initialize
 */
void UCoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FString BaseDir = IPluginManager::Get().FindPlugin("VoicevoxNativeCore")->GetBaseDir();
	FString DllName;
#if PLATFORM_WINDOWS
	DllName = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/VoicevoxCore/Win64/voicevox_core.dll"));
#elif PLATFORM_MAC
	DllName = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/VoicevoxCore/Mac/libvoicevox_core.dylib"));
#endif 
	
	// DLLを読み込み、ポインタを取得
	if (CoreLibraryHandle = FPlatformProcess::GetDllHandle(*DllName); CoreLibraryHandle == nullptr)
	{
		const FString Message = TEXT("VOICEVOX voicevox_core LoadError!!");
		ShowVoicevoxErrorMessage(Message);
	}
}

/**
 * @brief Deinitialize
 */
void UCoreSubsystem::Deinitialize()
{
	Super::Deinitialize();

	FPlatformProcess::FreeDllHandle(CoreLibraryHandle);
	CoreLibraryHandle = nullptr;
}

//--------------------------------
// VOICEVOX CORE Initialize関連
//--------------------------------

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
	
	const FString FuncName = "voicevox_initialize"; 
	typedef const VoicevoxResultCode(*DLL_Function)(VoicevoxInitializeOptions Option);
	
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 

		if (!FuncPtr)
		{
			const FString Message = TEXT("VOICEVOX voicevox_initialize Function Error");
			ShowVoicevoxErrorMessage(Message);
			return false;
		}

		const FString JtalkPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries"), PlatformFolderName, OPEN_JTALK_DIC_NAME));

		VoicevoxInitializeOptions Option;
		Option.acceleration_mode = bUseGPU ? VOICEVOX_ACCELERATION_MODE_GPU : VOICEVOX_ACCELERATION_MODE_CPU;
		Option.cpu_num_threads = CPUNumThreads;
		Option.load_all_models = bLoadAllModels;
		Option.open_jtalk_dict_dir = TCHAR_TO_UTF8(*JtalkPath);

		if (const VoicevoxResultCode Result = FuncPtr(Option); Result != VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(TEXT("Initialize"), Result);
			return false;
		}

		bIsInit = true;
		return true;
	}

	const FString Message = TEXT("VOICEVOX voicevox_core  LoadError!!");
	ShowVoicevoxErrorMessage(Message);
	return false;

}

//--------------------------------
// VOICEVOX CORE Finalize関連
//--------------------------------

/**
 * @brief 終了処理を行う。以降VOICEVOXのAPIを利用するためには再度Initializeメソッドを行う必要がある。
 */
void UCoreSubsystem::Finalize()
{
	if (CoreLibraryHandle != nullptr)
	{
		const FString FuncName = "voicevox_finalize"; 
		typedef const void(*DLL_Function)();

#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = TEXT("VOICEVOX voicevox_finalize Function Error");
			ShowVoicevoxErrorMessage(Message);
			return;
		}
	
		FuncPtr();
		bIsInit = false;
		return;
	}

	const FString Message = TEXT("VOICEVOX voicevox_core  LoadError!!");
	ShowVoicevoxErrorMessage(Message);
}

//--------------------------------
// VOICEVOX CORE Model関連
//--------------------------------

/**
 * @brief モデルをロードする。
 */
bool UCoreSubsystem::LoadModel(const int64 SpeakerId)
{
	if (CoreLibraryHandle != nullptr)
	{
		const FString FuncName = "voicevox_load_model"; 
		const FString CheckFuncName = "voicevox_is_model_loaded"; 
		typedef const VoicevoxResultCode(*DLL_Function)(uint32_t SpeakerId);
		typedef const bool(*DLL_CheckFunction)(uint32_t SpeakerId);

#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		const auto CheckFuncPtr = static_cast<DLL_CheckFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *CheckFuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
		const auto CheckFuncPtr = (DLL_CheckFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *CheckFuncName);
#endif 

		if (!FuncPtr || !CheckFuncPtr)
		{
			const FString Message = TEXT("VOICEVOX voicevox_load_model Function Error");
			ShowVoicevoxErrorMessage(Message);
			return false;
		}
		
		// 重い処理のため、スピーカーモデルがロードされていない場合のみロードを実行する
		if (!CheckFuncPtr(SpeakerId))
		{
			if (const VoicevoxResultCode Result = FuncPtr(SpeakerId); Result != VOICEVOX_RESULT_OK)
			{
				VoicevoxShowErrorResultMessage(TEXT("voicevox_load_model"), Result);
				return false;
			}
			return true;
		}
		return true;
	}
	const FString Message = TEXT("VOICEVOX voicevox_core  LoadError!!");
	ShowVoicevoxErrorMessage(Message);
	return false;
}

/**
 * @brief 使用するCOREにスピーカーモデルが存在するか
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

//--------------------------------
// VOICEVOX CORE AudioQuery関連
//--------------------------------

/**
 * @brief AudioQuery を取得する。
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
			if (CoreLibraryHandle != nullptr)
			{
				const FString FuncName = "voicevox_audio_query"; 
				const FString FreeFuncName = "voicevox_audio_query_json_free"; 
				typedef const VoicevoxResultCode(*DLL_Function)(const char *Text, uint32_t Speaker_ID, VoicevoxAudioQueryOptions Options, char **Output_Audio_Query_JSON);
				typedef const void(*DLL_FreeFunction)(char *Audio_Query_JSON);

#if PLATFORM_WINDOWS
				const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
				const auto FreeFuncPtr = static_cast<DLL_FreeFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName));
#elif PLATFORM_MAC
				const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
				const auto FreeFuncPtr = (DLL_FreeFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName);
#endif 

				if (!FuncPtr || !FreeFuncPtr)
				{
					const FString ErrorMessage = TEXT("VOICEVOX voicevox_audio_query Function Error");
					ShowVoicevoxErrorMessage(ErrorMessage);
					return AudioQuery;
				}

				char* Output = nullptr;
				VoicevoxAudioQueryOptions Options;
				Options.kana = bKana;
				if (const VoicevoxResultCode Result = FuncPtr(TCHAR_TO_UTF8(*Message), SpeakerId, Options, &Output);
					Result != VOICEVOX_RESULT_OK)
				{
					VoicevoxShowErrorResultMessage(TEXT("TTS"), Result);
				}
				else
				{
					FJsonObjectConverter::JsonObjectStringToUStruct(UTF8_TO_TCHAR(Output), &AudioQuery, 0, 0);
					FreeFuncPtr(Output);
				}
			}
			else
			{
				const FString ErrorMessage = TEXT("VOICEVOX voicevox_core  LoadError!!");
				ShowVoicevoxErrorMessage(ErrorMessage);
			}
		}
	}
	return AudioQuery;
}

//--------------------------------
// VOICEVOX CORE TextToSpeech関連
//--------------------------------

/**
 * @brief VOICEVOX COREのtext to speechを実行
 */
TArray<uint8> UCoreSubsystem::RunTextToSpeech(const int64 SpeakerId, const FString& Message, const bool bKana, const bool bEnableInterrogativeUpspeak)
{
	TArray<uint8> PCMData;
	PCMData.Empty();
	
	// スピーカーモデルがロードされていない場合はロードを実行する
	if (LoadModel(SpeakerId))
	{
		if (CoreLibraryHandle != nullptr)
		{
			const FString FuncName = "voicevox_tts"; 
			typedef const VoicevoxResultCode(*DLL_Function)(const char *text, uint32_t speaker_id, VoicevoxTtsOptions options, uintptr_t *output_wav_length, uint8_t **output_wav);

#if PLATFORM_WINDOWS
			const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
			const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 

			if (!FuncPtr)
			{
				const FString ErrorMessage = TEXT("VOICEVOX voicevox_tts Function Error");
				ShowVoicevoxErrorMessage(ErrorMessage);
				return PCMData;
			}

			uint8* OutputWAV = nullptr;
			VoicevoxTtsOptions Options;
			Options.kana = bKana;
			Options.enable_interrogative_upspeak = bEnableInterrogativeUpspeak;
			uintptr_t OutPutSize = 0;
		
			if (const VoicevoxResultCode Result = FuncPtr(TCHAR_TO_UTF8(*Message), SpeakerId, Options, &OutPutSize, &OutputWAV);
				Result != VOICEVOX_RESULT_OK)
			{
				VoicevoxShowErrorResultMessage(TEXT("TTS"), Result);
			}
			else
			{
				PCMData.Init(0, OutPutSize);
				FMemory::Memcpy(PCMData.GetData(), OutputWAV, OutPutSize);
				WavFree(OutputWAV);
			}
		}
		else
		{
			const FString ErrorMessage = TEXT("VOICEVOX voicevox_core  LoadError!!");
			ShowVoicevoxErrorMessage(ErrorMessage);
		}

	}

	return PCMData;
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UCoreSubsystem::RunSynthesis(const char* AudioQueryJson, const int64 SpeakerId, bool bEnableInterrogativeUpspeak)
{
	TArray<uint8> PCMData;
	PCMData.Empty();
	
	// スピーカーモデルがロードされていない場合はロードを実行する
	if (LoadModel(SpeakerId))
	{
		if (CoreLibraryHandle != nullptr)
		{
			const FString FuncName = "voicevox_synthesis"; 
			typedef const VoicevoxResultCode(*DLL_Function)(const char *audio_query_json, uint32_t speaker_id, VoicevoxSynthesisOptions options,
															uintptr_t *output_wav_length, uint8_t **output_wav);
#if PLATFORM_WINDOWS
			const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
			const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
			if (!FuncPtr)
			{
				const FString ErrorMessage = TEXT("VOICEVOX voicevox_synthesis Function Error");
				ShowVoicevoxErrorMessage(ErrorMessage);
				return PCMData;
			}

			uint8* OutputWAV = nullptr;
			VoicevoxSynthesisOptions Options;
			Options.enable_interrogative_upspeak = bEnableInterrogativeUpspeak;
			uintptr_t OutPutSize = 0;
			if (const VoicevoxResultCode Result = FuncPtr(AudioQueryJson, SpeakerId, Options, &OutPutSize, &OutputWAV);
				Result != VOICEVOX_RESULT_OK)
			{
				VoicevoxShowErrorResultMessage(TEXT("TTS"), Result);
			}
			else
			{
				PCMData.Init(0, OutPutSize);
				FMemory::Memcpy(PCMData.GetData(), OutputWAV, OutPutSize);
				WavFree(OutputWAV);
			}
		}
		else
		{
			const FString ErrorMessage = TEXT("VOICEVOX voicevox_core  LoadError!!");
			ShowVoicevoxErrorMessage(ErrorMessage);
		}
	}

	return PCMData;
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UCoreSubsystem::RunSynthesis(const FVoicevoxAudioQuery& AudioQueryJson, const int64 SpeakerId, const bool bEnableInterrogativeUpspeak)
{
	FString OutputJson = "";
	FJsonObjectConverter::UStructToJsonObjectString(AudioQueryJson, OutputJson, 0, 0, 0, nullptr, false);
	
	TArray<uint8> OutputWAV = RunSynthesis(TCHAR_TO_UTF8(*OutputJson), SpeakerId, bEnableInterrogativeUpspeak);
	return OutputWAV;
}

/**
 * @brief voicevox_tts等で生成した音声データを開放する
 */
void UCoreSubsystem::WavFree(uint8* Wav)
{
	const FString FuncName = "voicevox_wav_free"; 
	typedef const void(*DLL_Function)(uint8_t* WAV);
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = TEXT("VOICEVOX voicevox_wav_free Function Error");
			ShowVoicevoxErrorMessage(Message);
			return;
		}
		
		FuncPtr(Wav);
	}
}

//--------------------------------
// VOICEVOX CORE Configuration関連
//--------------------------------

/**
 * @brief VOICEVOX COREの名前取得
 */
FString UCoreSubsystem::GetVoicevoxCoreName()
{
	return VOICEVOX_CORE_NAME;
}

/**
 * @brief 話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> UCoreSubsystem::GetMetaList()
{
	const FString FuncName = "voicevox_get_metas_json"; 
	typedef const char*(*DLL_Function)();
	
	// DLLを読み込み、ポインタを取得
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = TEXT("VOICEVOX voicevox_get_metas_json Function Error");
			ShowVoicevoxErrorMessage(Message);
			return TArray<FVoicevoxMeta>();
		}
		TArray<FVoicevoxMeta> List;
		const char* Metas = FuncPtr();
		FJsonObjectConverter::JsonArrayStringToUStruct(UTF8_TO_TCHAR(Metas), &List, 0, 0);
		return List;
	}
	
	const FString Message = TEXT("VOICEVOX voicevox_core  LoadError!!");
	ShowVoicevoxErrorMessage(Message);
	return TArray<FVoicevoxMeta>();
}

/**
 * @brief サポートデバイス情報を取得する
 */
FVoicevoxSupportedDevices UCoreSubsystem::GetSupportedDevices()
{
	FVoicevoxSupportedDevices Devices = {};
	// 初期化が行われていない場合はJSON変換時にクラッシュするため、Empty状態で返却する
	if (!bIsInit) return Devices;

	const FString FuncName = "voicevox_get_supported_devices_json"; 
	typedef const char*(*DLL_Function)();
	
	// DLLを読み込み、ポインタを取得
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = TEXT("VOICEVOX voicevox_get_supported_devices_json Function Error");
			ShowVoicevoxErrorMessage(Message);
			return Devices;
		}
		
		FJsonObjectConverter::JsonObjectStringToUStruct(UTF8_TO_TCHAR(FuncPtr()), &Devices, 0, 0);
		return Devices;
	}
	
	const FString Message = TEXT("VOICEVOX voicevox_core  LoadError!!");
	ShowVoicevoxErrorMessage(Message);
	return Devices;
}

/**
 * @brief VOICEVOX COREのバージョンを取得する
 */
FString UCoreSubsystem::GetVoicevoxVersion()
{
	const FString FuncName = "voicevox_get_version"; 
	typedef const char*(*DLL_Function)();
	
	// DLLを読み込み、ポインタを取得
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = TEXT("VOICEVOX voicevox_get_version Function Error");
			ShowVoicevoxErrorMessage(Message);
			return FString();
		}
		return UTF8_TO_TCHAR(FuncPtr());
	}

	const FString Message = TEXT("VOICEVOX voicevox_core  LoadError!!");
	ShowVoicevoxErrorMessage(Message);
	return FString();
}

/**
 * @brief ハードウェアアクセラレーションがGPUモードか判定する
 */
bool UCoreSubsystem::IsGpuMode()
{
	const FString FuncName = "voicevox_is_gpu_mode"; 
	typedef bool(*DLL_Function)();
	
	// DLLを読み込み、ポインタを取得
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = TEXT("VOICEVOX voicevox_is_gpu_mode Function Error");
			ShowVoicevoxErrorMessage(Message);
			return false;
		}
		return FuncPtr();
	}
	
	const FString Message = TEXT("VOICEVOX voicevox_core  LoadError!!");
	ShowVoicevoxErrorMessage(Message);
	return false;
}

//--------------------------------
// VOICEVOX CORE PhonemeLength関連
//--------------------------------

/** 
 * @brief 音素列から、音素ごとの長さを求める
 */
TArray<float> UCoreSubsystem::GetPhonemeLength(const int64 Length, TArray<int64> PhonemeList, const int64 SpeakerID)
{
	TArray<float> Output;
	Output.Empty();
	uintptr_t OutPutSize = 0;
	float* OutputPredictDurationData = nullptr;
	
	if (CoreLibraryHandle != nullptr)
	{
		const FString FuncName = "voicevox_predict_duration"; 
		const FString FreeFuncName = "voicevox_predict_duration_data_free"; 
		typedef const VoicevoxResultCode(*DLL_Function)(uintptr_t lengt, int64_t *phoneme_vector, uint32_t speaker_id,
														uintptr_t *output_predict_duration_data_length, float **output_predict_duration_data);
		typedef const void(*DLL_FreeFunction)(float *predict_duration_data);

#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		const auto FreeFuncPtr = static_cast<DLL_FreeFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
		const auto FreeFuncPtr = (DLL_FreeFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName);
#endif 

		if (!FuncPtr || !FreeFuncPtr)
		{
			const FString ErrorMessage = TEXT("VOICEVOX voicevox_predict_duration Function Error");
			ShowVoicevoxErrorMessage(ErrorMessage);
			return Output;
		}
		
		if (const VoicevoxResultCode Result = FuncPtr(Length, PhonemeList.GetData(), SpeakerID, &OutPutSize, &OutputPredictDurationData); Result != VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(TEXT("voicevox_predict_duration"), Result);
		}
		else
		{
			Output.Init(0, OutPutSize);
			FMemory::Memcpy(Output.GetData(), OutputPredictDurationData, OutPutSize);
			FreeFuncPtr(OutputPredictDurationData);
		}
	}
	else
	{
		const FString Message = TEXT("VOICEVOX voicevox_core  LoadError!!");
		ShowVoicevoxErrorMessage(Message);
	}

	return Output;
}

//--------------------------------
// VOICEVOX CORE Mora関連
//--------------------------------

/**
 * @brief モーラごとの音素列とアクセント情報から、モーラごとの音高を求める
 */
TArray<float> UCoreSubsystem::FindPitchEachMora(const int64 Length, TArray<int64> VowelPhonemeList, TArray<int64> ConsonantPhonemeList,
                                                   TArray<int64> StartAccentList, TArray<int64> EndAccentList,
                                                   TArray<int64> StartAccentPhraseList, TArray<int64> EndAccentPhraseList,
                                                   const int64 SpeakerID)
{

	
	TArray<float> Output;
	uintptr_t OutPutSize = 0;
	float* OutputPredictIntonationData = nullptr;
	if (CoreLibraryHandle != nullptr)
	{
		const FString FuncName = "voicevox_predict_intonation"; 
		const FString FreeFuncName = "voicevox_predict_intonation_data_free"; 
		typedef const VoicevoxResultCode(*DLL_Function)(uintptr_t length, int64_t *vowel_phoneme_vector, int64_t *consonant_phoneme_vector,
													   int64_t *start_accent_vector, int64_t *end_accent_vector,
													   int64_t *start_accent_phrase_vector, int64_t *end_accent_phrase_vector,
													   uint32_t speaker_id, uintptr_t *output_predict_intonation_data_length, float **output_predict_intonation_data);
		typedef const void(*DLL_FreeFunction)(float *predict_duration_data);
		
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		const auto FreeFuncPtr = static_cast<DLL_FreeFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
		const auto FreeFuncPtr = (DLL_FreeFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName);
#endif 
		if (!FuncPtr || !FreeFuncPtr)
		{
			const FString ErrorMessage = TEXT("VOICEVOX voicevox_predict_intonation Function Error");
			ShowVoicevoxErrorMessage(ErrorMessage);
			return Output;
		}

		if (const VoicevoxResultCode Result = FuncPtr(Length, VowelPhonemeList.GetData(), ConsonantPhonemeList.GetData(),
											StartAccentList.GetData(), EndAccentList.GetData(), StartAccentPhraseList.GetData(),
											EndAccentPhraseList.GetData(), SpeakerID, &OutPutSize, &OutputPredictIntonationData); Result != VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(TEXT("voicevox_predict_intonation"), Result);
		}
		else
		{
			Output.Init(0, OutPutSize);
			FMemory::Memcpy(Output.GetData(), OutputPredictIntonationData, OutPutSize);
			FreeFuncPtr(OutputPredictIntonationData);
		}
	}
	else
	{
		const FString Message = TEXT("VOICEVOX voicevox_core  LoadError!!");
		ShowVoicevoxErrorMessage(Message);
	}
	
	return Output;
}

//--------------------------------
// VOICEVOX CORE DecodeForward関連
//--------------------------------

/**
 * @brief フレームごとの音素と音高から、波形を求める
 */
TArray<float> UCoreSubsystem::DecodeForward(const int64 Length, const int64 PhonemeSize, TArray<float> F0, TArray<float> Phoneme, const int64 SpeakerID)
{
	TArray<float> Output;
	uintptr_t OutPutSize = 0;
	float* OutputDecodeData = nullptr;
	if (CoreLibraryHandle != nullptr)
	{
		const FString FuncName = "voicevox_decode"; 
		const FString FreeFuncName = "voicevox_decode_data_free"; 
		typedef const VoicevoxResultCode(*DLL_Function)(uintptr_t length, uintptr_t phoneme_size, float *f0, float *phoneme_vector,
														uint32_t speaker_id, uintptr_t *output_decode_data_length, float **output_decode_data);
		typedef const void(*DLL_FreeFunction)(float* vdecode_data);

#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		const auto FreeFuncPtr = static_cast<DLL_FreeFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
		const auto FreeFuncPtr = (DLL_FreeFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName);
#endif 
		if (!FuncPtr || !FreeFuncPtr)
		{
			const FString ErrorMessage = TEXT("VOICEVOX voicevox_decode Function Error");
			ShowVoicevoxErrorMessage(ErrorMessage);
			return Output;
		}

		if (const VoicevoxResultCode Result = FuncPtr(Length, PhonemeSize, F0.GetData(), Phoneme.GetData(), SpeakerID, &OutPutSize, &OutputDecodeData); Result != VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(TEXT("voicevox_decode"), Result);
		}
		else
		{
			Output.Init(0, OutPutSize);
			FMemory::Memcpy(Output.GetData(), OutputDecodeData, OutPutSize);
			FreeFuncPtr(OutputDecodeData);
		}
	}
	else
	{
		const FString Message = TEXT("VOICEVOX voicevox_core  LoadError!!");
		ShowVoicevoxErrorMessage(Message);
	}
	
	return Output;
}

//--------------------------------
// VOICEVOX CORE Error関連
//--------------------------------

/**
 * エラー結果をメッセージに変換して表示
 */
void UCoreSubsystem::VoicevoxShowErrorResultMessage(const FString& ApiName, int ResultCode)
{
	const FString ErrorMessageFuncName = "voicevox_error_result_to_message";
	typedef const char*(*DLL_ErrorFunction)(VoicevoxResultCode Result);
#if PLATFORM_WINDOWS
	const auto ErrorFuncPtr = static_cast<DLL_ErrorFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName));
#elif PLATFORM_MAC
	const auto ErrorFuncPtr = (DLL_ErrorFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName);
#endif

	if (!ErrorFuncPtr)
	{
		const FString ErrorMessage = TEXT("VOICEVOX voicevox_error_result_to_message Function Error");
		ShowVoicevoxErrorMessage(ErrorMessage);
	}
	else
	{
#if PLATFORM_WINDOWS
		const FString LastMessage = UTF8_TO_TCHAR(ErrorFuncPtr(static_cast<VoicevoxResultCode>(ResultCode)));
#elif PLATFORM_MAC
		const FString LastMessage = UTF8_TO_TCHAR(ErrorFuncPtr((VoicevoxResultCode)ResultCode));
#endif
		const FString Message = FString::Printf(TEXT("VOICEVOX %s Error:%s"), *ApiName, *LastMessage);
		ShowVoicevoxErrorMessage(Message);
	}
}
