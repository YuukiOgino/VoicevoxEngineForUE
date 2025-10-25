// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief VOICEVOX COREのネイティブライブラリのAPIを実行する基礎Subsystem CPPファイル
 * @author Yuuki Ogino
 */

#include "Subsystems/VoicevoxNativeCoreSubsystem.h"
#include "JsonObjectConverter.h"

DEFINE_LOG_CATEGORY(LogVoicevoxNativeCore);

//--------------------------------
// VOICEVOX CORE Initialize関連
//--------------------------------

/**
 * @brief 音声合成するための初期化を行う。VOICEVOXのAPIを正しく実行するには先に初期化が必要
 */
bool UVoicevoxNativeCoreSubsystem::ApiInitialize(const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
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
		const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX %s Initialize Error:Not covered Platform"), *GetVoicevoxCoreName());
		ShowVoicevoxErrorMessage(ErrorMessage);
		return false;
	}
	
	const FString FuncName = "voicevox_synthesizer_new"; 
	using DLL_Function = const VoicevoxResultCode(*)(const VoicevoxOnnxruntime*, const OpenJtalkRc*, VoicevoxInitializeOptions, VoicevoxSynthesizer**);

	const FString MakeDefaultFuncName = "voicevox_make_default_load_onnxruntime_options"; 
	using DLL_MakeDefaultLoadOnnxruntimeOptionsFunction = const VoicevoxLoadOnnxruntimeOptions(*)();

	const FString OnnxruntimeLoadOnceFuncName = "voicevox_onnxruntime_load_once"; 
	using DLL_OnnxruntimeLoadOnceFunction = const VoicevoxResultCode(*)(VoicevoxLoadOnnxruntimeOptions, const VoicevoxOnnxruntime**);

	const FString OpenJTalkRcNewFuncName = "voicevox_open_jtalk_rc_new"; 
	using DLL_OpenJTalkRcNewFunction = const VoicevoxResultCode(*)(const char*, OpenJtalkRc**);
	
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto MakeDefaultFuncPtr = static_cast<DLL_MakeDefaultLoadOnnxruntimeOptionsFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *MakeDefaultFuncName));
#elif PLATFORM_MAC
		const auto MakeDefaultFuncPtr = (DLL_MakeDefaultLoadOnnxruntimeOptionsFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *MakeDefaultFuncName);
#endif

		if (!MakeDefaultFuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_make_default_load_onnxruntime_options Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return false;
		}

		const auto LoadOrtOptions = MakeDefaultFuncPtr();

#if PLATFORM_WINDOWS
		const auto LoadOnceFuncPtr = static_cast<DLL_OnnxruntimeLoadOnceFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *OnnxruntimeLoadOnceFuncName));
#elif PLATFORM_MAC
		const auto LoadOnceFuncPtr = (DLL_OnnxruntimeLoadOnceFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *OnnxruntimeLoadOnceFuncName);
#endif

		if (!LoadOnceFuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_onnxruntime_load_once Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return false;
		}
		
		if (const VoicevoxResultCode Result = LoadOnceFuncPtr(LoadOrtOptions, &Onnxruntime); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(TEXT("Initialize"), Result);
			return false;
		}

#if PLATFORM_WINDOWS
		const auto OpenJTalkRcFuncPtr = static_cast<DLL_OpenJTalkRcNewFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *OpenJTalkRcNewFuncName));
#elif PLATFORM_MAC
		const auto OpenJTalkRcFuncPtr = (DLL_OpenJTalkRcNewFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *OpenJTalkRcNewFuncName);
#endif

		if (!OpenJTalkRcFuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_open_jtalk_rc_new Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return false;
		}
		
		const FString JTalkPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries"), PlatformFolderName, GetOpenJtakeDirectoryName()));
		if (const VoicevoxResultCode Result = OpenJTalkRcFuncPtr(TCHAR_TO_UTF8(*JTalkPath), &OpenJTalk); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(TEXT("Initialize"), Result);
			return false;
		}
		
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 

		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_initialize Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return false;
		}
		
		VoicevoxInitializeOptions Option;
		Option.acceleration_mode = bUseGPU ? VoicevoxAccelerationMode::VOICEVOX_ACCELERATION_MODE_GPU : VoicevoxAccelerationMode::VOICEVOX_ACCELERATION_MODE_CPU;
		Option.cpu_num_threads = CPUNumThreads;

		if (const VoicevoxResultCode Result = FuncPtr(Onnxruntime, OpenJTalk, Option, &Synthesizer); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(TEXT("Initialize"), Result);
			OpenJTalkRcDelete(OpenJTalk);
			return false;
		}

		
		bIsInit = true;
		return true;
	}

	const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
	ShowVoicevoxErrorMessage(Message);
	return false;
}

