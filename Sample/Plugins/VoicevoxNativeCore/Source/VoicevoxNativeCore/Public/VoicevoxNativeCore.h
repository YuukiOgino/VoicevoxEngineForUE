// Copyright Yuuki Ogino. All Rights Reserved.

#pragma once

#include "Subsystems/VoicevoxCoreSubsystem.h"
#include "Modules/ModuleManager.h"

class FVoicevoxNativeCoreModule : public IModuleInterface
{
	friend class UVoicevoxCoreSubsystem;
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
};
