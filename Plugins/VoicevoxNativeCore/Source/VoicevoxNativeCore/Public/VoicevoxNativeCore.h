// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxNativeCore.h
 * @brief  VoicevoxNativeCoreモジュールヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "Modules/ModuleManager.h"


/**
 * @class FVoicevoxNativeCoreModule
 * @brief VoicevoxNativeCoreモジュールクラス
 */
class FVoicevoxNativeCoreModule final : public IModuleInterface
{
public:

	/**
	 * @brief StartupModule
	 */
	virtual void StartupModule() override;

	/**
	 * @brief ShutdownModule
	 */	
	virtual void ShutdownModule() override;
	
};
