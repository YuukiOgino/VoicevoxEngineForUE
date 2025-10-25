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
	const VoicevoxOnnxruntime* OnnxRuntime = OnnxRuntimeLoadOnce(MakeDefaultLoadOnnxRuntimeOptions());
	if (OnnxRuntime == nullptr) return false;
	if (!OpenJTalkRcNew()) return false;
	bIsInit = SynthesizerNew(*OnnxRuntime, bUseGPU, CPUNumThreads);
	return bIsInit;
}

/**
 * @brief デフォルトの初期化オプションを生成する
 */
VoicevoxInitializeOptions UVoicevoxNativeCoreSubsystem::MakeDefaultInitializeOptions()
{
	if (!IsValidCoreLibraryHandle()) return VoicevoxInitializeOptions{};
	
	const FString FuncName = "voicevox_make_default_initialize_options"; 
	using DLL_Function = const VoicevoxInitializeOptions(*)();

#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
	
	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
		return VoicevoxInitializeOptions{};
	}

	return FuncPtr();
}

//--------------------------------
// VOICEVOX CORE Finalize関連
//--------------------------------

/**
 * @brief 終了処理を行う。以降VOICEVOXのAPIを利用するためには再度Initializeメソッドを行う必要がある。
 */
void UVoicevoxNativeCoreSubsystem::Finalize()
{
	if (!IsValidCoreLibraryHandle()) return;
	
	if (OpenJTalk != nullptr)
	{
		OpenJTalkRcDelete(OpenJTalk);
	}
	
	if (Synthesizer != nullptr)
	{
		SynthesizerDelete();
		bIsInit = false;
	}
}

/**
 * @brief VoicevoxSynthesizer を<b>構築</b>する。
 */
bool UVoicevoxNativeCoreSubsystem::SynthesizerNew(const VoicevoxOnnxruntime& OnnxRuntime, const bool bUseGPU, const int CPUNumThreads)
{
	if (!IsValidCoreLibraryHandle()) return false;

	const FString FuncName = "voicevox_synthesizer_new"; 
	using DLL_Function = const VoicevoxResultCode(*)(const VoicevoxOnnxruntime*, const OpenJtalkRc*, VoicevoxInitializeOptions, VoicevoxSynthesizer**);

#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 

	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
		return false;
	}
		
	VoicevoxInitializeOptions Option;
	Option.acceleration_mode = bUseGPU ? VoicevoxAccelerationMode::VOICEVOX_ACCELERATION_MODE_GPU : VoicevoxAccelerationMode::VOICEVOX_ACCELERATION_MODE_CPU;
	Option.cpu_num_threads = CPUNumThreads;

	if (const VoicevoxResultCode Result = FuncPtr(&OnnxRuntime, OpenJTalk, Option, &Synthesizer); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
	{
		VoicevoxShowErrorResultMessage(FuncName, Result);
		OpenJTalkRcDelete(OpenJTalk);
		return false;
	}

	return true;
}

/**
 * @brief VoicevoxSynthesizer を<b>破棄</b>する。
 */
void UVoicevoxNativeCoreSubsystem::SynthesizerDelete()
{
	if (!IsValidCoreLibraryHandle()) return;

	const FString FuncName = "voicevox_synthesizer_delete"; 
	using DLL_Function = const void(*)(VoicevoxSynthesizer*);

#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
		return;
	}
	
	FuncPtr(Synthesizer);
	Synthesizer = nullptr;
}

//--------------------------------
// VOICEVOX CORE OpenJtake関連
//--------------------------------

/**
 * @brief テキスト解析器としてのOpen JTalkを作成する。
 * @return 取得結果
 */
bool UVoicevoxNativeCoreSubsystem::OpenJTalkRcNew()
{
	if (!IsValidCoreLibraryHandle()) return false;
	const FString PlatformFolderName = GetPlatformFolderName();
	if (PlatformFolderName.IsEmpty())  return false;
	
	const FString OpenJTalkRcNewFuncName = "voicevox_open_jtalk_rc_new"; 
	using DLL_OpenJTalkRcNewFunction = const VoicevoxResultCode(*)(const char*, OpenJtalkRc**);

#if PLATFORM_WINDOWS
	const auto OpenJTalkRcFuncPtr = static_cast<DLL_OpenJTalkRcNewFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *OpenJTalkRcNewFuncName));