/**
 * @brief デフォルトの初期化オプションを生成する
 */
VoicevoxInitializeOptions UVoicevoxNativeCoreSubsystem::MakeDefaultInitializeOptions()
{
	const FString FuncName = "voicevox_make_default_initialize_options"; 
	using DLL_Function = const VoicevoxInitializeOptions(*)();
	
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_make_default_initialize_options Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return VoicevoxInitializeOptions{};
		}

		return FuncPtr();
	}

	const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
	ShowVoicevoxErrorMessage(Message);
	return VoicevoxInitializeOptions{};
}

/**
 * @brief OpenJtalkRc を<b>破棄</b>(_destruct_)する。
 */
void UVoicevoxNativeCoreSubsystem::OpenJTalkRcDelete(OpenJtalkRc* Rc)
{
	const FString FuncName = "voicevox_open_jtalk_rc_delete"; 
	using DLL_Function = const void(*)(OpenJtalkRc*);
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_open_jtalk_rc_delete Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return;
		}
		
		FuncPtr(Rc);
	}
}

/**
 * @brief ONNX Runtimeの動的ライブラリの、バージョン付きのファイル名を取得。
 */
FString UVoicevoxNativeCoreSubsystem::GetOnnxruntimeLibVersionedFilename()
{
	FString Name;
	Name.Empty();
	const FString FuncName = "voicevox_get_onnxruntime_lib_versioned_filename"; 
	using DLL_Function = const char*(*)();
	
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_get_onnxruntime_lib_versioned_filename Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return Name;
		}
		
		Name = UTF8_TO_TCHAR(FuncPtr());
	}
	return Name;
}

/**
 * @brief ONNX Runtimeの動的ライブラリの、バージョン無しのファイル名を取得。
 */
FString UVoicevoxNativeCoreSubsystem::GetOnnxruntimeLibUnversionedFilename()
{
	FString Name;
	Name.Empty();
	const FString FuncName = "voicevox_get_onnxruntime_lib_unversioned_filename"; 
	using DLL_Function = const char*(*)();
	
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_get_onnxruntime_lib_unversioned_filename Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return Name;
		}
		
		Name = UTF8_TO_TCHAR(FuncPtr());
	}
	return Name;
}

/**
 * @brief VoicevoxOnnxruntime のインスタンスが既に作られているならそれを得る。 作られていなければ`NULL`を返す。
 */
const VoicevoxOnnxruntime* UVoicevoxNativeCoreSubsystem::GetOnnxruntime()
{
	const FString FuncName = "voicevox_onnxruntime_get"; 
	using DLL_Function = const VoicevoxOnnxruntime*(*)();
	
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_onnxruntime_get Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return nullptr;
		}
		
		return FuncPtr();
	}
	return nullptr;
}

/**
 * @brief ONNX Runtimeを初期化する。
 */
const VoicevoxOnnxruntime* UVoicevoxNativeCoreSubsystem::OnxruntimeInitOcec()
{
	const FString FuncName = "voicevox_onnxruntime_init_once"; 
	using DLL_Function = const VoicevoxResultCode(*)(const VoicevoxOnnxruntime**);
	const VoicevoxOnnxruntime* Runtime;
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_onnxruntime_init_once Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return nullptr;
		}

		if (const VoicevoxResultCode Result = FuncPtr(&Runtime); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(TEXT("OnxruntimeInitnOcec"), Result);
			return nullptr;
		}
		
		return Runtime;
	}
	return nullptr;
}

/**
 * @brief 日本語のテキストを解析する。
 */
char* UVoicevoxNativeCoreSubsystem::OpenJTalkRcAnalyze(const FString& Text)
{
	const FString FuncName = "voicevox_open_jtalk_rc_analyze"; 
	using DLL_Function = const VoicevoxResultCode(*)(const OpenJtalkRc*, const char*, char**);
	
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_onnxruntime_init_once Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return nullptr;
		}

		char* result;
		if (const VoicevoxResultCode Result = FuncPtr(OpenJTalk, TCHAR_TO_UTF8(*Text), &result); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(TEXT("OnxruntimeInitOcec"), Result);
			return nullptr;
		}
		JsonFree(result);
		return result;
	}
	return nullptr;
}

