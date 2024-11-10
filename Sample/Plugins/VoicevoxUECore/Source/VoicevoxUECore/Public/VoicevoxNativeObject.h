// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/VoicevoxNativeCoreSubsystem.h"
#include "UObject/Object.h"
#include "VoicevoxNativeObject.generated.h"

/**
 * 
 */
UCLASS(transient)
class UVoicevoxNativeObject : public UObject
{
	GENERATED_BODY()
	
	TArray<UClass*> SubsystemClasses;
	
public:
	
	UVoicevoxNativeCoreSubsystem* GetSubsystemBase(TSubclassOf<UVoicevoxNativeCoreSubsystem> SubsystemClass) const
	{
		return VoicevoxSubsystemCollection.GetSubsystem<UVoicevoxNativeCoreSubsystem>(SubsystemClass);
	}
	
	template <typename TSubsystemClass>
	TSubsystemClass* GetSubsystem() const
	{
		return VoicevoxSubsystemCollection.GetSubsystem<TSubsystemClass>(TSubsystemClass::StaticClass());
	}
	
	template <typename TSubsystemClass>
	static FORCEINLINE TSubsystemClass* GetSubsystem(const UVoicevoxNativeObject* GameInstance)
	{
		if (GameInstance)
		{
			return GameInstance->GetSubsystem<TSubsystemClass>();
		}
		return nullptr;
	}
	
	template <typename TSubsystemClass>
	const TArray<TSubsystemClass*>& GetSubsystemArray() const
	{
		return VoicevoxSubsystemCollection.GetSubsystemArray<TSubsystemClass>(TSubsystemClass::StaticClass());
	}

	VOICEVOXUECORE_API virtual void Init();

	VOICEVOXUECORE_API virtual void Shutdown();

	static VOICEVOXUECORE_API void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);

	VOICEVOXUECORE_API void CoreInitialize(bool bUseGPU, int CPUNumThreads = 0, bool bLoadAllModels = false);
	
private:
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 0)
	FSubsystemCollection<UVoicevoxNativeCoreSubsystem> VoicevoxSubsystemCollection;
#else
	FObjectSubsystemCollection<UVoicevoxNativeCoreSubsystem> VoicevoxSubsystemCollection;
#endif
};
