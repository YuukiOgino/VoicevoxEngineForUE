// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxCoreSubsystem.h
 * @brief  各VOICEVOX COREのAPIを呼び出すEngineSubsystemクラスをまとめたヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxNativeObject.h"
#include "VoicevoxUEDefined.h"
#include "Subsystems/EngineSubsystem.h"
#include "VoicevoxCoreSubsystem.generated.h"

template <typename ...T>
using TVoicevoxCoreDelegate = TDelegate<T...>;

template <typename ...T>
using TVoicevoxCoreMulticastDelegate = TMulticastDelegate<T...>;

using FVoicevoxCoreInitializeDelegate = TVoicevoxCoreDelegate<void(bool, int, bool)>;

using FVoicevoxCoreCompleteDelegate = TVoicevoxCoreDelegate<void(bool)>;

/**
 * @class UVoicevoxCoreSubsystem
 * @brief 各VOICEVOX COREのAPIを呼び出すEngineSubsystemクラス
 */
UCLASS()
class VOICEVOXUECORE_API UVoicevoxCoreSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	//--------------------------------
	// Variable
	//--------------------------------
	
	//! ロードしたVOICEVOX COREの数
	int LoadCoreNum = 0;

	//! 初期化完了したVOICEVOX COREの数
	int InitializeCoreCompleteNum = 0;

	TArray<FVoicevoxMeta> MetaList;

	TMap<FString, FVoicevoxSupportedDevices> SupportedDevicesMap;
	
	TMap<FString, FString> VoicevoxCoreVersionMap;

	UPROPERTY()
	TObjectPtr<UVoicevoxNativeObject> NativeInstance;
	
	//--------------------------------
	// Delegate
	//--------------------------------
	
	//! 初期化処理実行デリゲート
	TVoicevoxCoreMulticastDelegate<void(bool, int, bool)> OnInitialize;

	//! 初期化処理完了通知デリゲート
	TVoicevoxCoreDelegate<void(bool)> OnInitializeComplete;

	//! 終了処理実行デリゲート
	TVoicevoxCoreMulticastDelegate<void()> OnFinalize;

	//! 終了処理完了通知デリゲート
	TVoicevoxCoreDelegate<void(bool)> OnFinalizeComplete;
public:

	//--------------------------------
	// override
	//--------------------------------
	
	/**
	 * @brief Initialize
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @brief Deinitialize
	 */
	virtual void Deinitialize() override;

	void NativeInitialize() const;
	
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
     * @detail
     * VOICEVOXの初期化処理は何度も実行可能。use_gpuを変更して実行しなおすことも可能。
     * 最後に実行したuse_gpuに従って他の関数が実行される。
     * 初期化処理はUE::Tasks::Launchで非同期に行われる
     *
     * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
     */
    void Initialize(bool bUseGPU, int CPUNumThreads = 0, bool bLoadAllModels = false);

	/**
	 * @brief 音声合成するための初期化処理の結果をセット
	 * @param[in] bIsSuccess		初期化が成功したか
	 * @detail
	 * VOICEVOXの初期化処理のリザルトをセットする。NativeCoreプラグインで使用する。
	 * この関数が呼ばれると内部の初期化完了カウンターが増加するため、VOICEVOX COREの初期化処理以外では呼ばないでください。
	 */
	void SetInitializeResult(bool bIsSuccess);

	/**
	 * @brief 全てのVOICEVOX CORE 初期化が完了しているか
	 * @return 全て初期化済みであればtrue、何かしらのCOREの初期化が失敗したらfalse
	 */
	bool GetIsInitialize() const;
	
	/**
	 * @brief VOICEVOX CORE初期化実行のデリゲート関数登録
	 * @param OpenDelegate 初期化処理を実行するデリゲート
	 * @detail
	 * 複数のVOICEVOX COREを初期化処理を実行するデリゲートをSubsystemへ登録する
	 * この関数が呼ばれると初期化すべきCOREの数が増えるため、VoicevoxNaitveCore系プラグインでStartupModule内で使用してください。
	 */
	void SetOnInitializeDelegate(const FVoicevoxCoreInitializeDelegate& OpenDelegate);

	/**
	 * @brief VOICEVOX CORE初期化完了のデリゲート関数登録
	 * @param OpenDelegate 初期化完了を通知するデリゲート
	 * @detail
	 * 全てののVOICEVOX COREを初期化完了を通知するデリゲートをSubsystemへ登録する
	 */
	void SetOnInitializeCompleteDelegate(const FVoicevoxCoreCompleteDelegate& OpenDelegate);

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
	void Finalize() const;

	/**
	 * @brief VOICEVOX CORE終了処理実行のデリゲート関数登録
	 * @param OpenDelegate 終了処理を実行するデリゲート
	 * @detail
	 * 複数のVOICEVOX COREを終了処理を実行するデリゲートをSubsystemへ登録する
	 */
	void SetOnFinalizeDelegate(const TVoicevoxCoreDelegate<void()>& OpenDelegate);

	/**
	 * @brief VOICEVOX CORE終了処理完了のデリゲート関数登録
	 * @param OpenDelegate 終了完了を通知するデリゲート
	 * @detail
	 * 複数のVOICEVOX COREを終了処理完了を通知するデリゲートをSubsystemへ登録する
	 */
	void SetOnFinalizeCompleteDelegate(const FVoicevoxCoreCompleteDelegate& OpenDelegate);

	/**
	 * @brief VOICEVOX CORE終了処理の結果をセット
	 * @param[in] bIsSuccess		終了処理が成功したか
	 * @detail
	 * VOICEVOXの終了処理のリザルトをセットする。NativeCoreプラグインで使用する。
	 * この関数が呼ばれると内部の初期化完了カウンターが減少するため、VOICEVOX COREの初期化処理以外では呼ばないでください。
	 */
	void SetFinalizeResult(bool bIsSuccess);

	//--------------------------------
	// VOICEVOX CORE Meta関連
	//--------------------------------
	
	/**
	 * @fn
	 * メタ情報を取得する
	 * @brief 話者名や話者IDのリストを取得する
	 * @return メタ情報が格納されたjson形式の構造体
	 */
	TArray<FVoicevoxMeta> GetMetaList();

	/**
	 * @fn
	 * 各VOICEVOX COREの設定情報を内部メモリに加える
	 * @brief 各VOICEVOX COREの話者名や話者IDのリスト、サポートデバイス、バージョン情報を各変数へ追加
	 * @param [in]CoreName
	 * @param [in]List
	 * @param [in]SupportedDevices
	 * @param [in]Version
	 */
	void AddVoicevoxConfigData(const FString& CoreName, TArray<FVoicevoxMeta> List, FVoicevoxSupportedDevices SupportedDevices, const FString& Version);
	
};