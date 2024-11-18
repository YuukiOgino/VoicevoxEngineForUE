// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  UEで使用するVoicevoxCORE APIのモジュールCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxUECore.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"

#define LOCTEXT_NAMESPACE "FVoicevoxUECoreModule"

/**
 * @brief StartupModule
 */
void FVoicevoxUECoreModule::StartupModule()
{
    FCoreDelegates::OnAllModuleLoadingPhasesComplete.AddLambda([]
    {
		GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->NativeInitialize();
    });
}

/**
 * @brief ShutdownModule
 */
void FVoicevoxUECoreModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVoicevoxUECoreModule, VoicevoxUECore)