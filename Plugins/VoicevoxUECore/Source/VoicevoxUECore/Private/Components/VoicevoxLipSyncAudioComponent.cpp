// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxLipSyncAudioComponent.cpp
 * @brief  VOICEVOXのAudioQueryを解析して音再生とリップシンクを行うコンポーネントのCPPファイル
 * @author Yuuki Ogino
 */


#include "Components/VoicevoxLipSyncAudioComponent.h"


// Sets default values for this component's properties
UVoicevoxLipSyncAudioComponent::UVoicevoxLipSyncAudioComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	OnAudioPlaybackPercentNative.AddUObject(this, &UVoicevoxLipSyncAudioComponent::HandlePlaybackPercent);
	// ...
}


// Called when the game starts
void UVoicevoxLipSyncAudioComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UVoicevoxLipSyncAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SkeletalMeshComp != nullptr)
	{
		SkeletalMeshComp->SetMorphTarget(FName(TEXT("Face_M_F00_000_00_Fcl_MTH_A")),1.0f);
	}
}

void UVoicevoxLipSyncAudioComponent::HandlePlaybackPercent(const UAudioComponent* InComponent, const USoundWave* InSoundWave, const float InPlaybackPercentage)
{

}