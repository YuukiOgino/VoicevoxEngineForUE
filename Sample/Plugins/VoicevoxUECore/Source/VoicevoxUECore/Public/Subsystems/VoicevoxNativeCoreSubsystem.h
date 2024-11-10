// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "VoicevoxNativeCoreSubsystem.generated.h"

/**
 * 
 */
UCLASS(Abstract,  MinimalAPI)
class UVoicevoxNativeCoreSubsystem : public USubsystem
{
	GENERATED_BODY()
public:
	VOICEVOXUECORE_API UVoicevoxNativeCoreSubsystem();

	virtual bool CoreInitialize(bool bUseGPU, int CPUNumThreads = 0, bool bLoadAllModels = false) { return false; }
};
