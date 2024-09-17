// Copyright Yuuki Ogino. All Rights Reserved.

#include "VoicevoxNativeCore.h"
#include "Modules/ModuleManager.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"
#include "VoicevoxCoreUtil.h"

#define LOCTEXT_NAMESPACE "FVoicevoxNativeCoreModule"

/**
 * @brief StartupModule
 */
void FVoicevoxNativeCoreModule::StartupModule()
{
	FCoreDelegates::OnAllModuleLoadingPhasesComplete.AddLambda([]
	{
		const FVoicevoxCoreInitializeDelegate InitializeEvent =
			FVoicevoxCoreInitializeDelegate::CreateLambda([](const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
			{
				const bool bResult = FVoicevoxCoreUtil::Initialize(bUseGPU, CPUNumThreads, bLoadAllModels);
				if (bResult)
				{
					GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->AddMetaList(FVoicevoxCoreUtil::GetMetaList());
					GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->AddSupportedDevices(VOICEVOX_CORE_NAME, FVoicevoxCoreUtil::GetSupportedDevices());
					GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->AddVoicevoCoreVersion(VOICEVOX_CORE_NAME, FVoicevoxCoreUtil::GetVoicevoxVersion());
				}
				GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SetInitializeResult(bResult);
			});
		GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SetOnInitializeDelegate(InitializeEvent);

		const TVoicevoxCoreDelegate<void()> FinalizeEvent =
			TVoicevoxCoreDelegate<void()>::CreateLambda([]
			{
				FVoicevoxCoreUtil::Finalize();
				GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SetFinalizeResult(true);
			});
		GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SetOnFinalizeDelegate(FinalizeEvent);
	});
}

/**
 * @brief ShutdownModule
 */
void FVoicevoxNativeCoreModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVoicevoxNativeCoreModule, VoicevoxNativeCore)
