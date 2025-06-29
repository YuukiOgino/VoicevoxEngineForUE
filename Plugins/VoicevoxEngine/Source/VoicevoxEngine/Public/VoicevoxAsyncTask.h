// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxAsyncTask.h
 * @brief  VOICEVOX COREのAPIへ接続するBlueprint公開ノード（要Latentノード）をまとめたヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxUEDefined.h"
#include "VoicevoxQuery.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Tasks/Task.h"
#include "VoicevoxAsyncTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVoicevoxCoreAsyncTaskDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoicevoxCoreAsyncTaskTextToSpeechDelegate, USoundWave*, Sound);

//------------------------------------------------------------------------
// UVoicevoxAsyncTaskBase
//------------------------------------------------------------------------

/**
 * @class UVoicevoxAsyncTaskBase
 * @brief BlueprintでVOICEVOX COREのAPIへアクセスするためのLatentノード基礎クラス
 */
UCLASS()
class VOICEVOXENGINE_API UVoicevoxAsyncTaskBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:

	//! 処理成功時のデリゲート
	UPROPERTY(BlueprintAssignable)
	FVoicevoxCoreAsyncTaskDelegate OnSuccess;

	//! 処理失敗時のデリゲート
	UPROPERTY(BlueprintAssignable)
	FVoicevoxCoreAsyncTaskDelegate OnFail;
	
};

//------------------------------------------------------------------------
// UVoicevoxInitializeAsyncTask
//------------------------------------------------------------------------

/**
 * @class UVoicevoxInitializeAsyncTask
 * @brief BlueprintでVOICEVOX CORE APIのInitializeを実行するLatentノードクラス
 */
UCLASS()
class VOICEVOXENGINE_API UVoicevoxInitializeAsyncTask : public UVoicevoxAsyncTaskBase
{
	GENERATED_BODY()

	//! 実行タスク
	UE::Tasks::TTask<void> Task;
public:
	
	/**
	 * @brief VOICEVOX CORE初期化を実行。VOICEVOXのAPIを正しく実行するには先に初期化が必要(Blueprint公開ノード)
	 * @param[in] WorldContextObject
	 * @param[in] bUseGPU			trueならGPU用、falseならCPU用の初期化を行う
	 * @param[in] CPUNumThreads		推論に用いるスレッド数を設定する。0の場合論理コア数の半分か、物理コア数が設定される
	 * @detail
	 * VOICEVOXの初期化処理は何度も実行可能。bUseGPUを変更して実行しなおすことも可能。
	 * 最後に実行したbUseGPUに従って他の関数が実行される。
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxInitialize", BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"))
	static UVoicevoxInitializeAsyncTask* Initialize(UObject* WorldContextObject, bool bUseGPU, int CPUNumThreads = 0);
	
	//! trueならGPU用、falseならCPU用の初期化を行う
	bool bUseGPU = false;
	//! 推論に用いるスレッド数を設定する。0の場合論理コア数の半分か、物理コア数が設定される
	int CPUNumThreads = 0;

	/**
	 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
	 */
	virtual void Activate() override;

	/**
	 * @brief BeginDestroy
	 */
	virtual void BeginDestroy() override;
};

//------------------------------------------------------------------------
// UVoicevoxLoadModelAsyncTask
//------------------------------------------------------------------------

/**
 * @class UVoicevoxLoadModelAsyncTask
 * @brief BlueprintでVOICEVOX CORE APIのload_modelを実行するLatentノードクラス
 */
UCLASS()
class VOICEVOXENGINE_API UVoicevoxLoadModelAsyncTask : public UVoicevoxAsyncTaskBase
{
	GENERATED_BODY()
public:

	/**
	 * @brief VOICEVOX COREのモデルをロード実行
	 * @param[in] WorldContextObject
	 * @param[in] SpeakerType 話者番号
	 * @detail
	 * 必ずしも話者とモデルが1:1対応しているわけではない。
	 */	
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxLoadModel", BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"))
	static UVoicevoxLoadModelAsyncTask* LoadModel(UObject* WorldContextObject, int SpeakerType);

	//! 話者番号
	int64 SpeakerId = 0;

	/**
	 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
	 */
	virtual void Activate() override;
};

//------------------------------------------------------------------------
// UVoicevoxTextToSpeechAsyncTask
//------------------------------------------------------------------------

/**
 * @class UVoicevoxTextToSpeechAsyncTask
 * @brief BlueprintでVOICEVOX CORE APIのvoicevox_ttsを実行後、再生用のUSwondWaveを生成させるLatentノードクラス
 */
UCLASS()
class VOICEVOXENGINE_API UVoicevoxTextToSpeechAsyncTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	//! 実行タスク
	UE::Tasks::TTask<void> Task;
	
