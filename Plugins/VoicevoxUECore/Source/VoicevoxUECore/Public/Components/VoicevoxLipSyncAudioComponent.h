// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxLipSyncAudioComponent.h
 * @brief  VOICEVOXのAudioQueryを解析して音再生とリップシンクを行うコンポーネントのヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxUEDefined.h"
#include "Components/AudioComponent.h"
#include "Tasks/Task.h"
#include "VoicevoxLipSyncAudioComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLipSyncUpdate, FVoicevoxLipSync, VoicevoxLipSync);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLipSyncUpdateNative, FVoicevoxLipSync);

/**
 * @class UVoicevoxLipSyncAudioComponent
 * @brief VOICEVOXのAudioQueryを解析して音再生とリップシンクを行うコンポーネントクラス
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOICEVOXUECORE_API UVoicevoxLipSyncAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

	//!
	UE::Tasks::FTask TtsTask;

	//!
	float PlayStartTime = 0.0f;

	//!
	FVoicevoxAudioQuery AudioQuery;

	//! 
	TArray<FVoicevoxLipSync> LipSyncList;

	//!
	float LipSyncTime = 0.0f;
	
	//!
	bool bIsExecTts = false;
	
	void HandlePlaybackPercent(const UAudioComponent* InComponent, const USoundWave* InSoundWave, const float InPlaybackPercentage);
	
public:
	// Sets default values for this component's properties
	UVoicevoxLipSyncAudioComponent();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ToSoundWave(int64 SpeakerType, bool bEnableInterrogativeUpspeak = true);
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnLipSyncUpdate OnLipSyncUpdate;
	
	FOnLipSyncUpdateNative OnLipSyncUpdateNative;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PlayToText(int SpeakerType, FString Message, bool bRunKana = false, bool bEnableInterrogativeUpspeak = true, float StartTime = 0.0f);

	UFUNCTION(BlueprintCallable)
	void PlayToAudioQuery(const FVoicevoxAudioQuery& Query, int64 SpeakerType, bool bEnableInterrogativeUpspeak = true, float StartTime = 0.0f);
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxLipSync, Log, All);