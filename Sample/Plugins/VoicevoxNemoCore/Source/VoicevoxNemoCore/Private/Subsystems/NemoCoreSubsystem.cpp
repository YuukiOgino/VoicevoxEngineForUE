// Copyright Yuuki Ogino. All Rights Reserved.

#include "Subsystems/NemoCoreSubsystem.h"
#include "JsonObjectConverter.h"
#include <voicevox_core.h>
#include "Interfaces/IPluginManager.h"

//--------------------------------
// override
//--------------------------------
	
/**
 * @brief Initialize
 */
void UNemoCoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FString BaseDir = IPluginManager::Get().FindPlugin("VoicevoxNemoCore")->GetBaseDir();
	FString DllName;
#if PLATFORM_WINDOWS
	DllName = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/VoicevoxCoreNemo/Win64/voicevox_core.dll"));
#elif PLATFORM_MAC
	DllName = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/VoicevoxCoreNemo/Mac/libvoicevox_core.dylib"));
#endif 
	
	// DLLを読み込み、ポインタを取得
	if (CoreLibraryHandle = FPlatformProcess::GetDllHandle(*DllName); CoreLibraryHandle == nullptr)
	{
		const FString Message = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
		ShowVoicevoxErrorMessage(Message);
	}
}

/**
 * @brief Deinitialize
 */
void UNemoCoreSubsystem::Deinitialize()
{
	Super::Deinitialize();

	FPlatformProcess::FreeDllHandle(CoreLibraryHandle);
	CoreLibraryHandle = nullptr;
}

/**
 * @brief 音声合成するための初期化を行う。VOICEVOXのAPIを正しく実行するには先に初期化が必要
 */
bool UNemoCoreSubsystem::CoreInitialize(const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
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
	const FString ErrorMessageFuncName = "voicevox_error_result_to_message";
	typedef const VoicevoxResultCode(*DLL_Function)(VoicevoxInitializeOptions Option);
	typedef const char*(*DLL_ErrorFunction)(VoicevoxResultCode Result);
	
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		const auto ErrorFuncPtr = static_cast<DLL_ErrorFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName));
#else
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
		const auto ErrorFuncPtr = (DLL_ErrorFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName);
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
			if (!ErrorFuncPtr)
			{
				const FString Message = TEXT("VOICEVOX voicevox_error_result_to_message Function Error");
				ShowVoicevoxErrorMessage(Message);
				return false;
			}
			const FString LastMessage = UTF8_TO_TCHAR(ErrorFuncPtr(Result));
			const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX Initialize Error:%s"), *LastMessage);
			ShowVoicevoxErrorMessage(ErrorMessage);
			return false;
		}

		bIsInit = true;
		return true;
	}

	const FString Message = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
	ShowVoicevoxErrorMessage(Message);
	return false;

}

/**
 * @brief 終了処理を行う。以降VOICEVOXのAPIを利用するためには再度Initializeメソッドを行う必要がある。
 */
void UNemoCoreSubsystem::Finalize()
{
	if (CoreLibraryHandle != nullptr)
	{
		const FString FuncName = "voicevox_finalize"; 
		typedef const void(*DLL_Function)();

		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
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

	const FString Message = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
	ShowVoicevoxErrorMessage(Message);
}

/**
 * @brief モデルをロードする。
 */
bool UNemoCoreSubsystem::LoadModel(const int64 SpeakerId)
{
	if (CoreLibraryHandle != nullptr)
	{
		const FString FuncName = "voicevox_load_model"; 
		const FString CheckFuncName = "voicevox_is_model_loaded"; 
		const FString ErrorMessageFuncName = "voicevox_error_result_to_message";
		typedef const VoicevoxResultCode(*DLL_Function)(uint32_t SpeakerId);
		typedef const bool(*DLL_CheckFunction)(uint32_t SpeakerId);
		typedef const char*(*DLL_ErrorFunction)(VoicevoxResultCode Result);

		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		const auto CheckFuncPtr = static_cast<DLL_CheckFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *CheckFuncName));
		const auto ErrorFuncPtr = static_cast<DLL_ErrorFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName));
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
				if (!ErrorFuncPtr)
				{
					const FString Message = TEXT("VOICEVOX voicevox_error_result_to_message Function Error");
					ShowVoicevoxErrorMessage(Message);
					return false;
				}
				const FString LastMessage = UTF8_TO_TCHAR(ErrorFuncPtr(Result));
				const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX voicevox_load_model Error:%s"), *LastMessage);
				ShowVoicevoxErrorMessage(ErrorMessage);
				return false;
			}
			return true;
		}
		return true;
	}
	const FString Message = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
	ShowVoicevoxErrorMessage(Message);
	return false;
}

