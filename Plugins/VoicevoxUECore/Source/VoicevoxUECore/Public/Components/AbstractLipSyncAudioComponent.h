// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile AbstractLipSyncAudioComponent.h
 * @brief  VOICEVOXのAudioQueryを解析して音再生とリップシンクを行う抽象コンポーネントのヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxQuery.h"
#include "VoicevoxUEDefined.h"
#include "Components/AudioComponent.h"
#include "AbstractLipSyncAudioComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreateSoundWave);
DECLARE_MULTICAST_DELEGATE(FOnCreateSoundWaveNative);

/**
 * @class UAbstractLipSyncAudioComponent
 * @brief VOICEVOXから生成したデータを元に音再生とリップシンク再生を行う抽象AudioComponentクラス
 */
UCLASS(Abstract, ClassGroup=(Audio, Common), HideCategories=(Object, ActorComponent, Physics, Rendering, Mobility, LOD), ShowCategories=Trigger, meta=(BlueprintSpawnableComponent))
class VOICEVOXUECORE_API UAbstractLipSyncAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

	//! タスク
	UE::Tasks::FTask TtsTask;

	//! リップシンク対象のAudioQuery
	FVoicevoxAudioQuery AudioQuery;
	
	//!　音生成タスク実行中か？
	bool bIsExecTts = false;

	//! モーフターゲット値のマップ
	TMap<ELipSyncVowelType, float> LipSyncMorphNumMap;

	//! リップシンクのデータリスト
	TArray<FVoicevoxLipSync> LipSyncList;
	
	//!　現在実行中のリップシンクデータ
	FVoicevoxLipSync NowLipSync;

	//!　現在実行中のリップシンク実行時間
	float LipSyncTime = 0.0f;

	//! 簡易リップシンク再生をしているか
	bool bIsPlayLipSyncSimple = false;
	
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

protected:
	
	/**
	 * @brief モーフターゲット値の通知実行
	 * @param [in] Map : 「あいうえお」、もしくは簡易リップシンクに関わるモーフターゲット値のマップ
	 */
	virtual void NotificationMorphNum(TMap<ELipSyncVowelType, float> Map){}

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

	//! リップシンクの実行速度（最大値までの時間、0だとリップシンク実行が行われない）
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin = "0.1", ClampMax = "2.0", UIMin = "0.1", UIMax = "2.0"))
	float LipSyncSpeed = 0.75f;

	//! 発音に関わるモーフターゲットの最大値（0だとリップシンク実行が行われない）
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin = "0.1", ClampMax = "1.0", UIMin = "0.1", UIMax = "1.0"))
	float MaxMouthScale = 1.0f;

	//! リップシンクを実行するか。falseの場合は音再生のみ行う
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEnabledLipSync = true;

	//! 簡易的なリップシンクを実行するか。
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEnabledSimpleLipSync = false;
	
	/**
	 * @brief コンストラクタ
	 */
	UAbstractLipSyncAudioComponent();

	/**
	 * @brief オーディオ再生とリップシンク再生を止める
	 * @details サウンド生成中の場合、生成完了するまでスレッドを停止します。ゲームの描画が長時間止まっても問題ないタイミングで実行してください。
	 */
	UFUNCTION(BlueprintCallable, Category="Voicevox|Components")
	void StopAudioAndLipSync();
	
	/**
	 * @brief テキストを解析してSoundWave生成後、音祭再生とリップシンク再生を行います。
	 * @param [in] Message							: 音声データに変換するtextデータ
	 * @param [in] bRunKana							: AquesTalkライクな記法で実行するか
	 * @param [in] bEnableInterrogativeUpspeak		: 疑問文の調整を有効にする
	 * @param [in] SpeedScale						: 話速
	 * @param [in] PitchScale						: 音高
	 * @param [in] IntonationScale					: 抑揚
	 * @param [in] VolumeScale						: 音量
	 * @param [in] PrePhonemeLength					: 開始無音
	 * @param [in] PostPhonemeLength				: 終了無音
	 */
	UFUNCTION(BlueprintCallable, Category="Voicevox|Components")
	void PlayToText(FString Message, bool bRunKana = false, bool bEnableInterrogativeUpspeak = true,
		float SpeedScale = 1.0f, float PitchScale = 0.0f,  float IntonationScale = 1.0f, float VolumeScale = 1.0f, float PrePhonemeLength = 0.1f, float PostPhonemeLength = 0.1f);

	/**
	 * @brief VOICEVOX COREで取得したAudioQueryを元にSoundWaveを生成後、音祭再生とリップシンク再生を行います。
	 * @param [in] Query							: VOICEVOXのAudioQuery
	 * @param [in] bEnableInterrogativeUpspeak 		: 疑問文の調整を有効にする
	 */
	UFUNCTION(BlueprintCallable, Category="Voicevox|Components")
	void PlayToAudioQuery(const FVoicevoxAudioQuery& Query, bool bEnableInterrogativeUpspeak = true);

	/**
	 * @brief AudioQueryアセットからSoundWaveを生成後、音祭再生とリップシンク再生を行います。
	 * @param [in] VoicevoxQuery					: Queryアセット
	 * @param [in] bEnableInterrogativeUpspeak 		: 疑問文の調整を有効にする
	 */
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
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxLipSync, Log, All);