//--------------------------------
// VOICEVOX CORE Finalize関連
//--------------------------------

/**
 * @brief 終了処理を行う。以降VOICEVOXのAPIを利用するためには再度Initializeメソッドを行う必要がある。
 */
void UVoicevoxNativeCoreSubsystem::Finalize()
{
	if (CoreLibraryHandle != nullptr && OpenJTalk != nullptr)
	{
		OpenJTalkRcDelete(OpenJTalk);
	}
	
	if (CoreLibraryHandle != nullptr && Synthesizer != nullptr)
	{
		const FString FuncName = "voicevox_synthesizer_delete"; 
		using DLL_Function = const void(*)(VoicevoxSynthesizer*);

#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_finalize Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return;
		}
	
		FuncPtr(nullptr);
		bIsInit = false;
		return;
	}

	const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
	ShowVoicevoxErrorMessage(Message);
}

//--------------------------------
// VOICEVOX CORE Model関連
//--------------------------------

/**
 * @brief　VVMファイルのロード実行
 */
bool UVoicevoxNativeCoreSubsystem::LoadVoiceModel(const FString VvmFileName)
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
		const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX %s Initialize Error:Not covered Platform"), *GetVoicevoxCoreName());
		ShowVoicevoxErrorMessage(ErrorMessage);
		return false;
	}
	
	VoicevoxVoiceModelFile* Model = nullptr;
	FString VVMName = VvmFileName + TEXT(".vvm");
	if (const FString VmmPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries"), PlatformFolderName, TEXT("models"), TEXT("vvms"), VVMName));
		!VoiceModelFileOpen(VmmPath, &Model))
	{
		return false;
	}
	
	if (!SynthesizerLoadVoiceModel(*Model))
	{
		return false;
	}

	ModelIdMap.Add(VvmFileName, VoiceModelFileId(*Model));
	VoiceModelFileDelete(*Model);

	return true;
}

/**
 * @brief　全てのVVMファイルを開く。
 */
bool UVoicevoxNativeCoreSubsystem::AllLoadVoiceModel()
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
		const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX %s Initialize Error:Not covered Platform"), *GetVoicevoxCoreName());
		ShowVoicevoxErrorMessage(ErrorMessage);
		return false;
	}

	const FString ModelsDirPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries"), PlatformFolderName, TEXT("models")));
	TArray<FString> FoundFiles;
	IFileManager::Get().FindFilesRecursive(FoundFiles, *ModelsDirPath,TEXT("*.vvm"), true, false, false);

	for (const FString& FileName : FoundFiles)
	{
		VoicevoxVoiceModelFile* Model = nullptr;
		if (!VoiceModelFileOpen(FileName, &Model))
		{
			return false;
		}
		
		if (!SynthesizerLoadVoiceModel(*Model))
		{
			return false;
		}

		FString FileNameOnly = FPaths::GetBaseFilename(FileName);
		ModelIdMap.Add(FileNameOnly, VoiceModelFileId(*Model));
		
		VoiceModelFileDelete(*Model);
	}
	
	return true;
}
/**
 * @brief VVMファイルを開く。
 */
bool UVoicevoxNativeCoreSubsystem::VoiceModelFileOpen(const FString& Path, VoicevoxVoiceModelFile** Model)
{
	
	if (CoreLibraryHandle != nullptr)
	{
		const FString FileOpenFuncName = "voicevox_voice_model_file_open";
		using DLL_FileOpenFunction = const VoicevoxResultCode(*)(const char*, VoicevoxVoiceModelFile**);

#if PLATFORM_WINDOWS
		const auto FileOpenFuncPtr = static_cast<DLL_FileOpenFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileOpenFuncName));
#elif PLATFORM_MAC
		const auto FileOpenFuncPtr = (DLL_FileOpenFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileOpenFuncName);
#endif

		if (!FileOpenFuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_voice_model_file_open Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return false;
		}

		if (const VoicevoxResultCode Result = FileOpenFuncPtr(TCHAR_TO_UTF8(*Path), Model);
			Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(TEXT("voicevox_voice_model_file_open"), Result);
			return false;
		}
		
		return true;
	}

	return false;
}

/**
 * 音声モデルを読み込む。
 */
