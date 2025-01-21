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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLipSyncUpdate, ELipSyncVowelType, VowelType, FName, MorphTargetName, float, MorphTargetNum);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnLipSyncUpdateNative, ELipSyncVowelType, FName, float);

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

	//!
	FVoicevoxLipSync NowLipSync;
	
	void HandlePlaybackPercent(const UAudioComponent* InComponent, const USoundWave* InSoundWave, const float InPlaybackPercentage);
	
public:
	// Sets default values for this component's properties
	UVoicevoxLipSyncAudioComponent();
	
protected:

	//! モーフターゲット値のマップ
	TMap<ELipSyncVowelType, float> LipSyncMorphNumMap =
	{
		{ELipSyncVowelType::A, 0.0f},
		{ELipSyncVowelType::I, 0.0f},
		{ELipSyncVowelType::U, 0.0f},
		{ELipSyncVowelType::E, 0.0f},
		{ELipSyncVowelType::O, 0.0f},
	};
	
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	 * @brief AudioQueryからSoundWaveへ変換
	 * @param [in] SpeakerType 
	 * @param [in] bEnableInterrogativeUpspeak 
	 */
	void ToSoundWave(int64 SpeakerType, bool bEnableInterrogativeUpspeak = true);

	/**
	 * @briefテキストから音声変換を実行中かチェック
	 * @return trueの場合はテキストから音声変換のタスク実行中
	 */
	bool CheckExecTts() const;

	/**
	 * @brief モーフターゲット値を初期化
	 */
	void InitMorphNumMap();

	TMap<ELipSyncVowelType, float> UpdateVowelMorphNum(float Alpha);

	TMap<ELipSyncVowelType, float> UpdateConsonantMorphNum(float Alpha);
	
	TMap<ELipSyncVowelType, float> UpdatePauseMorphNum(float Alpha);
public:

	//! リップシンク更新イベント
	UPROPERTY(BlueprintAssignable)
	FOnLipSyncUpdate OnLipSyncUpdate;

	//! リップシンク更新デリゲート（C++）
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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(AllowPrivateAccess=true, ClampMin = "0.1", ClampMax = "2.0", UIMin = "0.1", UIMax = "2.0"))
	float LipSyncSpeed = 1.2f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(AllowPrivateAccess=true, ClampMin = "0.1", ClampMax = "1.0", UIMin = "0.1", UIMax = "1.0"))
	float MaxMouthScale = 1.0f;
	
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