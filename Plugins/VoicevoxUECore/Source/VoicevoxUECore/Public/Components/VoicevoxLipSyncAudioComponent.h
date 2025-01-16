// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxLipSyncAudioComponent.h
 * @brief  VOICEVOXのAudioQueryを解析して音再生とリップシンクを行うコンポーネントのヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxQuery.h"
#include "VoicevoxUEDefined.h"
#include "Components/AudioComponent.h"
#include "Tasks/Task.h"
#include "VoicevoxLipSyncAudioComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLipSyncUpdate, FVoicevoxLipSync, VoicevoxLipSync, FName, MorphTargetName);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLipSyncUpdateNative, FVoicevoxLipSync, FName);

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

	/**
	 * @brief AudioQueryからSoundWaveへ変換
	 * @param [in] SpeakerType 
	 * @param [in] bEnableInterrogativeUpspeak 
	 */
	void ToSoundWave(int64 SpeakerType, bool bEnableInterrogativeUpspeak = true);

	/**
	 * テキストから音声変換を実行中かチェック
	 * @return trueの場合はテキストから音声変換のタスク実行中
	 */
	bool CheckExecTts() const;

	void InitMorphNumMap();
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnLipSyncUpdate OnLipSyncUpdate;
	
	FOnLipSyncUpdateNative OnLipSyncUpdateNative;

	//! リップシンクで使用するモーフターゲット名のマップ
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TMap<ELipSyncVowelType, FName> LipSyncMorphNameMap =
		{
			{ELipSyncVowelType::A, NAME_None},
			{ELipSyncVowelType::I, NAME_None},
			{ELipSyncVowelType::U, NAME_None},
			{ELipSyncVowelType::E, NAME_None},
			{ELipSyncVowelType::O, NAME_None},
		};
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	TMap<ELipSyncVowelType, float> LipSyncMorphNumMap =
		{
			{ELipSyncVowelType::A, 0.0f},
			{ELipSyncVowelType::I, 0.0f},
			{ELipSyncVowelType::U, 0.0f},
			{ELipSyncVowelType::E, 0.0f},
			{ELipSyncVowelType::O, 0.0f},
		};

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PlayToText(int SpeakerType, FString Message, bool bRunKana = false, bool bEnableInterrogativeUpspeak = true, float StartTime = 0.0f);

	UFUNCTION(BlueprintCallable)
	void PlayToAudioQuery(const FVoicevoxAudioQuery& Query, int64 SpeakerType, bool bEnableInterrogativeUpspeak = true, float StartTime = 0.0f);

	UFUNCTION(BlueprintCallable)
	void PlayToAudioQueryAsset(UVoicevoxQuery* VoicevoxQuery, bool bEnableInterrogativeUpspeak = true, float StartTime = 0.0f);
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxLipSync, Log, All);