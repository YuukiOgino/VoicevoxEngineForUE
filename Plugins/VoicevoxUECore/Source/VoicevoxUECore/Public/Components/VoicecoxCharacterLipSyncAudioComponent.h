// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicecoxCharacterLipSyncAudioComponent.h
 * @brief  VOICEVOXのAudioQueryを解析して音再生とスケルタルメッシュコンポーネントにリップシンクを行うコンポーネントのヘッダーファイル
 * @author Yuuki Ogino
 */


#pragma once

#include "CoreMinimal.h"
#include "VoicevoxLipSyncAudioComponent.h"
#include "VoicevoxQuery.h"
#include "VoicevoxUEDefined.h"
#include "Components/AudioComponent.h"
#include "Tasks/Task.h"
#include "VoicecoxCharacterLipSyncAudioComponent.generated.h"

/**
 * @class UVoicecoxCharacterLipSyncAudioComponent
 * @brief VOICEVOXのAudioQueryを解析して、音再生とスケルタルメッシュコンポーネントにリップシンクを行うコンポーネントクラス
 */
UCLASS(ClassGroup=(Audio, Common), HideCategories=(Object, ActorComponent, Physics, Rendering, Mobility, LOD), ShowCategories=Trigger, meta=(BlueprintSpawnableComponent))
class VOICEVOXUECORE_API UVoicecoxCharacterLipSyncAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

	//! リップシンク対象のスケルタルメッシュコンポーネント
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	
	//!
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

	//! リップシンク実行しているのは簡易リップシンク再生をしているか
	bool bIsPlayLipSyncSimple = false;
	
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
	 * @param [in]EndPlayReason 
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

	void UpdateSkeletalMeshMorphTargetNum(TMap<ELipSyncVowelType, float> Map);

public:

	//! サウンド生成完了イベント
	UPROPERTY(BlueprintAssignable)
	FOnCreateSoundWave OnCreateSoundWave;

	//! サウンド再生完了デリゲート(C++)
	FOnCreateSoundWaveNative OnCreateSoundWaveNative;
	
	//! リップシンクで使用するモーフターゲット名のマップ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, EditFixedSize)
	TMap<ELipSyncVowelType, FName> LipSyncMorphNameMap;

	//! 再生するスピーカーID
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int64 SpeakerId = 3;
	
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
	UVoicecoxCharacterLipSyncAudioComponent();

	/**
	 * @brief オーディオ再生とリップシンク再生を止める
	 * @details サウンド生成中の場合、生成完了するまでスレッドを停止します。ゲームの描画が長時間止まっても問題ないタイミングで実行してください。
	 */
	UFUNCTION(BlueprintCallable, Category="Voicevox|Components")
	void StopAudioAndLipSync();
	
	UFUNCTION(BlueprintCallable, Category="Voicevox|Components")
	void PlayToText(FString Message, bool bRunKana = false, bool bEnableInterrogativeUpspeak = true,
		float SpeedScale = 1.0f, float PitchScale = 0.0f,  float IntonationScale = 1.0f, float VolumeScale = 1.0f, float PrePhonemeLength = 0.1f, float PostPhonemeLength = 0.1f);

	UFUNCTION(BlueprintCallable, Category="Voicevox|Components")
	void PlayToAudioQuery(const FVoicevoxAudioQuery& Query, bool bEnableInterrogativeUpspeak = true);

	UFUNCTION(BlueprintCallable, Category="Voicevox|Components")
	void PlayToAudioQueryAsset(UVoicevoxQuery* VoicevoxQuery, bool bEnableInterrogativeUpspeak = true);

	/**
	 * @brief リップシンクデータをAudioQueryからセットする
	 * @details 予め生成したVOICCEVOXの音声アセットをSetSoundでセット後、リップシンクを行う場合はAudioQueryを渡してリップシンクデータを生成してください。<br/>
	*			サウンド再生中はバグを防ぐため更新は行いません。サウンドをStopした状態で呼び出してください。
	 * @param [in]Query : リップシンクデータをセットするAudioQuery 
	 */
	UFUNCTION(BlueprintCallable, Category="Voicevox|Components")
	void SetLipSyncDataToAudioQuery(const FVoicevoxAudioQuery& Query);

	/**
	 * @brief リップシンクデータをAudioQueryアセットからセットする
	 * @details 予め生成したVOICCEVOXの音声アセットをSetSoundでセット後、リップシンクを行う場合はAudioQueryアセットを渡してリップシンクデータを生成してください。<br/>
	*			サウンド再生中はバグを防ぐため更新は行いません。サウンドをStopした状態で呼び出してください。
	 * @param [in]VoicevoxQuery : リップシンクデータをセットするAudioQueryアセット
	 */
	UFUNCTION(BlueprintCallable, Category="Voicevox|Components")
	void SetLipSyncDataToAudioQueryAsset(UVoicevoxQuery* VoicevoxQuery);
	
	/**
	 * @brief リップシンク対象のスケルタルメッシュをセット
	 * @param [in]SkeletalMesh :  リップシンク対象のスケルタルメッシュ
	 */
	UFUNCTION(BlueprintCallable)
	void SetSkeletalMesh(USkeletalMeshComponent* SkeletalMesh);
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxCharacterLipSync, Log, All);