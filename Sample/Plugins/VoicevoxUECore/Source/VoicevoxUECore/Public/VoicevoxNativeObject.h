// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/VoicevoxNativeCoreSubsystem.h"
#include "UObject/Object.h"
#include "VoicevoxNativeObject.generated.h"

/**
 * 
 */
UCLASS()
class UVoicevoxNativeObject : public UObject
{
	GENERATED_BODY()
public:
	VOICEVOXUECORE_API virtual void Init();
	
private:
	FObjectSubsystemCollection<UVoicevoxNativeCoreSubsystem> VoicevoxSubsystemCollection;
};
