// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VoicevoxEngineのモジュールCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxEngine.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FVoicevoxEngineModule"

/**
 * @brief StartupModule
 */
void FVoicevoxEngineModule::StartupModule(){}

/**
 * @brief ShutdownModule
 */
void FVoicevoxEngineModule::ShutdownModule(){}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVoicevoxEngineModule, VoicevoxEngine)
