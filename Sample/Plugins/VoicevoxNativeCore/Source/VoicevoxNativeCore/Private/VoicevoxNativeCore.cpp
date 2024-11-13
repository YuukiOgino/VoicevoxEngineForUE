// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VoicevoxNativeCoreモジュールCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxNativeCore.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FVoicevoxNativeCoreModule"

/**
 * @brief StartupModule
 */
void FVoicevoxNativeCoreModule::StartupModule() {}

/**
 * @brief ShutdownModule
 */
void FVoicevoxNativeCoreModule::ShutdownModule() {}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVoicevoxNativeCoreModule, VoicevoxNativeCore)
