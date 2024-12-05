// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief VOICEVOX COREのAPIをまとめたSubsystem CPPファイル
 * @author Yuuki Ogino
 */

#include "Subsystems/CoreSubsystem.h"
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
FString UCoreSubsystem::GetOpenJtakeDirectoryName()
{
	return OPEN_JTALK_DIC_NAME;
}

/**
 * @brief VOICEVOX COREの名前取得
 */
FString UCoreSubsystem::GetVoicevoxCoreName()
{
	return VOICEVOX_CORE_NAME;
}