#elif PLATFORM_MAC
	const auto OpenJTalkRcFuncPtr = (DLL_OpenJTalkRcNewFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *OpenJTalkRcNewFuncName);
#endif

	if (!OpenJTalkRcFuncPtr)
	{
		ShowDllErrorMessage(OpenJTalkRcNewFuncName);
		return false;
	}
		
	const FString JTalkPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries"), PlatformFolderName, GetOpenJTakeDirectoryName()));
	if (const VoicevoxResultCode Result = OpenJTalkRcFuncPtr(TCHAR_TO_UTF8(*JTalkPath), &OpenJTalk); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
	{
		VoicevoxShowErrorResultMessage(OpenJTalkRcNewFuncName, Result);
		return false;
	}

	return true;
}

/**
 * @brief OpenJtalkRc を<b>破棄</b>(_destruct_)する。
 */
void UVoicevoxNativeCoreSubsystem::OpenJTalkRcDelete(OpenJtalkRc* Rc)
{
	const FString FuncName = "voicevox_open_jtalk_rc_delete"; 
	using DLL_Function = const void(*)(OpenJtalkRc*);
	if (IsValidCoreLibraryHandle())
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			ShowDllErrorMessage(FuncName);
			return;
		}
		
		FuncPtr(Rc);
	}
}

//--------------------------------
// VOICEVOX CORE ONNX Runtime関連
//--------------------------------

/**
 * @brief VoicevoxOnnxruntime のインスタンスを得る。
 * @returns ::VoicevoxOnnxruntime のインスタンス
 */
const VoicevoxOnnxruntime* UVoicevoxNativeCoreSubsystem::SynthesizerGetOnnxRuntime()
{
	const FString FuncName = "voicevox_synthesizer_get_onnxruntime"; 
	using DLL_Function = const VoicevoxOnnxruntime*(*)(const VoicevoxSynthesizer*);

	if (IsValidCoreLibraryHandle())
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			ShowDllErrorMessage(FuncName);
			return nullptr;
		}
		
		return FuncPtr(Synthesizer);
	}
	return nullptr;
}

/**
 * @brief VoicevoxOnnxruntime のインスタンスが既に作られているならそれを得る。 作られていなければ`NULL`を返す。
 */
const VoicevoxOnnxruntime* UVoicevoxNativeCoreSubsystem::GetOnnxRuntime()
{
	const FString FuncName = "voicevox_onnxruntime_get"; 
	using DLL_Function = const VoicevoxOnnxruntime*(*)();
	
	if (IsValidCoreLibraryHandle())
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			ShowDllErrorMessage(FuncName);
			return nullptr;
		}
		
		return FuncPtr();
	}
	return nullptr;
}

/**
 * @brief ONNX Runtimeをロードして初期化する。
 */
const VoicevoxOnnxruntime* UVoicevoxNativeCoreSubsystem::OnnxRuntimeLoadOnce(const VoicevoxLoadOnnxruntimeOptions& Option)
{
	if (!IsValidCoreLibraryHandle()) return nullptr;
	const FString OnnxRuntimeLoadOnceFuncName = "voicevox_onnxruntime_load_once"; 
	using DLL_OnnxRuntimeLoadOnceFunction = const VoicevoxResultCode(*)(VoicevoxLoadOnnxruntimeOptions, const VoicevoxOnnxruntime**);

#if PLATFORM_WINDOWS
	const auto LoadOnceFuncPtr = static_cast<DLL_OnnxRuntimeLoadOnceFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *OnnxRuntimeLoadOnceFuncName));
#elif PLATFORM_MAC
	const auto LoadOnceFuncPtr = (DLL_OnnxRuntimeLoadOnceFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *OnnxRuntimeLoadOnceFuncName);
#endif

	if (!LoadOnceFuncPtr)
	{
		ShowDllErrorMessage(OnnxRuntimeLoadOnceFuncName);
		return nullptr;
	}

	//! ONNX Runtime。
	const VoicevoxOnnxruntime* OnnxRuntime;
	if (const VoicevoxResultCode Result = LoadOnceFuncPtr(Option, &OnnxRuntime); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
	{
		VoicevoxShowErrorResultMessage(OnnxRuntimeLoadOnceFuncName, Result);
		return nullptr;
	}

	return OnnxRuntime;
}