bool UVoicevoxNativeCoreSubsystem::SynthesizerLoadVoiceModel(const VoicevoxVoiceModelFile& Model)
{
	if (CoreLibraryHandle != nullptr)
	{
		const FString LoadFuncName = "voicevox_synthesizer_load_voice_model";
		using DLL_LoadFunction = const VoicevoxResultCode(*)(const VoicevoxSynthesizer*, const VoicevoxVoiceModelFile*);
#if PLATFORM_WINDOWS
		const auto LoadFuncPtr = static_cast<DLL_LoadFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *LoadFuncName));
#elif PLATFORM_MAC
		const auto LoadFuncPtr = (DLL_LoadFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *LoadFuncName);
#endif

		if (!LoadFuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_synthesizer_load_voice_model Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return false;
		}
		
		if (const VoicevoxResultCode Result = LoadFuncPtr(Synthesizer, &Model); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(TEXT("voicevox_synthesizer_load_voice_model"), Result);
			return false;
		}

		return true;
	}

	return false;
}

/**
 * @brief VoicevoxVoiceModelFile を、所有しているファイルディスクリプタを閉じた上で<b>破棄</b>(_destruct_)する。
 */
void UVoicevoxNativeCoreSubsystem::VoiceModelFileDelete(VoicevoxVoiceModelFile& Model)
{
	if (CoreLibraryHandle != nullptr)
	{
		const FString FileDeleteFuncName = "voicevox_voice_model_file_delete";
		using DLL_FileDeleteFunction = const void(*)(VoicevoxVoiceModelFile*);

#if PLATFORM_WINDOWS
		const auto FileDeleteFuncPtr = static_cast<DLL_FileDeleteFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileDeleteFuncName));
#elif PLATFORM_MAC
		const auto FileDeleteFuncPtr = (DLL_LoadFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileDeleteFuncName);
#endif

		if (!FileDeleteFuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_voice_model_file_delete Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return;
		}
		
		FileDeleteFuncPtr(&Model);
	}
}

/**
 * @brief  VoicevoxVoiceModelFile からIDを取得する。
 */
TArray<uint8_t> UVoicevoxNativeCoreSubsystem::VoiceModelFileId(const VoicevoxVoiceModelFile& Model)
{
	TArray<uint8_t> Output;
	if (CoreLibraryHandle != nullptr)
	{
		const FString FileIdFuncName = "voicevox_voice_model_file_id";
		using DLL_FileIdFunction = const void(*)(const VoicevoxVoiceModelFile*, uint8_t(*)[16]);

#if PLATFORM_WINDOWS
		const auto FileIdFuncPtr = static_cast<DLL_FileIdFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileIdFuncName));
#elif PLATFORM_MAC
		const auto FileIdFuncPtr = (DLL_FileIdFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileIdFuncName);
#endif

		if (!FileIdFuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_voice_model_file_id Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return Output;
		}
		
		uint8_t ModelId[16];
		FileIdFuncPtr(&Model, &ModelId);
		Output.Init(0, 16);
		FMemory::Memcpy(Output.GetData(), ModelId, 16);
	}

	return Output;
}

/**
 * @brief モデルをロードする。
 */
bool UVoicevoxNativeCoreSubsystem::LoadModel(const int64 SpeakerId)
{
	return true;
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
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_load_model Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return false;
		}
		
		// 重い処理のため、スピーカーモデルがロードされていない場合のみロードを実行する
		if (!CheckFuncPtr(SpeakerId))
		{
			if (const VoicevoxResultCode Result = FuncPtr(SpeakerId); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
			{
				VoicevoxShowErrorResultMessage(TEXT("voicevox_load_model"), Result);
				return false;
			}
			return true;
		}
		return true;
	}
	const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
	ShowVoicevoxErrorMessage(Message);
	return false;
}

/**
 * @brief 使用するCOREにスピーカーモデルが存在するか
 */
