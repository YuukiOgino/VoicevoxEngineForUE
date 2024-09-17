// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxCoreSubsystem.h
 * @brief  各VOICEVOX COREのAPIを呼び出すEngineSubsystemクラスをまとめたヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "VoicevoxCoreSubsystem.generated.h"

template <typename ...T>
using TVoicevoxCoreDelegate = TDelegate<T...>;

template <typename ...T>
using TVoicevoxCoreMulticastDelegate = TMulticastDelegate<T...>;

/**
 * @class UVoicevoxCoreSubsystem
 * @brief 各VOICEVOX COREのAPIを呼び出すEngineSubsystemクラス
 */
UCLASS()
class VOICEVOXUECORE_API UVoicevoxCoreSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	
public:

	TVoicevoxCoreMulticastDelegate<void(bool, int, bool)> OnInitialize;
	
    /**
     * @fn
     * VOICEVOX CORE 初期化
     * @brief 音声合成するための初期化を行う。VOICEVOXのAPIを正しく実行するには先に初期化が必要
     * @param[in] bUseGPU			trueならGPU用、falseならCPU用の初期化を行う
     * @param[in] CPUNumThreads		推論に用いるスレッド数を設定する。0の場合論理コア数の半分か、物理コア数が設定される
     * @param[in] bLoadAllModels	trueなら全てのモデルをロードする(かなり時間がかかるのでtrueは非推奨です。trueはデバッグ用として使用してください)
     * @return 成功したらtrue、失敗したらfalse
     * @detail
     * VOICEVOXの初期化処理は何度も実行可能。use_gpuを変更して実行しなおすことも可能。
     * 最後に実行したuse_gpuに従って他の関数が実行される。
     * 初期化処理はUE::Tasks::Launchで非同期に行われる
     *
     * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
     */
    bool Initialize(bool bUseGPU, int CPUNumThreads = 0, bool bLoadAllModels = false);

	void SetOnInitializeDelegate(const TVoicevoxCoreMulticastDelegate<void(bool, int, bool)>& OpenDelegate);
};