/**
 * @brief 使用するCOREにスピーカーモデルが存在するか
 */
bool UNemoCoreSubsystem::IsModel(const int64 SpeakerId)
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
 * @brief AudioQuery を取得する。
 * @details
 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
 */
FVoicevoxAudioQuery UNemoCoreSubsystem::GetAudioQuery(int64 SpeakerId, const FString& Message, bool bKana)
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
				const FString ErrorMessageFuncName = "voicevox_error_result_to_message";
				typedef const VoicevoxResultCode(*DLL_Function)(const char *Text, uint32_t Speaker_ID, VoicevoxAudioQueryOptions Options, char **Output_Audio_Query_JSON);
				typedef const void(*DLL_FreeFunction)(char *Audio_Query_JSON);
				typedef const char*(*DLL_ErrorFunction)(VoicevoxResultCode Result);
				
				const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
				const auto FreeFuncPtr = static_cast<DLL_FreeFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName));
				const auto ErrorFuncPtr = static_cast<DLL_ErrorFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName));
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
					if (!ErrorFuncPtr)
					{
						const FString ErrorMessage = TEXT("VOICEVOX voicevox_error_result_to_message Function Error");
						ShowVoicevoxErrorMessage(ErrorMessage);
					}
					else
					{
						const FString ResultMessage = UTF8_TO_TCHAR(ErrorFuncPtr(Result));
						const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX TTS Error:%s"), *ResultMessage);
						ShowVoicevoxErrorMessage(ErrorMessage);
					}
				}
				else
				{
					FJsonObjectConverter::JsonObjectStringToUStruct(UTF8_TO_TCHAR(Output), &AudioQuery, 0, 0);
					FreeFuncPtr(Output);
				}
			}
			else
			{
				const FString ErrorMessage = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
				ShowVoicevoxErrorMessage(ErrorMessage);
			}
		}
	}
	return AudioQuery;
}

/**
 * @brief VOICEVOX COREのtext to speechを実行
 */
