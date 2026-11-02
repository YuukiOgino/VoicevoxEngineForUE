// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief VOICEVOX NEMO COREのAPIをまとめたSubsystem　CPPファイル
 * @author Yuuki Ogino
 */

#include "Subsystems/NemoCoreSubsystem.h"
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

	const FString BaseDir = IPluginManager::Get().FindPlugin("VoicevoxNativeCoreNemo")->GetBaseDir();
	FString DllName;
#if PLATFORM_WINDOWS
	DllName = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/VoicevoxCoreNemo/Win64/voicevox_core.dll"));
#elif PLATFORM_MAC
	DllName = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/VoicevoxCoreNemo/Mac/libvoicevox_core_nemo.dylib"));
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

	if (CoreLibraryHandle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(CoreLibraryHandle);
		CoreLibraryHandle = nullptr;
	}
}

//--------------------------------
// VOICEVOX CORE Property関連
//--------------------------------

/**
 * @brief OpenJtakeのディレクトリ名を取得
 */
FString UNemoCoreSubsystem::GetOpenJtakeDirectoryName()
{
	return OPEN_JTALK_DIC_NAME;
}

/**
 * @brief VOICEVOX COREの名前取得
 */
FString UNemoCoreSubsystem::GetVoicevoxCoreName()
{
	return VOICEVOX_NEMO_CORE_NAME;
}