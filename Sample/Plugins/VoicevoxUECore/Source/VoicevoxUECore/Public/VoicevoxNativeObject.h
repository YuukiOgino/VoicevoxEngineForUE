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
public:
	/**
	 * Get a Subsystem of specified type
	 */
	UVoicevoxNativeCoreSubsystem* GetSubsystemBase(TSubclassOf<UVoicevoxNativeCoreSubsystem> SubsystemClass) const
	{
		return VoicevoxSubsystemCollection.GetSubsystem<UVoicevoxNativeCoreSubsystem>(SubsystemClass);
	}

	/**
	 * Get a Subsystem of specified type
	 */
	template <typename TSubsystemClass>
	TSubsystemClass* GetSubsystem() const
	{
		return VoicevoxSubsystemCollection.GetSubsystem<TSubsystemClass>(TSubsystemClass::StaticClass());
	}

	/**
	 * Get a Subsystem of specified type from the provided GameInstance
	 * returns nullptr if the Subsystem cannot be found or the GameInstance is null
	 */
	template <typename TSubsystemClass>
	static FORCEINLINE TSubsystemClass* GetSubsystem(const UVoicevoxNativeObject* GameInstance)
	{
		if (GameInstance)
		{
			return GameInstance->GetSubsystem<TSubsystemClass>();
		}
		return nullptr;
	}

	/**
	 * Get all Subsystem of specified type, this is only necessary for interfaces that can have multiple implementations instanced at a time.
	 *
	 * Do not hold onto this Array reference unless you are sure the lifetime is less than that of UGameInstance
	 */
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
	FObjectSubsystemCollection<UVoicevoxNativeCoreSubsystem> VoicevoxSubsystemCollection;
};
