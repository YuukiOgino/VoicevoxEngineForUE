// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxNativeCoreNemo.h
 * @brief  VoicevoxNativeCoreNemoモジュールヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "Modules/ModuleManager.h"

//----------------------------------------------------------------
// class
//----------------------------------------------------------------

/**
 * @class FVoicevoxNativeCoreNemoModule
 * @brief VoicevoxNativeCoreNemoモジュールクラス
 */
class FVoicevoxNativeCoreNemoModule final : public IModuleInterface
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