/**
 * @brief ONNX Runtimeを初期化する。
 */
const VoicevoxOnnxruntime* UVoicevoxNativeCoreSubsystem::OnnxRuntimeInitOnce()
{
	const FString FuncName = "voicevox_onnxruntime_init_once"; 
	using DLL_Function = const VoicevoxResultCode(*)(const VoicevoxOnnxruntime**);
	const VoicevoxOnnxruntime* Runtime;
	if (IsValidCoreLibraryHandle())
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			ShowDllErrorMessage(FuncName);
			return nullptr;
		}

		if (const VoicevoxResultCode Result = FuncPtr(&Runtime); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(FuncName, Result);
			return nullptr;
		}
		
		return Runtime;
	}
	return nullptr;
}

/**
 * @brief デフォルトのvoicevox_onnxruntime_load_once のオプションを生成する。
 */
VoicevoxLoadOnnxruntimeOptions UVoicevoxNativeCoreSubsystem::MakeDefaultLoadOnnxRuntimeOptions()
{
	if (!IsValidCoreLibraryHandle()) return VoicevoxLoadOnnxruntimeOptions{};
	
	const FString MakeDefaultFuncName = "voicevox_make_default_load_onnxruntime_options"; 
	using DLL_MakeDefaultLoadOnnxRuntimeOptionsFunction = const VoicevoxLoadOnnxruntimeOptions(*)();

#if PLATFORM_WINDOWS
	const auto MakeDefaultFuncPtr = static_cast<DLL_MakeDefaultLoadOnnxRuntimeOptionsFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *MakeDefaultFuncName));
#elif PLATFORM_MAC
	const auto MakeDefaultFuncPtr = (DLL_MakeDefaultLoadOnnxRuntimeOptionsFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *MakeDefaultFuncName);
#endif

	if (!MakeDefaultFuncPtr)
	{
		ShowDllErrorMessage(MakeDefaultFuncName);
		return VoicevoxLoadOnnxruntimeOptions{};
	}

	return MakeDefaultFuncPtr();
}

/**
 * @brief ONNX Runtimeの動的ライブラリの、バージョン付きのファイル名を取得。
 */
FString UVoicevoxNativeCoreSubsystem::GetOnnxRuntimeLibVersionedFilename()
{
	FString Name;
	Name.Empty();
	const FString FuncName = "voicevox_get_onnxruntime_lib_versioned_filename"; 
	using DLL_Function = const char*(*)();
	
	if (IsValidCoreLibraryHandle())
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			ShowDllErrorMessage(FuncName);
			return Name;
		}
		
		Name = UTF8_TO_TCHAR(FuncPtr());
	}
	return Name;
}

/**
 * @brief ONNX Runtimeの動的ライブラリの、バージョン無しのファイル名を取得。
 */
FString UVoicevoxNativeCoreSubsystem::GetOnnxRuntimeLibUnversionedFilename()
{
	FString Name;
	Name.Empty();
	const FString FuncName = "voicevox_get_onnxruntime_lib_unversioned_filename"; 
	using DLL_Function = const char*(*)();
	
	if (IsValidCoreLibraryHandle())
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			ShowDllErrorMessage(FuncName);
			return Name;
		}
		
		Name = UTF8_TO_TCHAR(FuncPtr());
	}
	return Name;
}

/**
 * @brief 日本語のテキストを解析する。
 */
char* UVoicevoxNativeCoreSubsystem::OpenJTalkRcAnalyze(const FString& Text)
{
	const FString FuncName = "voicevox_open_jtalk_rc_analyze"; 
	using DLL_Function = const VoicevoxResultCode(*)(const OpenJtalkRc*, const char*, char**);
	
	if (IsValidCoreLibraryHandle())
	{
#if PLATFORM_WINDOWS
		const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
		const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		if (!FuncPtr)
		{
			ShowDllErrorMessage(FuncName);
			return nullptr;
		}

		char* AccentPhrases;
		if (const VoicevoxResultCode Result = FuncPtr(OpenJTalk, TCHAR_TO_UTF8(*Text), &AccentPhrases); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(FuncName, Result);
			return nullptr;
		}
		JsonFree(AccentPhrases);
		return AccentPhrases;
	}
	return nullptr;
}