public:

	//! 処理成功時のデリゲート
	UPROPERTY(BlueprintAssignable)
	FVoicevoxCoreAsyncTaskTextToSpeechDelegate OnSuccess;

	//! 処理失敗時のデリゲート
	UPROPERTY(BlueprintAssignable)
	FVoicevoxCoreAsyncTaskDelegate OnFail;
	
	/**
	 * @brief 非同期でVOICEVOX COERで変換した音声データを取得(Blueprint公開ノード)
	 * @param[in] WorldContextObject
	 * @param[in] SpeakerType	話者番号
	 * @param[in] Message		音声データに変換するtextデータ
	 * @param[in] bRunKana		AquesTalkライクな記法で実行するか
	 * @param[in] bEnableInterrogativeUpspeak		疑問文の調整を有効にする
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxTextToSpeechOutputAsync", BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"))
	static UVoicevoxTextToSpeechAsyncTask* TextToSpeech(UObject* WorldContextObject, int SpeakerType, FString Message, bool bRunKana = false, bool bEnableInterrogativeUpspeak = true);

	/**
	 * @brief 非同期で入力したテキストをVOICEVOX COREでAudioQueryに変換後、SoundWaveを生成(Blueprint公開ノード)
	* @param[in] WorldContextObject
	 * @param[in] SpeakerType						話者番号
	 * @param[in] Message							音声データに変換するtextデータ
	 * @param[in] bRunKana							AquesTalkライクな記法で実行するか
	 * @param[in] bEnableInterrogativeUpspeak		疑問文の調整を有効にする
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxToTextAudioQueryOutputAsync", BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"))
	static UVoicevoxTextToSpeechAsyncTask* TextToAudioQuery(UObject* WorldContextObject, int SpeakerType, FString Message, bool bRunKana = false, bool bEnableInterrogativeUpspeak = true);
	
	//! 話者番号
	int64 SpeakerId = 0;
	//! 音声データに変換するtextデータ
	FString Message = "";
	//! AquesTalkライクな記法で実行するか
	bool bRunKana = false;
	//! 疑問文の調整を有効
	bool bEnableInterrogativeUpspeak = true;
	//! AudioQueryに変換するか
	bool bIsUseAudioQuery = false;
	
	/**
	 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
	 */	
	virtual void Activate() override;

	/**
	 * @brief BeginDestroy
	 */
	virtual void BeginDestroy() override;
};

//------------------------------------------------------------------------
// UVoicevoxAudioQueryToSpeechAsyncTask
//------------------------------------------------------------------------

/**
 * @class UVoicevoxAudioQueryToSpeechAsyncTask
 * @brief BlueprintでVOICEVOX CORE APIのvoicevox_synthesisを実行後、再生用のUSwondWaveを生成させるLatentノードクラス
 */
UCLASS()
class VOICEVOXENGINE_API UVoicevoxAudioQueryToSpeechAsyncTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	//! 実行タスク
	UE::Tasks::TTask<void> Task;
	
public:

	//! 処理成功時のデリゲート
	UPROPERTY(BlueprintAssignable)
	FVoicevoxCoreAsyncTaskTextToSpeechDelegate OnSuccess;

	//! 処理失敗時のデリゲート
	UPROPERTY(BlueprintAssignable)
	FVoicevoxCoreAsyncTaskDelegate OnFail;
	
	/**
	 * @brief 非同期でVOICEVOX COREで取得したAudioQueryを元に音声データを取得(Blueprint公開ノード)
	 * @param[in] WorldContextObject
	 * @param[in] SpeakerType	話者番号
	 * @param[in] AudioQuery						AudioQuery構造体
	 * @param[in] bEnableInterrogativeUpspeak		疑問文の調整を有効にする
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxAudioQueryOutputAsync", BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"))
	static UVoicevoxAudioQueryToSpeechAsyncTask* AudioQueryOutput(UObject* WorldContextObject, int SpeakerType, FVoicevoxAudioQuery AudioQuery, bool bEnableInterrogativeUpspeak = true);

	/**
	 * @brief 非同期でVOICEVOX COERで変換した音声データを取得(Blueprint公開ノード)
	 * @param[in] WorldContextObject
	 * @param[in] VoicevoxQuery						Queryアセット
	 * @param[in] bEnableInterrogativeUpspeak		疑問文の調整を有効にする
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxQueryAssetOutputAsync", BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"))
	static UVoicevoxAudioQueryToSpeechAsyncTask* VoicevoxQueryOutput(UObject* WorldContextObject, UVoicevoxQuery* VoicevoxQuery, bool bEnableInterrogativeUpspeak = true);
	
	//! 話者番号
	int64 SpeakerId = 0;
	//! AudioQuery
	FVoicevoxAudioQuery AudioQuery;
	//! 疑問文の調整を有効
	bool bEnableInterrogativeUpspeak = true;
	
	/**
	 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
	 */	
	virtual void Activate() override;

	/**
	 * @brief BeginDestroy
	 */
	virtual void BeginDestroy() override;
};