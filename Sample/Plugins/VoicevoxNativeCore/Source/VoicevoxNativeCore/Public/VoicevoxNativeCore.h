// Copyright Yuuki Ogino. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

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