//--------------------------------
// VOICEVOX CORE Model関連(旧API)
//--------------------------------

/**
 * @brief モデルをロードする。
 */
bool UVoicevoxNativeCoreSubsystem::LoadModel(const int64 SpeakerId)
{
	if (IsModel(SpeakerId)) return true;
	
	const FString PlatformFolderName = GetPlatformFolderName();
	if (PlatformFolderName.IsEmpty())
	{
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
			continue;
		}

		for (auto Metas = VoiceModelFileCreateMetas(*Model);
			auto Meta : Metas)
		{
			const auto i = Meta.Styles.IndexOfByPredicate(
			[&](const FVoicevoxStyle& Item)
			{
				return Item.Id == SpeakerId;
			});

			if (i >= 0)
			{
				if (!SynthesizerLoadVoiceModel(*Model))
				{
					VoiceModelFileDelete(*Model);
					return false;
				}

				const FString FileNameOnly = FPaths::GetBaseFilename(FileName);
				ModelIdMap.Add(FileNameOnly, VoiceModelFileId(*Model));
				VoiceModelFileDelete(*Model);
				return true;
			}
		}
		
		VoiceModelFileDelete(*Model);
	}
	
	return false;
}

/**
 * @brief 使用するCOREにスピーカーモデルが存在するか
 */
bool UVoicevoxNativeCoreSubsystem::IsModel(const int64 SpeakerId)
{
	for (auto [Name, Styles, Speaker_uuid, Version] : GetMetaList())
	{
		const auto i = Styles.IndexOfByPredicate(
			[&](const FVoicevoxStyle& Item)
			{
				return Item.Id == SpeakerId;
			});
		
		if (i >= 0) return true;
	}
	return false;
}

//--------------------------------
// VOICEVOX CORE VMM関連
//--------------------------------

/**
 * @brief　VVMファイルのロード実行
 */
bool UVoicevoxNativeCoreSubsystem::LoadVoiceModel(const FString VvmFileName)
{
	const FString PlatformFolderName = GetPlatformFolderName();
	if (PlatformFolderName.IsEmpty())
	{
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
	const FString PlatformFolderName = GetPlatformFolderName();
	if (PlatformFolderName.IsEmpty())
	{
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
			VoiceModelFileDelete(*Model);
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
	if (!IsValidCoreLibraryHandle()) return false;
	const FString FileOpenFuncName = "voicevox_voice_model_file_open";
	using DLL_FileOpenFunction = const VoicevoxResultCode(*)(const char*, VoicevoxVoiceModelFile**);

#if PLATFORM_WINDOWS
	const auto FileOpenFuncPtr = static_cast<DLL_FileOpenFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileOpenFuncName));
#elif PLATFORM_MAC
	const auto FileOpenFuncPtr = (DLL_FileOpenFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileOpenFuncName);
#endif

	if (!FileOpenFuncPtr)
	{
		ShowDllErrorMessage(FileOpenFuncName);
		return false;
	}

	if (const VoicevoxResultCode Result = FileOpenFuncPtr(TCHAR_TO_UTF8(*Path), Model);
		Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
	{
		VoicevoxShowErrorResultMessage(FileOpenFuncName, Result);
		return false;
	}
		
	return true;
}

/**
 * 音声モデルを読み込む。
 */
bool UVoicevoxNativeCoreSubsystem::SynthesizerLoadVoiceModel(const VoicevoxVoiceModelFile& Model)
{
	if (!IsValidCoreLibraryHandle()) return false;
	
	const FString LoadFuncName = "voicevox_synthesizer_load_voice_model";
	using DLL_LoadFunction = const VoicevoxResultCode(*)(const VoicevoxSynthesizer*, const VoicevoxVoiceModelFile*);
#if PLATFORM_WINDOWS
	const auto LoadFuncPtr = static_cast<DLL_LoadFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *LoadFuncName));
#elif PLATFORM_MAC
	const auto LoadFuncPtr = (DLL_LoadFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *LoadFuncName);
#endif

	if (!LoadFuncPtr)
	{
		ShowDllErrorMessage(LoadFuncName);
		return false;
	}
		
	if (const VoicevoxResultCode Result = LoadFuncPtr(Synthesizer, &Model); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
	{
		VoicevoxShowErrorResultMessage(LoadFuncName, Result);
		return false;
	}
	return true;
}

