// Fill out your copyright notice in the Description page of Project Settings.


#include "VoicevoxNativeObject.h"

void UVoicevoxNativeObject::Init()
{
	VoicevoxSubsystemCollection.Initialize(this);
}