TArray<uint8> UNemoCoreSubsystem::RunTextToSpeech(const int64 SpeakerId, const FString& Message, const bool bKana, const bool bEnableInterrogativeUpspeak)
{
	TArray<uint8> PCMData;
	PCMData.Empty();
	
	// スピーカーモデルがロードされていない場合はロードを実行する
	if (LoadModel(SpeakerId))
	{
		if (CoreLibraryHandle != nullptr)
		{
			const FString FuncName = "voicevox_tts"; 
			const FString ErrorMessageFuncName = "voicevox_error_result_to_message";
			typedef const VoicevoxResultCode(*DLL_Function)(const char *text,
								uint32_t speaker_id,
								VoicevoxTtsOptions options,
								uintptr_t *output_wav_length,
								uint8_t **output_wav);
			typedef const char*(*DLL_ErrorFunction)(VoicevoxResultCode Result);

			const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
			const auto ErrorFuncPtr = static_cast<DLL_ErrorFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName));
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
				if (!ErrorFuncPtr)
				{
					const FString ErrorMessage = TEXT("VOICEVOX voicevox_error_result_to_message Function Error");
					ShowVoicevoxErrorMessage(ErrorMessage);
				}
				else
				{
					const FString ResultMessage = UTF8_TO_TCHAR(ErrorFuncPtr(Result));
					const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX TTS Error:%s"), *ResultMessage);
					ShowVoicevoxErrorMessage(ErrorMessage);
				}
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
			const FString ErrorMessage = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
			ShowVoicevoxErrorMessage(ErrorMessage);
		}

	}

	return PCMData;
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UNemoCoreSubsystem::RunSynthesis(const char* AudioQueryJson, const int64 SpeakerId, bool bEnableInterrogativeUpspeak)
{
	TArray<uint8> PCMData;
	PCMData.Empty();
	
	// スピーカーモデルがロードされていない場合はロードを実行する
	if (LoadModel(SpeakerId))
	{
		if (CoreLibraryHandle != nullptr)
		{
			const FString FuncName = "voicevox_synthesis"; 
			const FString ErrorMessageFuncName = "voicevox_error_result_to_message";
			typedef const VoicevoxResultCode(*DLL_Function)(const char *audio_query_json,
									  uint32_t speaker_id,
									  VoicevoxSynthesisOptions options,
									  uintptr_t *output_wav_length,
									  uint8_t **output_wav);
			typedef const char*(*DLL_ErrorFunction)(VoicevoxResultCode Result);

			const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
			const auto ErrorFuncPtr = static_cast<DLL_ErrorFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName));
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
				if (!ErrorFuncPtr)
				{
					const FString ErrorMessage = TEXT("VOICEVOX voicevox_error_result_to_message Function Error");
					ShowVoicevoxErrorMessage(ErrorMessage);
				}
				else
				{
					const FString ResultMessage = UTF8_TO_TCHAR(ErrorFuncPtr(Result));
					const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX TTS Error:%s"), *ResultMessage);
					ShowVoicevoxErrorMessage(ErrorMessage);
				}
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
			const FString ErrorMessage = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
			ShowVoicevoxErrorMessage(ErrorMessage);
		}
	}

	return PCMData;
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UNemoCoreSubsystem::RunSynthesis(const FVoicevoxAudioQuery& AudioQueryJson, const int64 SpeakerId, const bool bEnableInterrogativeUpspeak)
{
	FString OutputJson = "";
	FJsonObjectConverter::UStructToJsonObjectString(AudioQueryJson, OutputJson, 0, 0, 0, nullptr, false);
	
	TArray<uint8> OutputWAV = RunSynthesis(TCHAR_TO_UTF8(*OutputJson), SpeakerId, bEnableInterrogativeUpspeak);
	return OutputWAV;
}

/**
 * @brief voicevox_tts等で生成した音声データを開放する
 */
void UNemoCoreSubsystem::WavFree(uint8* Wav)
{
	const FString FuncName = "voicevox_wav_free"; 
	typedef const void(*DLL_Function)(uint8_t* WAV);
	if (CoreLibraryHandle != nullptr)
	{
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		if (!FuncPtr)
		{
			const FString Message = TEXT("VOICEVOX voicevox_wav_free Function Error");
			ShowVoicevoxErrorMessage(Message);
			return;
		}
		
		FuncPtr(Wav);
	}
}

/**
 * @brief VOICEVOX COREの名前取得
 */
FString UNemoCoreSubsystem::GetVoicevoxCoreName()
{
	return VOICEVOX_NEMO_CORE_NAME;
}

/**
 * @brief 話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> UNemoCoreSubsystem::GetMetaList()
{
	const FString FuncName = "voicevox_get_metas_json"; 
	typedef const char*(*DLL_Function)();
	
	// DLLを読み込み、ポインタを取得
	if (CoreLibraryHandle != nullptr)
	{
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
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
	
	const FString Message = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
	ShowVoicevoxErrorMessage(Message);
	return TArray<FVoicevoxMeta>();
}

/**
 * @brief サポートデバイス情報を取得する
 */
FVoicevoxSupportedDevices UNemoCoreSubsystem::GetSupportedDevices()
{
	FVoicevoxSupportedDevices Devices = {};
	// 初期化が行われていない場合はJSON変換時にクラッシュするため、Empty状態で返却する
	if (!bIsInit) return Devices;

	const FString FuncName = "voicevox_get_supported_devices_json"; 
	typedef const char*(*DLL_Function)();
	
	// DLLを読み込み、ポインタを取得
	if (CoreLibraryHandle != nullptr)
	{
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		if (!FuncPtr)
		{
			const FString Message = TEXT("VOICEVOX voicevox_get_supported_devices_json Function Error");
			ShowVoicevoxErrorMessage(Message);
			return Devices;
		}
		
		FJsonObjectConverter::JsonObjectStringToUStruct(UTF8_TO_TCHAR(FuncPtr()), &Devices, 0, 0);
		return Devices;
	}
	
	const FString Message = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
	ShowVoicevoxErrorMessage(Message);
	return Devices;
}

