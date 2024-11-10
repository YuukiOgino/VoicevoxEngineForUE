// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxUEDefined.h"
#include "Subsystems/Subsystem.h"
#include "VoicevoxNativeCoreSubsystem.generated.h"

/**
 * @class UVoicevoxNativeCoreSubsystem
 * @brief VOICEVOX COREのネイティブライブラリのAPIを実行する基礎Subsystem
 */
UCLASS(Abstract,  MinimalAPI)
class UVoicevoxNativeCoreSubsystem : public USubsystem
{
	GENERATED_BODY()
protected:
	
	//--------------------------------
	// Variable
	//--------------------------------
	
	//! 初期化処理実施済みか
	bool bIsInit = false;

	//--------------------------------
	// Function
	//--------------------------------
	
	/**
	 * @brief VOICEVOXから受信したエラーメッセージを表示
	 * @param[in] MessageFormat : エラーメッセージのフォーマット
	 */
	VOICEVOXUECORE_API void ShowVoicevoxErrorMessage(const FString& MessageFormat) const;
	
public:
	
	VOICEVOXUECORE_API UVoicevoxNativeCoreSubsystem();

	//--------------------------------
	// VOICEVOX CORE Initialize関連
	//--------------------------------
	
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
	virtual bool CoreInitialize(bool bUseGPU, int CPUNumThreads = 0, bool bLoadAllModels = false) { return false; }

	//--------------------------------
	// VOICEVOX CORE Finalize関連
	//--------------------------------
	
	/**
	 * @fn
	 * VOICEVOX CORE 終了処理
	 * @brief 終了処理を行う。以降VOICEVOXのAPIを利用するためには再度Initializeメソッドを行う必要がある。
	 * @detail
	 * VOICEVOXの終了処理は何度も実行可能。
	 * 実行せずにexitしても大抵の場合問題ないが、CUDAを利用している場合は終了処理を実行しておかないと例外が起こることがある。
	 */
	virtual void Finalize() {}

	/**
	 * @fn
	 * VOICEVOX COREのモデルをロード実行
	 * @brief モデルをロードする。
	 * @param SpeakerId 話者番号
	 * @return 成功したらtrue、失敗したらfalse
	 * @detail
	 * 必ずしも話者とモデルが1:1対応しているわけではない。
	 *
	 * ※モデルによってはメインスレッドが暫く止まるほど重いので、その場合は非同期で処理してください。（UE::Tasks::Launch等）
	 */
	virtual bool LoadModel(int64 SpeakerId) { return false; }

	/**
	 * @fn
	 * VOICEVOX COREに該当のスピーカーモデルが存在するか
	 * @brief 使用するCOREにスピーカーモデルが存在するか
	 * @param SpeakerId 話者番号
	 * @return 存在したらtrue、無い場合はfalse
	 */
	virtual bool IsModel(int64 SpeakerId) { return false; }

	/**
	 * @fn
	 * VOICEVOX COREのvoicevox_audio_queryを取得
	 * @brief AudioQuery を取得する。
	 * @param[in] SpeakerId 話者番号
	 * @param[in] Message 音声データに変換するtextデータ
	 * @param[in] bKana aquestalk形式のkanaとしてテキストを解釈する
	 * @return AudioQueryをjsonでフォーマット後、構造体へ変換したもの。
	 * @details
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	virtual FVoicevoxAudioQuery GetAudioQuery(int64 SpeakerId, const FString& Message, bool bKana) { return FVoicevoxAudioQuery(); }
	
	/**
	 * @fn
	 *  VOICEVOX CORE名取得
	 * @brief VOICEVOX COREの名前取得
	 * @return VOICEVOX COREの名前取得
	 */
	virtual FString GetVoicevoxCoreName() { return FString(""); }
	
	/**
	 * @fn
	 * メタ情報を取得する
	 * @brief 話者名や話者IDのリストを取得する
	 * @return メタ情報が格納されたjson形式の構造体
	 */
	virtual TArray<FVoicevoxMeta> GetMetaList() { return TArray<FVoicevoxMeta>(); }

	/**
	 * @brief サポートデバイス情報を取得する
	 * @return サポートデバイス情報の構造体
	 */
	virtual FVoicevoxSupportedDevices GetSupportedDevices() { return FVoicevoxSupportedDevices(); }

	/**
	 * @brief VOICEVOX COREのバージョンを取得する
	 * @return SemVerでフォーマットされたバージョン
	 */
	virtual FString GetVoicevoxVersion() { return FString(""); }

	/**
	 * @brief ハードウェアアクセラレーションがGPUモードか判定する
	 * @return GPUモードならtrue、そうでないならfalse
	 */
	virtual bool IsGpuMode() { return false; }
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxNativeCore, Log, All);