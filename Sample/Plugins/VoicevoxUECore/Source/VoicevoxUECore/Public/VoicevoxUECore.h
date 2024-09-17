// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxUECore.h
 * @brief  UEで使用するVoicevoxCORE APIのモジュールヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "Modules/ModuleManager.h"

/**
* @class FVoicevoxUECoreModule
 * @brief UE用VoicevoxCOREモジュール
 */
class FVoicevoxUECoreModule : public IModuleInterface
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