/**
 * @brief VOICEVOX COREのバージョンを取得する
 */
FString UNemoCoreSubsystem::GetVoicevoxVersion()
{
	const FString FuncName = "voicevox_get_version"; 
	typedef const char*(*DLL_Function)();
	
	// DLLを読み込み、ポインタを取得
	if (CoreLibraryHandle != nullptr)
	{
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		if (!FuncPtr)
		{
			const FString Message = TEXT("VOICEVOX voicevox_get_version Function Error");
			ShowVoicevoxErrorMessage(Message);
			return FString();
		}
		return UTF8_TO_TCHAR(FuncPtr());
	}

	const FString Message = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
	ShowVoicevoxErrorMessage(Message);
	return FString();
}

/**
 * @brief ハードウェアアクセラレーションがGPUモードか判定する
 */
bool UNemoCoreSubsystem::IsGpuMode()
{
	const FString FuncName = "voicevox_is_gpu_mode"; 
	typedef bool(*DLL_Function)();
	
	// DLLを読み込み、ポインタを取得
	if (CoreLibraryHandle != nullptr)
	{
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		if (!FuncPtr)
		{
			const FString Message = TEXT("VOICEVOX voicevox_is_gpu_mode Function Error");
			ShowVoicevoxErrorMessage(Message);
			return false;
		}
		return FuncPtr();
	}
	
	const FString Message = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
	ShowVoicevoxErrorMessage(Message);
	return false;
}

/** 
 * @brief 音素列から、音素ごとの長さを求める
 */
TArray<float> UNemoCoreSubsystem::GetPhonemeLength(const int64 Length, TArray<int64> PhonemeList, const int64 SpeakerID)
{
	TArray<float> Output;
	Output.Empty();
	uintptr_t OutPutSize = 0;
	float* OutputPredictDurationData = nullptr;
	
	if (CoreLibraryHandle != nullptr)
	{
		const FString FuncName = "voicevox_predict_duration"; 
		const FString FreeFuncName = "voicevox_predict_duration_data_free"; 
		const FString ErrorMessageFuncName = "voicevox_error_result_to_message";
		typedef const VoicevoxResultCode(*DLL_Function)(uintptr_t length,
											 int64_t *phoneme_vector,
											 uint32_t speaker_id,
											 uintptr_t *output_predict_duration_data_length,
											 float **output_predict_duration_data);
		typedef const void(*DLL_FreeFunction)(float *predict_duration_data);
		typedef const char*(*DLL_ErrorFunction)(VoicevoxResultCode Result);

		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		const auto FreeFuncPtr = static_cast<DLL_FreeFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName));
		const auto ErrorFuncPtr = static_cast<DLL_ErrorFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName));
		if (!FuncPtr || !FreeFuncPtr)
		{
			const FString ErrorMessage = TEXT("VOICEVOX voicevox_predict_duration Function Error");
			ShowVoicevoxErrorMessage(ErrorMessage);
			return Output;
		}
		
		if (const VoicevoxResultCode Result = FuncPtr(Length, PhonemeList.GetData(), SpeakerID, &OutPutSize, &OutputPredictDurationData); Result != VOICEVOX_RESULT_OK)
		{
			if (!ErrorFuncPtr)
			{
				const FString ErrorMessage = TEXT("VOICEVOX voicevox_error_result_to_message Function Error");
				ShowVoicevoxErrorMessage(ErrorMessage);
			}
			else
			{
				const FString LastMessage = UTF8_TO_TCHAR(ErrorFuncPtr(Result));
				const FString Message = FString::Printf(TEXT("VOICEVOX voicevox_predict_duration Error:%s"), *LastMessage);
				ShowVoicevoxErrorMessage(Message);
			}
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
		const FString Message = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
		ShowVoicevoxErrorMessage(Message);
	}

	return Output;
}

/**
 * @brief モーラごとの音素列とアクセント情報から、モーラごとの音高を求める
 */