bool UVoicevoxNativeCoreSubsystem::IsModel(const int64 SpeakerId)
{
	for (auto [Name, Styles, Speaker_uuid, Version] : GetMetaList())
	{
		for (const auto Style : Styles)
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
FVoicevoxAudioQuery UVoicevoxNativeCoreSubsystem::GetAudioQuery(int64 SpeakerId, const FString& Message, bool bKana)
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
				typedef const VoicevoxResultCode(*DLL_Function)(const char *Text, uint32_t Speaker_ID, char **Output_Audio_Query_JSON);
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
					const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX %s voicevox_audio_query Function Error"), *GetVoicevoxCoreName());
					ShowVoicevoxErrorMessage(ErrorMessage);
					return AudioQuery;
				}

				char* Output = nullptr;
				if (const VoicevoxResultCode Result = FuncPtr(TCHAR_TO_UTF8(*Message), SpeakerId, &Output);
					Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
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
				const FString MessageFormat =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
				ShowVoicevoxErrorMessage(MessageFormat);
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
TArray<uint8> UVoicevoxNativeCoreSubsystem::RunTextToSpeech(const int64 SpeakerId, const FString& Message, const bool bKana, const bool bEnableInterrogativeUpspeak)
{
	TArray<uint8> PCMData;
	PCMData.Empty();
	
	// スピーカーモデルがロードされていない場合はロードを実行する
	if (LoadModel(SpeakerId))
	{
		if (CoreLibraryHandle != nullptr)
		{
			const FString FuncName = bKana ? "voicevox_synthesizer_tts_from_kana" : "voicevox_synthesizer_tts"; 
			typedef const VoicevoxResultCode(*DLL_Function)(const VoicevoxSynthesizer *synthesizer,
											const char *text,
											VoicevoxStyleId style_id,
											VoicevoxTtsOptions options,
											uintptr_t *output_wav_length,
											uint8_t **output_wav);

#if PLATFORM_WINDOWS
			const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
			const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 

			if (!FuncPtr)
			{
				const FString ErrorMessage =  FString::Printf(TEXT("VOICEVOX %s voicevox_synthesizer_tts Function Error"), *GetVoicevoxCoreName());
				ShowVoicevoxErrorMessage(ErrorMessage);
				return PCMData;
			}

			uint8* OutputWAV = nullptr;
			VoicevoxTtsOptions Options;
			Options.enable_interrogative_upspeak = bEnableInterrogativeUpspeak;
			uintptr_t OutPutSize = 0;
		
			if (const VoicevoxResultCode Result = FuncPtr(Synthesizer, TCHAR_TO_UTF8(*Message), SpeakerId, Options, &OutPutSize, &OutputWAV);
				Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
			{
				VoicevoxShowErrorResultMessage(TEXT("voicevox_synthesizer_ttsW"), Result);
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
			const FString MessageFormat =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(MessageFormat);
		}

	}

	return PCMData;
}

/**
 * @brief デフォルトのテキスト音声合成オプションを生成する
 */
VoicevoxTtsOptions UVoicevoxNativeCoreSubsystem::MakeDefaultTtsOptions()
{
	const FString FuncName = "voicevox_make_default_tts_options"; 
	typedef const VoicevoxTtsOptions(*DLL_Function)();
	
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_make_default_tts_options Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return VoicevoxTtsOptions{};
		}

		return FuncPtr();
	}

	const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
	ShowVoicevoxErrorMessage(Message);
	return VoicevoxTtsOptions{};
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UVoicevoxNativeCoreSubsystem::RunSynthesis(const char* AudioQueryJson, const int64 SpeakerId, bool bEnableInterrogativeUpspeak)
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
				const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX %s voicevox_synthesis Function Error"), *GetVoicevoxCoreName());
				ShowVoicevoxErrorMessage(ErrorMessage);
				return PCMData;
			}

			uint8* OutputWAV = nullptr;
			VoicevoxSynthesisOptions Options;
			Options.enable_interrogative_upspeak = bEnableInterrogativeUpspeak;
			uintptr_t OutPutSize = 0;
			if (const VoicevoxResultCode Result = FuncPtr(AudioQueryJson, SpeakerId, Options, &OutPutSize, &OutputWAV);
				Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
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
			const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
		}
	}

	return PCMData;
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UVoicevoxNativeCoreSubsystem::RunSynthesis(const FVoicevoxAudioQuery& AudioQueryJson, const int64 SpeakerId, const bool bEnableInterrogativeUpspeak)
{
	FString OutputJson = "";
	FJsonObjectConverter::UStructToJsonObjectString(AudioQueryJson, OutputJson, 0, 0, 0, nullptr, false);
	
	TArray<uint8> OutputWAV = RunSynthesis(TCHAR_TO_UTF8(*OutputJson), SpeakerId, bEnableInterrogativeUpspeak);
	return OutputWAV;
}

/**
 * @brief voicevox_tts等で生成した音声データを開放する
 */
void UVoicevoxNativeCoreSubsystem::WavFree(uint8* Wav)
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
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_wav_free Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return;
		}
		
		FuncPtr(Wav);
	}
}

