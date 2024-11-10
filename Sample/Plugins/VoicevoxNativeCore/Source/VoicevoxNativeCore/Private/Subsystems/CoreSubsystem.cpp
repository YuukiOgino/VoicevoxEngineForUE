#include "Subsystems/CoreSubsystem.h"
#include "voicevox_core.h"

UCoreSubsystem::UCoreSubsystem()
{
}

bool UCoreSubsystem::CoreInitialize(bool bUseGPU, int CPUNumThreads, bool bLoadAllModels)
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
		return false;
	}
	
	return true;
}