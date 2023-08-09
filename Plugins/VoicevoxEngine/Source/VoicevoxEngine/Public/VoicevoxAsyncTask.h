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