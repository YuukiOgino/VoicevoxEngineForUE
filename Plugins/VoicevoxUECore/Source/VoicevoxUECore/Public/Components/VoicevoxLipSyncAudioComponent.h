// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxLipSyncAudioComponent.h
 * @brief  VOICEVOXのAudioQueryを解析して音再生とリップシンクを行うコンポーネントのヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "VoicevoxLipSyncAudioComponent.generated.h"


/**
 * @class UVoicevoxLipSyncAudioComponent
 * @brief VOICEVOXのAudioQueryを解析して音再生とリップシンクを行うコンポーネントクラス
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOICEVOXUECORE_API UVoicevoxLipSyncAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

	void HandlePlaybackPercent(const UAudioComponent* InComponent, const USoundWave* InSoundWave, const float InPlaybackPercentage);
	
public:
	// Sets default values for this component's properties
	UVoicevoxLipSyncAudioComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