/**
 * @brief VoicevoxVoiceModelFile を、所有しているファイルディスクリプタを閉じた上で<b>破棄</b>する。
 */
void UVoicevoxNativeCoreSubsystem::VoiceModelFileDelete(VoicevoxVoiceModelFile& Model)
{
	if (!IsValidCoreLibraryHandle()) return;
	const FString FileDeleteFuncName = "voicevox_voice_model_file_delete";
	using DLL_FileDeleteFunction = const void(*)(VoicevoxVoiceModelFile*);

#if PLATFORM_WINDOWS
	const auto FileDeleteFuncPtr = static_cast<DLL_FileDeleteFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileDeleteFuncName));
#elif PLATFORM_MAC
	const auto FileDeleteFuncPtr = (DLL_LoadFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileDeleteFuncName);
#endif

	if (!FileDeleteFuncPtr)
	{
		ShowDllErrorMessage(FileDeleteFuncName);
		return;
	}
	FileDeleteFuncPtr(&Model);
}

/**
 * @brief  VoicevoxVoiceModelFile からIDを取得する。
 */
TArray<uint8_t> UVoicevoxNativeCoreSubsystem::VoiceModelFileId(const VoicevoxVoiceModelFile& Model)
{
	TArray<uint8_t> Output;
	if (!IsValidCoreLibraryHandle()) return Output;
	const FString FileIdFuncName = "voicevox_voice_model_file_id";
	using DLL_FileIdFunction = const void(*)(const VoicevoxVoiceModelFile*, uint8_t(*)[16]);

#if PLATFORM_WINDOWS
	const auto FileIdFuncPtr = static_cast<DLL_FileIdFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileIdFuncName));
#elif PLATFORM_MAC
	const auto FileIdFuncPtr = (DLL_FileIdFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileIdFuncName);
#endif

	if (!FileIdFuncPtr)
	{
		ShowDllErrorMessage(FileIdFuncName);
	}
	else
	{
		uint8_t ModelId[16];
		FileIdFuncPtr(&Model, &ModelId);
		Output.Init(0, 16);
		FMemory::Memcpy(Output.GetData(), ModelId, 16);
	}
	
	return Output;
}

/**
 * @breaf 音声モデルの読み込みを解除する。
 */
bool UVoicevoxNativeCoreSubsystem::UnloadVoiceModel(const FString VvmFileName)
{
	if (!IsValidCoreLibraryHandle()) return false;
	const FString UnloadFuncName = "voicevox_synthesizer_unload_voice_model";
	using DLL_UnloadFunction = const VoicevoxResultCode(*)(const VoicevoxSynthesizer*, VoicevoxVoiceModelId);

#if PLATFORM_WINDOWS
	const auto UnloadFuncPtr = static_cast<DLL_UnloadFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *UnloadFuncName));
#elif PLATFORM_MAC
	const auto UnloadFuncPtr = (DLL_UnloadFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *UnloadFuncName);
#endif

	if (!UnloadFuncPtr)
	{
		ShowDllErrorMessage(UnloadFuncName);
	}
	else if (const auto ModelId = ModelIdMap.Find(VvmFileName); ModelId != nullptr)
	{
		if (const VoicevoxResultCode Result = UnloadFuncPtr(Synthesizer, reinterpret_cast<VoicevoxVoiceModelId>(ModelId->GetData()));
				Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(UnloadFuncName, Result);
		}
		else
		{
			ModelIdMap.Remove(VvmFileName);
			return true;
		}
	}

	return false;
}

/**
 * @breaf 指定したIDの音声モデルが読み込まれているか判定する。
 */
bool UVoicevoxNativeCoreSubsystem::IsLoadedVoiceModel(const FString VvmFileName)
{
	if (!IsValidCoreLibraryHandle()) return false;
	
	const FString IsFileLoadedFuncName = "voicevox_synthesizer_is_loaded_voice_model";
	using DLL_IsFileLoadedFunction = const bool(*)(const VoicevoxSynthesizer*, VoicevoxVoiceModelId);

#if PLATFORM_WINDOWS
	const auto IsFileLoadedFuncPtr = static_cast<DLL_IsFileLoadedFunction>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *IsFileLoadedFuncName));
#elif PLATFORM_MAC
	const auto IsFileLoadedFuncPtr = (DLL_IsFileLoadedFunction)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FileOpenFuncName);
