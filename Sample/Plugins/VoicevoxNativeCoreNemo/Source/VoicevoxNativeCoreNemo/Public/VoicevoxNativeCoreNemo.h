// Copyright Yuuki Ogino. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FVoicevoxNativeCoreNemoModule final : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
