// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxAsyncTask.h
 * @brief  VOICEVOX COREのAPIへ接続するBlueprint公開ノードをまとめたヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxCoreUtil.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "VoicevoxAsyncTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVoicevoxCoreAsyncTaskDelegate);

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
public:
	
	/**
	 * @brief VOICEVOX COER 初期化を実行。VOICEVOXのAPIを正しく実行するには先に初期化が必要(Blueprint公開ノード)
	 * @param[in] WorldContextObject
	 * @param[in] bUseGPU			trueならGPU用、falseならCPU用の初期化を行う
	 * @param[in] CPUNumThreads		推論に用いるスレッド数を設定する。0の場合論理コア数の半分か、物理コア数が設定される
	 * @detail
	 * VOICEVOXの初期化処理は何度も実行可能。bUseGPUを変更して実行しなおすことも可能。
	 * 最後に実行したbUseGPUに従って他の関数が実行される。
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxInitialize", BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"))
	static UVoicevoxInitializeAsyncTask* Initialize(UObject* WorldContextObject, bool bUseGPU, int CPUNumThreads = 0);

	/**
	 * @fn
	 * VOICEVOX COER 終了処理
	 * @brief 終了処理を行う。以降VOICEVOXのAPIを利用するためには再度Initializeメソッドを行う必要がある。(Blueprint公開ノード)
	 * @detail
	 * VOICEVOXの終了処理は何度も実行可能。
	 * 実行せずにexitしても大抵の場合問題ないが、CUDAを利用している場合は終了処理を実行しておかないと例外が起こることがある。
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxFinalize"))
	static void Finalize();
	
	/**
	 * @fn
	 *  VOICEVOX COER メタ情報を取得する(Blueprint公開ノード)
	 * @brief 話者名や話者IDのリストを取得する(Blueprint公開ノード)
	 * @return メタ情報が格納されたjson形式の文字列
	 */
    UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxMetasToString"))
    static void GetMetasToString(FString& Metas);
	
	/**
	 * @fn
	 *  VOICEVOX COER メタ情報を取得する(Blueprint公開ノード)
	 * @brief 話者名や話者IDのリストを取得する(Blueprint公開ノード)
	 * @return メタ情報が格納されたjson形式の構造体
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxMetasToList"))
	static void GetMetasToList(TArray<FVoicevoxMeta>& Metas);
	
	//! trueならGPU用、falseならCPU用の初期化を行う
	bool bUseGPU = false;
	//! 推論に用いるスレッド数を設定する。0の場合論理コア数の半分か、物理コア数が設定される
	int CPUNumThreads = 0;

	/**
	 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
	 */
	virtual void Activate() override;
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
	static UVoicevoxLoadModelAsyncTask* LoadModel(UObject* WorldContextObject, ESpeakerType SpeakerType);

	//! 話者番号
	int64 SpeakerId = 0;

	/**
	 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
	 */
	virtual void Activate() override;
};

//------------------------------------------------------------------------
// UVoicevoxSimplePlayTextToSpeechAsyncTask
//------------------------------------------------------------------------

/**
 * @class UVoicevoxSimplePlayTextToSpeechAsyncTask
 * @brief BlueprintでVOICEVOX CORE APIのvoicevox_ttsを実行し簡易的に音声再生させるLatentノードクラス
 */
UCLASS()
class VOICEVOXENGINE_API UVoicevoxSimplePlayTextToSpeechAsyncTask : public UVoicevoxAsyncTaskBase
{
	GENERATED_BODY()
public:
	/**
	 * @brief VOICEVOX COERで変換した音声データを簡易的に再生させる(Blueprint公開ノード)
	 * @param[in] WorldContextObject
	 * @param[in] SpeakerType	話者番号
	 * @param[in] Message		音声データに変換するtextデータ
	 * @param[in] bRunKana		AquesTalkライクな記法で実行するか
	 * @detail
	 * 音声再生はネイティブ機能を使用しているため、UE独自のサウンドシステムは不使用。
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxSimplePlayTextToSpeech", BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"))
	static UVoicevoxSimplePlayTextToSpeechAsyncTask* SimplePlayTextToSpeech(UObject* WorldContextObject, ESpeakerType SpeakerType, FString Message, bool bRunKana=false);
	
	//! 話者番号
	int64 SpeakerId = 0;
	//! 音声データに変換するtextデータ
	FString Message = "";
	//! AquesTalkライクな記法で実行するか
	bool bRunKana = false;

	/**
	 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
	 */	
	virtual void Activate() override;
};

//------------------------------------------------------------------------
// UVoicevoxSimplePlayTextToSpeechAsyncTask
//------------------------------------------------------------------------

/**
 * @class UVoicevoxSimplePlayTextToAudioQueryAsyncTask
 * @brief BlueprintでVOICEVOX CORE APIのvoicevox_audio_queryを実行し簡易的に音声再生させるLatentノードクラス
 */
UCLASS()
class VOICEVOXENGINE_API UVoicevoxSimplePlayTextToAudioQueryAsyncTask : public UVoicevoxAsyncTaskBase
{
	GENERATED_BODY()
public:
	/**
	 * @brief VOICEVOX COERで変換したAudioQuery簡易的に再生させる(Blueprint公開ノード)
	 * @param[in] WorldContextObject
	 * @param[in] SpeakerType	話者番号
	 * @param[in] Message		音声データに変換するtextデータ
	 * @param[in] bRunKana		AquesTalkライクな記法で実行するか
	 * @detail
	 * 音声再生はネイティブ機能を使用しているため、UE独自のサウンドシステムは不使用。
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxSimplePlayTextToAudioQuery", BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"))
	static UVoicevoxSimplePlayTextToAudioQueryAsyncTask* SimplePlayTextToAudioQuery(UObject* WorldContextObject, ESpeakerType SpeakerType, FString Message, bool bRunKana=false);

	//! 話者番号
	int64 SpeakerId = 0;
	//! 音声データに変換するtextデータ
	FString Message = "";
	//! AquesTalkライクな記法で実行するか
	bool bRunKana = false;

	/**
	 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
	 */	
	virtual void Activate() override;
	
	/**
	 * @brief VOICEVOX COERで変換したAudioQueryを取得する(Blueprint公開ノード)
	 * @param[out] Query	AudioQueryデータ
	 * @param[in] SpeakerType	話者番号
	 * @param[in] Message		音声データに変換するtextデータ
	 * @param[in] bRunKana		AquesTalkライクな記法で実行するか
	 * @detail
	 * 返却された文字列はJSON形式
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxAudioQuery"))
	static void GetAudioQuery(FString& Query, ESpeakerType SpeakerType, FString Message, bool bRunKana=false);
	
};