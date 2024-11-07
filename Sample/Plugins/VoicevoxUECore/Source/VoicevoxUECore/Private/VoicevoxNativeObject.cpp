// Fill out your copyright notice in the Description page of Project Settings.


#include "VoicevoxNativeObject.h"

void UVoicevoxNativeObject::Init()
{
	if (!VoicevoxSubsystemCollection.IsInitialized())
	{
		VoicevoxSubsystemCollection.Initialize(this);
	}
}

void UVoicevoxNativeObject::Shutdown()
{
	VoicevoxSubsystemCollection.Deinitialize();
}

void UVoicevoxNativeObject::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
	UVoicevoxNativeObject* This = CastChecked<UVoicevoxNativeObject>(InThis);
	This->VoicevoxSubsystemCollection.AddReferencedObjects(This, Collector);
	UObject::AddReferencedObjects(This, Collector);
}

void UVoicevoxNativeObject::CoreInitialize(bool bUseGPU, int CPUNumThreads, bool bLoadAllModels)
{
	
}