#endif

	if (!IsFileLoadedFuncPtr)
	{
		ShowDllErrorMessage(IsFileLoadedFuncName);
	}
	else if (const auto ModelId = ModelIdMap.Find(VvmFileName); ModelId != nullptr)
	{
		return IsFileLoadedFuncPtr(Synthesizer, reinterpret_cast<VoicevoxVoiceModelId>(ModelId->GetData()));
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
	if (!bIsInit) return AudioQuery; 
	
	if (!IsValidCoreLibraryHandle()) return AudioQuery;
		
	const FString FuncName = bKana ? "voicevox_synthesizer_create_audio_query_from_kana" : "voicevox_synthesizer_create_audio_query"; 
	using DLL_Function = const VoicevoxResultCode(*)(const VoicevoxSynthesizer*, const char*, VoicevoxStyleId, char**);

#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 

	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
	}
	else
	{
		char* Output = nullptr;
		if (const VoicevoxResultCode Result = FuncPtr(Synthesizer, TCHAR_TO_UTF8(*Message), SpeakerId, &Output);
			Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(FuncName, Result);
		}
		else
		{
			FJsonObjectConverter::JsonObjectStringToUStruct(UTF8_TO_TCHAR(Output), &AudioQuery, 0, 0);
			JsonFree(Output);
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
	
	if (!IsValidCoreLibraryHandle()) return PCMData;
	
	const FString FuncName = bKana ? "voicevox_synthesizer_tts_from_kana" : "voicevox_synthesizer_tts"; 
	using DLL_Function = const VoicevoxResultCode(*)(const VoicevoxSynthesizer*, const char*, VoicevoxStyleId, VoicevoxTtsOptions, uintptr_t*, uint8_t**);

#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 

	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
	}
	else
	{
		uint8* OutputWAV = nullptr;
		VoicevoxTtsOptions Options;
		Options.enable_interrogative_upspeak = bEnableInterrogativeUpspeak;
		uintptr_t OutPutSize = 0;
		
		if (const VoicevoxResultCode Result = FuncPtr(Synthesizer, TCHAR_TO_UTF8(*Message), SpeakerId, Options, &OutPutSize, &OutputWAV);
			Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(FuncName, Result);
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
 * @brief デフォルトのテキスト音声合成オプションを生成する
 */
VoicevoxTtsOptions UVoicevoxNativeCoreSubsystem::MakeDefaultTtsOptions()
{
	if (!IsValidCoreLibraryHandle()) return VoicevoxTtsOptions{};
	
	const FString FuncName = "voicevox_make_default_tts_options"; 
	using DLL_Function = const VoicevoxTtsOptions(*)();
#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif
	
	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
		return VoicevoxTtsOptions{};
	}
	return FuncPtr();
}

//--------------------------------
// VOICEVOX CORE Synthesis関連
//--------------------------------

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UVoicevoxNativeCoreSubsystem::RunSynthesis(const char* AudioQueryJson, const int64 SpeakerId, bool bEnableInterrogativeUpspeak)
{
	TArray<uint8> PCMData;
	PCMData.Empty();
	
	if (!IsValidCoreLibraryHandle()) return PCMData;
	
	const FString FuncName = "voicevox_synthesizer_synthesis"; 
	using DLL_Function = const VoicevoxResultCode(*)(const VoicevoxSynthesizer*, const char*, VoicevoxStyleId, VoicevoxSynthesisOptions, uintptr_t*, uint8_t**);
#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
	}
	else
	{
		uint8* OutputWAV = nullptr;
		VoicevoxSynthesisOptions Options;
		Options.enable_interrogative_upspeak = bEnableInterrogativeUpspeak;
		uintptr_t OutPutSize = 0;
		if (const VoicevoxResultCode Result = FuncPtr(Synthesizer, AudioQueryJson, SpeakerId, Options, &OutPutSize, &OutputWAV);
			Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
		{
			VoicevoxShowErrorResultMessage(FuncName, Result);
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
TArray<uint8> UVoicevoxNativeCoreSubsystem::RunSynthesis(const FVoicevoxAudioQuery& AudioQueryJson, const int64 SpeakerId, const bool bEnableInterrogativeUpspeak)
{
	FString OutputJson = "";
	FJsonObjectConverter::UStructToJsonObjectString(AudioQueryJson, OutputJson, 0, 0, 0, nullptr, false);
	
	TArray<uint8> OutputWAV = RunSynthesis(TCHAR_TO_UTF8(*OutputJson), SpeakerId, bEnableInterrogativeUpspeak);
	return OutputWAV;
}

/**
 * @brief デフォルトの `voicevox_synthesis` のオプションを生成する
 */
VoicevoxSynthesisOptions UVoicevoxNativeCoreSubsystem::MakeDefaultSynthesisOptions()
{
	if (!IsValidCoreLibraryHandle()) return VoicevoxSynthesisOptions{};
	const FString FuncName = "voicevox_make_default_synthesis_options"; 
	using DLL_Function = const VoicevoxSynthesisOptions(*)();
	
#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
		
	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
		return VoicevoxSynthesisOptions{};
	}
	return FuncPtr();
}

//--------------------------------
// VOICEVOX CORE Property関連
//--------------------------------

/**
 * @brief コアライブラリをロード済みか
 */
bool UVoicevoxNativeCoreSubsystem::IsValidCoreLibraryHandle()
{
	if (CoreLibraryHandle == nullptr)
	{
		const FString Message =  FString::Printf(TEXT("VOICEVOX %s LoadError!!"), *GetVoicevoxCoreName());
		ShowVoicevoxErrorMessage(Message);
		return false;
	}

	return true;
}

/**
 * @brief プラットフォームのフォルダ名を取得
 * @return プラットフォームのフォルダ名。空の場合は非対応
 */
FString UVoicevoxNativeCoreSubsystem::GetPlatformFolderName()
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
	}

	return PlatformFolderName;
}

/**
 * @brief 話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> UVoicevoxNativeCoreSubsystem::GetMetaList()
{
	if (!IsValidCoreLibraryHandle()) return TArray<FVoicevoxMeta>();
	
	const FString FuncName = "voicevox_synthesizer_create_metas_json"; 
	using DLL_Function = char*(*)(const VoicevoxSynthesizer*);
#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
		return TArray<FVoicevoxMeta>();
	}
	TArray<FVoicevoxMeta> List;
	char* Metas = FuncPtr(Synthesizer);
	FJsonObjectConverter::JsonArrayStringToUStruct(UTF8_TO_TCHAR(Metas), &List, 0, 0);
	JsonFree(Metas);
	return List;
}

/**
 * @brief 指定のVoicevoxVoiceModelFileから話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> UVoicevoxNativeCoreSubsystem::GetVoiceModelFileMetaList(const FString VvmFileName)
{
	const FString PlatformFolderName = GetPlatformFolderName();
	if (PlatformFolderName.IsEmpty())
	{
		return TArray<FVoicevoxMeta>();
	}
	
	VoicevoxVoiceModelFile* Model = nullptr;
	FString VVMName = VvmFileName + TEXT(".vvm");
	if (const FString VmmPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries"), PlatformFolderName, TEXT("models"), TEXT("vvms"), VVMName));
		!VoiceModelFileOpen(VmmPath, &Model))
	{
		return TArray<FVoicevoxMeta>();
	}

	auto Metas = VoiceModelFileCreateMetas(*Model);
	VoiceModelFileDelete(*Model);

	return Metas;
}

/**
 * @brief VoicevoxVoiceModelFile からメタ情報を取得する。
 * @param [in] Model 音声モデル
 * @returns VoicevoxVoiceModelFileのメタ情報
 */
TArray<FVoicevoxMeta> UVoicevoxNativeCoreSubsystem::VoiceModelFileCreateMetas(const VoicevoxVoiceModelFile& Model)
{
	if (!IsValidCoreLibraryHandle()) return TArray<FVoicevoxMeta>();
	const FString FuncName = "voicevox_voice_model_file_create_metas_json"; 
	using DLL_Function = char*(*)(const VoicevoxVoiceModelFile*);
	
#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
		return TArray<FVoicevoxMeta>();
	}
	TArray<FVoicevoxMeta> List;
	char* Metas = FuncPtr(&Model);
	FJsonObjectConverter::JsonArrayStringToUStruct(UTF8_TO_TCHAR(Metas), &List, 0, 0);
	JsonFree(Metas);
	return List;
}

/**
 * @brief サポートデバイス情報を取得する
 */
FVoicevoxSupportedDevices UVoicevoxNativeCoreSubsystem::GetSupportedDevices()
{
	FVoicevoxSupportedDevices Devices = {};
	// 初期化が行われていない場合はJSON変換時にクラッシュするため、Empty状態で返却する
	if (!IsValidCoreLibraryHandle() || !bIsInit) return Devices;

	const FString FuncName = "voicevox_onnxruntime_create_supported_devices_json"; 
	using DLL_Function = const VoicevoxResultCode(*)(const VoicevoxOnnxruntime*, char**);
#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
		return Devices;
	}
	char* OutputSupportedDevicesJSON;
	const VoicevoxOnnxruntime* OnnxRuntime = SynthesizerGetOnnxRuntime();
	if (OnnxRuntime == nullptr)
	{
		return Devices;
	}
		
	if (const VoicevoxResultCode Result = FuncPtr(OnnxRuntime, &OutputSupportedDevicesJSON); Result != VoicevoxResultCode::VOICEVOX_RESULT_OK)
	{
		VoicevoxShowErrorResultMessage(FuncName, Result);
		return Devices;
	}
	FJsonObjectConverter::JsonObjectStringToUStruct(UTF8_TO_TCHAR(OutputSupportedDevicesJSON), &Devices, 0, 0);
	JsonFree(OutputSupportedDevicesJSON);
	return Devices;
}

