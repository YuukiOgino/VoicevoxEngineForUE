// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxEngine.h
 * @brief  VoicevoxEngineのモジュールヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

/**
 * @class FVoicevoxEngineModule
 * @brief VoicevoxEngineモジュール
 */
class FVoicevoxEngineModule : public IModuleInterface
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
