// Fill out your copyright notice in the Description page of Project Settings.


#include "VoicevoxNativeObject.h"

#include "Subsystems/VoicevoxNativeCoreSubsystem.h"

void UVoicevoxNativeObject::Init()
{
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 0)
	VoicevoxSubsystemCollection.Initialize(this);
#else
	if (!VoicevoxSubsystemCollection.IsInitialized())
	{
		VoicevoxSubsystemCollection.Initialize(this);
	}
#endif
	UClass* BaseType = UVoicevoxNativeCoreSubsystem::StaticClass();
	GetDerivedClasses(BaseType, SubsystemClasses, true);
}

void UVoicevoxNativeObject::Shutdown()
{
	VoicevoxSubsystemCollection.Deinitialize();
	SubsystemClasses.Empty();
}

void UVoicevoxNativeObject::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
	UVoicevoxNativeObject* This = CastChecked<UVoicevoxNativeObject>(InThis);
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 0)
	This->VoicevoxSubsystemCollection.AddReferencedObjects(Collector);
#else
	This->VoicevoxSubsystemCollection.AddReferencedObjects(This, Collector);
#endif
	UObject::AddReferencedObjects(This, Collector);
}

void UVoicevoxNativeObject::CoreInitialize(bool bUseGPU, int CPUNumThreads, bool bLoadAllModels)
{
	
	for (auto Element : SubsystemClasses)
	{
		/*auto subsystem = VoicevoxSubsystemCollection.GetSubsystem(Element);
		static_cast<UVoicevoxNativeCoreSubsystem*>(subsystem)->CoreInitialize(bUseGPU, CPUNumThreads, bLoadAllModels);*/
	}
}