/**
 * @brief VOICEVOX COREのバージョンを取得する
 */
FString UVoicevoxNativeCoreSubsystem::GetVoicevoxVersion()
{
	if (!IsValidCoreLibraryHandle()) return FString();
	const FString FuncName = "voicevox_get_version"; 
	using DLL_Function = const char*(*)();
#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
		return FString();
	}
	return UTF8_TO_TCHAR(FuncPtr());
}

/**
 * @brief ハードウェアアクセラレーションがGPUモードか判定する
 */
bool UVoicevoxNativeCoreSubsystem::IsGpuMode()
{
	if (!IsValidCoreLibraryHandle()) return false;
	const FString FuncName = "voicevox_synthesizer_is_gpu_mode"; 
	using DLL_Function = bool(*)(const VoicevoxSynthesizer*);
#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
		return false;
	}
	return FuncPtr(Synthesizer);
}

//--------------------------------
// VOICEVOX CORE Free関連
//--------------------------------

/**
 * @brief voicevox_tts等で生成した音声データを開放する
 */
void UVoicevoxNativeCoreSubsystem::WavFree(uint8* Wav)
{
	if (!IsValidCoreLibraryHandle()) return;
	const FString FuncName = "voicevox_wav_free"; 
	using DLL_Function = const void(*)(uint8_t* WAV);
#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
		return;
	}
	FuncPtr(Wav);
}

