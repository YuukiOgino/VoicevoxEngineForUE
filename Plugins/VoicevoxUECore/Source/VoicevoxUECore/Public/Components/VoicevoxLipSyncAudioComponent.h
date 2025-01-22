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
UCLASS(ClassGroup=(Audio, Common), HideCategories=(Object, ActorComponent, Physics, Rendering, Mobility, LOD), ShowCategories=Trigger, meta=(BlueprintSpawnableComponent))
class VOICEVOXUECORE_API UVoicevoxLipSyncAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

	//! タスク
	UE::Tasks::FTask TtsTask;

	//! 
	float PlayStartTime = 0.0f;

	//!
	FVoicevoxAudioQuery AudioQuery;
	
	//!
	bool bIsExecTts = false;

	//! モーフターゲット値のマップ
	TMap<ELipSyncVowelType, float> LipSyncMorphNumMap;

	//! 
	TArray<FVoicevoxLipSync> LipSyncList;
	
	//!
	FVoicevoxLipSync NowLipSync;

	//!
	float LipSyncTime = 0.0f;
	
	/**
	 * @brief OnAudioPlaybackPercentのコールバック
	 * @param InComponent 
	 * @param InSoundWave 
	 * @param InPlaybackPercentage 
	 */
	void HandlePlaybackPercent(const UAudioComponent* InComponent, const USoundWave* InSoundWave, const float InPlaybackPercentage);

	/**
	 * @brief BeginPlay
	 */
	virtual void BeginPlay() override;
	
	/**
	 * @brief TickComponent
	 * @param DeltaTime 
	 * @param TickType 
	 * @param ThisTickFunction 
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	 * @brief EndPlay 
	 * @param EndPlayReason 
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
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

	/**
	 * @brief 母音のモーフターゲット値リストを更新
	 * @param [in]Alpha : α値 
	 * @return 母音のモーフターゲット値リスト
	 */
	TMap<ELipSyncVowelType, float> UpdateVowelMorphNum(float Alpha);

	/**
	 * @brief 子音のモーフターゲット値リストを更新
	 * @param [in]Alpha : α値 
	 * @return 子音のモーフターゲット値リスト
	 */
	TMap<ELipSyncVowelType, float> UpdateConsonantMorphNum(float Alpha);

	/**
	 * @brief 無音のモーフターゲット値リストを更新
	 * @param [in]Alpha : α値 
	 * @return 無音のモーフターゲット値リスト
	 */	
	TMap<ELipSyncVowelType, float> UpdatePauseMorphNum(float Alpha);

	/**
	 * @brief モーフターゲット値の通知実行
	 * @param Map 
	 */
	void NotificationMorphNum(TMap<ELipSyncVowelType, float> Map);
	
public:

	//! リップシンク更新イベント
	UPROPERTY(BlueprintAssignable)
	FOnLipSyncUpdate OnLipSyncUpdate;

	//! リップシンク更新デリゲート（C++）
	FOnLipSyncUpdateNative OnLipSyncUpdateNative;

	//! リップシンクで使用するモーフターゲット名のマップ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, EditFixedSize)
	TMap<ELipSyncVowelType, FName> LipSyncMorphNameMap;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin = "0.1", ClampMax = "2.0", UIMin = "0.1", UIMax = "2.0"))
	float LipSyncSpeed = 0.75f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin = "0.1", ClampMax = "1.0", UIMin = "0.1", UIMax = "1.0"))
	float MaxMouthScale = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEnabledLipSync = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEnabledSimpleLipSync = false;
	
	/**
	 * @brief コンストラクタ
	 */
	UVoicevoxLipSyncAudioComponent();

	UFUNCTION(BlueprintCallable)
	void PlayToText(int SpeakerType, FString Message, bool bRunKana = false, bool bEnableInterrogativeUpspeak = true);

	UFUNCTION(BlueprintCallable)
	void PlayToAudioQuery(const FVoicevoxAudioQuery& Query, int64 SpeakerType, bool bEnableInterrogativeUpspeak = true);

	UFUNCTION(BlueprintCallable)
	void PlayToAudioQueryAsset(UVoicevoxQuery* VoicevoxQuery, bool bEnableInterrogativeUpspeak = true);
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxLipSync, Log, All);