TArray<float> UNemoCoreSubsystem::FindPitchEachMora(const int64 Length, TArray<int64> VowelPhonemeList, TArray<int64> ConsonantPhonemeList,
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
		const FString ErrorMessageFuncName = "voicevox_error_result_to_message";
		typedef const VoicevoxResultCode(*DLL_Function)(uintptr_t length,
											   int64_t *vowel_phoneme_vector,
											   int64_t *consonant_phoneme_vector,
											   int64_t *start_accent_vector,
											   int64_t *end_accent_vector,
											   int64_t *start_accent_phrase_vector,
											   int64_t *end_accent_phrase_vector,
											   uint32_t speaker_id,
											   uintptr_t *output_predict_intonation_data_length,
											   float **output_predict_intonation_data);
		typedef const void(*DLL_FreeFunction)(float *predict_duration_data);
		typedef const char*(*DLL_ErrorFunction)(VoicevoxResultCode Result);

		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		const auto FreeFuncPtr = static_cast<DLL_FreeFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName));
		const auto ErrorFuncPtr = static_cast<DLL_ErrorFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName));
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
			if (!ErrorFuncPtr)
			{
				const FString ErrorMessage = TEXT("VOICEVOX voicevox_error_result_to_message Function Error");
				ShowVoicevoxErrorMessage(ErrorMessage);
			}
			else
			{
				const FString LastMessage = UTF8_TO_TCHAR(ErrorFuncPtr(Result));
				const FString Message = FString::Printf(TEXT("VOICEVOX voicevox_predict_intonation Error:%s"), *LastMessage);
				ShowVoicevoxErrorMessage(Message);
			}
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
		const FString Message = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
		ShowVoicevoxErrorMessage(Message);
	}
	
	return Output;
}

/**
 * @brief フレームごとの音素と音高から、波形を求める
 */
TArray<float> UNemoCoreSubsystem::DecodeForward(const int64 Length, const int64 PhonemeSize, TArray<float> F0, TArray<float> Phoneme, const int64 SpeakerID)
{
	TArray<float> Output;
	uintptr_t OutPutSize = 0;
	float* OutputDecodeData = nullptr;
	if (CoreLibraryHandle != nullptr)
	{
		const FString FuncName = "voicevox_decode"; 
		const FString FreeFuncName = "voicevox_decode_data_free"; 
		const FString ErrorMessageFuncName = "voicevox_error_result_to_message";
		typedef const VoicevoxResultCode(*DLL_Function)(uintptr_t length,
								   uintptr_t phoneme_size,
								   float *f0,
								   float *phoneme_vector,
								   uint32_t speaker_id,
								   uintptr_t *output_decode_data_length,
								   float **output_decode_data);
		typedef const void(*DLL_FreeFunction)(float* vdecode_data);
		typedef const char*(*DLL_ErrorFunction)(VoicevoxResultCode Result);

		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		const auto FreeFuncPtr = static_cast<DLL_FreeFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName));
		const auto ErrorFuncPtr = static_cast<DLL_ErrorFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName));
		if (!FuncPtr || !FreeFuncPtr)
		{
			const FString ErrorMessage = TEXT("VOICEVOX voicevox_decode Function Error");
			ShowVoicevoxErrorMessage(ErrorMessage);
			return Output;
		}

		if (const VoicevoxResultCode Result = FuncPtr(Length, PhonemeSize, F0.GetData(), Phoneme.GetData(), SpeakerID, &OutPutSize, &OutputDecodeData); Result != VOICEVOX_RESULT_OK)
		{
			if (!ErrorFuncPtr)
			{
				const FString ErrorMessage = TEXT("VOICEVOX voicevox_error_result_to_message Function Error");
				ShowVoicevoxErrorMessage(ErrorMessage);
			}
			else
			{
				const FString LastMessage = UTF8_TO_TCHAR(ErrorFuncPtr(Result));
				const FString Message = FString::Printf(TEXT("VOICEVOX voicevox_decode Error:%s"), *LastMessage);
				ShowVoicevoxErrorMessage(Message);
			}
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
		const FString Message = TEXT("VOICEVOX voicevox_core nemo LoadError!!");
		ShowVoicevoxErrorMessage(Message);
	}
	
	return Output;
}