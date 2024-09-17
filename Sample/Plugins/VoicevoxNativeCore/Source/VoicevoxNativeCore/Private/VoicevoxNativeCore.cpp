// Copyright Yuuki Ogino. All Rights Reserved.

#include "VoicevoxNativeCore.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FVoicevoxNativeCoreModule"


void FVoicevoxNativeCoreModule::StartupModule()
{
	
	GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->OnInitialize.AddLambda([](bool bUseGPU, int CPUNumThreads, bool bLoadAllModels)
	{
		
	});
}

void FVoicevoxNativeCoreModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVoicevoxNativeCoreModule, VoicevoxNativeCore)