/**
 * @brief デフォルトの `voicevox_synthesis` のオプションを生成する
 */
VoicevoxSynthesisOptions UVoicevoxNativeCoreSubsystem::MakeDefaultSynthesisOptions()
{
	const FString FuncName = "voicevox_make_default_synthesis_options"; 
	typedef const VoicevoxSynthesisOptions(*DLL_Function)();
	
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_make_default_synthesis_options Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return VoicevoxSynthesisOptions{};
		}

		return FuncPtr();
	}

	const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
	ShowVoicevoxErrorMessage(Message);
	return VoicevoxSynthesisOptions{};
}

//--------------------------------
// VOICEVOX CORE Property関連
//--------------------------------

/**
 * @brief 話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> UVoicevoxNativeCoreSubsystem::GetMetaList()
{
	const FString FuncName = "voicevox_synthesizer_create_metas_json"; 
	using DLL_Function = char*(*)(const VoicevoxSynthesizer*);
	
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_get_metas_json Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return TArray<FVoicevoxMeta>();
		}
		TArray<FVoicevoxMeta> List;
		char* Metas = FuncPtr(Synthesizer);
		FJsonObjectConverter::JsonArrayStringToUStruct(UTF8_TO_TCHAR(Metas), &List, 0, 0);
		JsonFree(Metas);
		return List;
	}
	
	const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
	ShowVoicevoxErrorMessage(Message);
	return TArray<FVoicevoxMeta>();
}

/**
 * @brief サポートデバイス情報を取得する
 */
FVoicevoxSupportedDevices UVoicevoxNativeCoreSubsystem::GetSupportedDevices()
{
	FVoicevoxSupportedDevices Devices = {};
	// 初期化が行われていない場合はJSON変換時にクラッシュするため、Empty状態で返却する
	if (!bIsInit) return Devices;

	const FString FuncName = "voicevox_onnxruntime_create_supported_devices_json"; 
	using DLL_Function = const VoicevoxResultCode(*)(const VoicevoxOnnxruntime*, char**);
	
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
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_onnxruntime_create_supported_devices_json Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return Devices;
		}
		char* OutputSupportedDevicesJSON;
		if (const VoicevoxResultCode Result = FuncPtr(Onnxruntime, &OutputSupportedDevicesJSON); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(TEXT("voicevox_onnxruntime_create_supported_devices_json"), Result);
			return Devices;
		}
		FJsonObjectConverter::JsonObjectStringToUStruct(UTF8_TO_TCHAR(OutputSupportedDevicesJSON), &Devices, 0, 0);
		JsonFree(OutputSupportedDevicesJSON);
		return Devices;
	}
	
	const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
	ShowVoicevoxErrorMessage(Message);
	return Devices;
}

/**
 * @brief VOICEVOX COREのバージョンを取得する
 */
FString UVoicevoxNativeCoreSubsystem::GetVoicevoxVersion()
{
	const FString FuncName = "voicevox_get_version"; 
	using DLL_Function = const char*(*)();
	
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_get_version Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return FString();
		}
		return UTF8_TO_TCHAR(FuncPtr());
	}

	const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
	ShowVoicevoxErrorMessage(Message);
	return FString();
}

/**
 * @brief ハードウェアアクセラレーションがGPUモードか判定する
 */
bool UVoicevoxNativeCoreSubsystem::IsGpuMode()
{
	const FString FuncName = "voicevox_synthesizer_is_gpu_mode"; 
	typedef bool(*DLL_Function)(const VoicevoxSynthesizer *synthesizer);
	
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
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_synthesizer_is_gpu_mode Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return false;
		}
		return FuncPtr(Synthesizer);
	}
	
	const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
	ShowVoicevoxErrorMessage(Message);
	return false;
}

//--------------------------------
// VOICEVOX CORE PhonemeLength関連
//--------------------------------

/** 
 * @brief 音素列から、音素ごとの長さを求める
 */