/**
 * @brief JSON文字列を解放する。
 */
void UVoicevoxNativeCoreSubsystem::JsonFree(char* JsonData)
{
	if (!IsValidCoreLibraryHandle()) return;
	const FString FuncName = "voicevox_json_free"; 
	using DLL_Function = const void(*)(char*);
#if PLATFORM_WINDOWS
	const auto FuncPtr = static_cast<DLL_Function>(FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName));
#elif PLATFORM_MAC
	const auto FuncPtr = (DLL_Function)FPlatformProcess::GetDllExport(CoreLibraryHandle, *FuncName);
#endif 
	if (!FuncPtr)
	{
		ShowDllErrorMessage(FuncName);
		return;
	}
	FuncPtr(JsonData);
}

//--------------------------------
// VOICEVOX CORE Error関連
//--------------------------------

/**
 * エラー結果をメッセージに変換して表示
 */
void UVoicevoxNativeCoreSubsystem::VoicevoxShowErrorResultMessage(const FString& ApiName, const VoicevoxResultCode ResultCode)
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
		ShowDllErrorMessage(ErrorMessageFuncName);
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
 * DLL読み込み失敗をメッセージに変換して表示
 */
void UVoicevoxNativeCoreSubsystem::ShowDllErrorMessage(const FString& ApiName)
{
	const FString Message = FString::Printf(TEXT("VOICEVOX %s %s Function Error"), *GetVoicevoxCoreName(), *ApiName);
	ShowVoicevoxErrorMessage(Message);
}