TArray<float> UVoicevoxNativeCoreSubsystem::GetPhonemeLength(const int64 Length, TArray<int64> PhonemeList, const int64 SpeakerID)
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
			const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX %s voicevox_predict_duration Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(ErrorMessage);
			return Output;
		}
		
		if (const VoicevoxResultCode Result = FuncPtr(Length, PhonemeList.GetData(), SpeakerID, &OutPutSize, &OutputPredictDurationData); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
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
		const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
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
TArray<float> UVoicevoxNativeCoreSubsystem::FindPitchEachMora(const int64 Length, TArray<int64> VowelPhonemeList, TArray<int64> ConsonantPhonemeList,
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
			const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX %s voicevox_predict_intonation Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(ErrorMessage);
			return Output;
		}

		if (const VoicevoxResultCode Result = FuncPtr(Length, VowelPhonemeList.GetData(), ConsonantPhonemeList.GetData(),
											StartAccentList.GetData(), EndAccentList.GetData(), StartAccentPhraseList.GetData(),
											EndAccentPhraseList.GetData(), SpeakerID, &OutPutSize, &OutputPredictIntonationData); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
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
		const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
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
TArray<float> UVoicevoxNativeCoreSubsystem::DecodeForward(const int64 Length, const int64 PhonemeSize, TArray<float> F0, TArray<float> Phoneme, const int64 SpeakerID)
{
	TArray<float> Output;
	uintptr_t OutPutSize = 0;
	float* OutputDecodeData = nullptr;
	if (CoreLibraryHandle != nullptr)
	{
		const FString FuncName = "voicevox_decode"; 
		const FString FreeFuncName = "voicevox_decode_data_free"; 
		using DLL_Function = const VoicevoxResultCode(*)(uintptr_t length, uintptr_t phoneme_size, float *f0, float *phoneme_vector,
														uint32_t speaker_id, uintptr_t *output_decode_data_length, float **output_decode_data);
		using DLL_FreeFunction = const void(*)(float*);

#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
		const auto FreeFuncPtr = static_cast<DLL_FreeFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
		const auto FreeFuncPtr = (DLL_FreeFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FreeFuncName);
#endif 
		if (!FuncPtr || !FreeFuncPtr)
		{
			const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX %s voicevox_decode Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(ErrorMessage);
			return Output;
		}

		if (const VoicevoxResultCode Result = FuncPtr(Length, PhonemeSize, F0.GetData(), Phoneme.GetData(), SpeakerID, &OutPutSize, &OutputDecodeData); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
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
		const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
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
void UVoicevoxNativeCoreSubsystem::VoicevoxShowErrorResultMessage(const FString& ApiName, VoicevoxResultCode ResultCode)
{
	const FString ErrorMessageFuncName = "voicevox_error_result_to_message";
	using DLL_ErrorFunction = const char*(*)(VoicevoxResultCode Result);
#if PLATFORM_WINDOWS
	const auto ErrorFuncPtr = static_cast<DLL_ErrorFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName));
#elif PLATFORM_MAC
	const auto ErrorFuncPtr = (DLL_ErrorFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *ErrorMessageFuncName);
#endif

	if (!ErrorFuncPtr)
	{
		const FString ErrorMessage = FString::Printf(TEXT("VOICEVOX %s voicevox_error_result_to_message Function Error"), *GetVoicevoxCoreName());
		ShowVoicevoxErrorMessage(ErrorMessage);
	}
	else
	{
#if PLATFORM_WINDOWS
		const FString LastMessage = UTF8_TO_TCHAR(ErrorFuncPtr(static_cast<VoicevoxResultCode>(ResultCode)));
#elif PLATFORM_MAC
		const FString LastMessage = UTF8_TO_TCHAR(ErrorFuncPtr((VoicevoxResultCode)ResultCode));
#endif
		const FString Message = FString::Printf(TEXT("VOICEVOX %s %s Error:%s"), *GetVoicevoxCoreName(), *ApiName, *LastMessage);
		ShowVoicevoxErrorMessage(Message);
	}
}

/**
 * @brief JSON文字列を解放する。
 */
void UVoicevoxNativeCoreSubsystem::JsonFree(char* JsonData)
{
	const FString FuncName = "voicevox_json_free"; 
	using DLL_Function = const void(*)(char*);
	if (CoreLibraryHandle != nullptr)
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			const FString Message = FString::Printf(TEXT("VOICEVOX %s voicevox_json_free Function Error"), *GetVoicevoxCoreName());
			ShowVoicevoxErrorMessage(Message);
			return;
		}
		
		FuncPtr(JsonData);
	}
}
