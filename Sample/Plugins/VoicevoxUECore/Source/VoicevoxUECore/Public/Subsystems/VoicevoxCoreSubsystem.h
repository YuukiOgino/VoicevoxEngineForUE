// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxCoreSubsystem.h
 * @brief  各VOICEVOX COREのAPIを呼び出すSubsystemヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxNativeObject.h"
#include "VoicevoxUEDefined.h"
#include "VoicevoxQuery.h"
#include "Subsystems/EngineSubsystem.h"
#include "VoicevoxCoreSubsystem.generated.h"

//----------------------------------------------------------------
// using(Delegate)
//----------------------------------------------------------------

/**
 * @typedef TVoicevoxCoreDelegate
 */
template <typename ...T>
using TVoicevoxCoreDelegate = TDelegate<T...>;

/**
 * @typedef TVoicevoxCoreMulticastDelegate
 */
template <typename ...T>
using TVoicevoxCoreMulticastDelegate = TMulticastDelegate<T...>;

/**
 * @typedef FVoicevoxCoreCompleteDelegate
 */
using FVoicevoxCoreCompleteDelegate = TVoicevoxCoreDelegate<void(bool)>;

//----------------------------------------------------------------
// class
//----------------------------------------------------------------

class UVoicevoxNativeObject;

/**
 * @class UVoicevoxCoreSubsystem
 * @brief 各VOICEVOX COREのAPIを呼び出すEngineSubsystemクラス
 */
UCLASS()
class VOICEVOXUECORE_API UVoicevoxCoreSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	//----------------------------------------------------------------
	// friend class
	//----------------------------------------------------------------
	
	friend class UVoicevoxNativeObject;
	
	//----------------------------------------------------------------
	// Variable
	//----------------------------------------------------------------

	//! VOICEVOX COREライブラリ参照プラグイン名リスト
	UPROPERTY()
	TArray<FString> CoreNameList;

	//! 話者名や話者IDのリスト
	UPROPERTY()
	TArray<FVoicevoxMeta> MetaList;

	//! サポートデバイス情報リスト
	UPROPERTY()
	TMap<FString, FVoicevoxSupportedDevices> SupportedDevicesMap;

	//! VOICEVOX COREライブラリバージョンリスト
	UPROPERTY()
	TMap<FString, FString> VoicevoxCoreVersionMap;

	//! GPUモードリスト
	UPROPERTY()
	TMap<FString, bool> IsGpuModeMap;

	//! VoicevoxNativeCoreSubsystem管理インスタンス 
	UPROPERTY()
	TObjectPtr<UVoicevoxNativeObject> NativeInstance;

	//! 初期化済みフラグ
	UPROPERTY()
	bool bIsInitialized = false;
	
	//----------------------------------------------------------------
	// Delegate
	//----------------------------------------------------------------

	//! 初期化処理完了通知デリゲート
	TVoicevoxCoreDelegate<void(bool)> OnInitializeComplete;

	//! 終了処理完了通知デリゲート
	TVoicevoxCoreDelegate<void(bool)> OnFinalizeComplete;

	//! モデル読み込み処理完了通知デリゲート
	TVoicevoxCoreDelegate<void(bool)> OnLodeModelComplete;

	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------

	//--------------------------------
	// CORE API処理完了通知関数
	// VoicevoxNativeObjectから呼び出し
	//--------------------------------
	
	/**
	 * @brief 音声合成するための初期化処理の結果をセット
	 * @param[in] bIsSuccess		初期化が成功したか
	 * @detail
	 * VOICEVOXの初期化処理のリザルトをセットする。NativeCoreプラグインで使用する。
	 */
	void SetInitializeResult(bool bIsSuccess);

	/**
	 * @brief VOICEVOX CORE終了処理の結果をセット
	 * @param[in] bIsSuccess		終了処理が成功したか
	 * @detail
	 * VOICEVOXの終了処理のリザルトをセットする。NativeCoreプラグインで使用する。
	 */
	void SetFinalizeResult(bool bIsSuccess);

	/**
	 * @brief VOICEVOX COREモデル読み込みの結果をセット
	 * @param[in] bIsSuccess		モデル読み込み処理が成功したか
	 * @detail
	 * VOICEVOXのモデル読み込み処理のリザルトをセットする。NativeCoreプラグインで使用する。
	 */
	void SetLodeModelResult(bool bIsSuccess) const;
	
	/**
	 * @fn
	 * 各VOICEVOX COREの設定情報を内部メモリに加える
	 * @brief 各VOICEVOX COREの話者名や話者IDのリスト、サポートデバイス、バージョン情報、GPUモード状態を各変数へ追加
	 * @param [in] CoreName : プラグインに定義したコア名
	 * @param [in] List : 
	 * @param [in] SupportedDevices
	 * @param [in] Version
	 * @param [in] bIsGpuMode
	 */
	void AddVoicevoxConfigData(const FString& CoreName, TArray<FVoicevoxMeta> List, FVoicevoxSupportedDevices SupportedDevices, const FString& Version, const bool bIsGpuMode);
	
public:

	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------
	
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

	//--------------------------------
	// Subsystem管理インスタンス初期化
	//--------------------------------

	/**
	 * @brief VoicevoxNativeCoreSubsystem管理インスタンスの初期化処理実行
	 */
	void NativeInitialize() const;

	//----------------------------------------------------------------
	// VOICEVOX CORE APIアクセス関数
	//----------------------------------------------------------------
	
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
	 * @brief 全てのVOICEVOX CORE 初期化が完了しているか
	 * @return 全て初期化済みであればtrue、何かしらのCOREの初期化が失敗したらfalse
	 */
	bool GetIsInitialize() const;

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
	 * @brief VOICEVOX CORE終了処理完了のデリゲート関数登録
	 * @param OpenDelegate 終了完了を通知するデリゲート
	 * @detail
	 * 複数のVOICEVOX COREを終了処理完了を通知するデリゲートをSubsystemへ登録する
	 */
	void SetOnFinalizeCompleteDelegate(const FVoicevoxCoreCompleteDelegate& OpenDelegate);

	//--------------------------------
	// VOICEVOX CORE LoadModel関連
	//--------------------------------

	/**
	 * @fn
	 * VOICEVOX COREのモデルをロード実行
	 * @brief モデルをロードする。
	 * @param SpeakerId 話者番号
	 * @detail
	 * 必ずしも話者とモデルが1:1対応しているわけではない。
	 *
	 * ※モデルによってはメインスレッドが暫く止まるほど重いので、その場合は非同期で処理してください。（UE::Tasks::Launch等）
	 */
	 void LoadModel(int64 SpeakerId) const;

	/**
	 * @brief VOICEVOX COREモデルロード処理完了のデリゲート関数登録
	 * @param OpenDelegate モデルロード完了を通知するデリゲート
	 * @detail
	 * VOICEVOX COREのモデルロード処理完了を通知するデリゲートをSubsystemへ登録する
	 */
	void SetOnLoadModelCompleteDelegate(const FVoicevoxCoreCompleteDelegate& OpenDelegate);

	//--------------------------------
	// VOICEVOX CORE AudioQuery関連
	//--------------------------------
	
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
	FVoicevoxAudioQuery GetAudioQuery(int64 SpeakerId, const FString& Message, bool bKana) const;

	//--------------------------------
	// VOICEVOX CORE TextToSpeech関連
	//--------------------------------
	
	/**
	 * @fn
	 * VOICEVOX COREのtext to speechを実行
	 * @brief Textデータを音声データに変換する。
	 * @param[in] SpeakerId 話者番号
	 * @param[in] Message 音声データに変換するtextデータ
	 * @param[in] bKana aquestalk形式のkanaとしてテキストを解釈する
	 * @param[in] bEnableInterrogativeUpspeak 疑問文の調整を有効にする
	 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
	 * @details
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	TArray<uint8> RunTextToSpeech(int64 SpeakerId, const FString& Message, bool bKana, bool bEnableInterrogativeUpspeak) const;
	
	//--------------------------------
	// VOICEVOX CORE Synthesis関連
	//--------------------------------
	
	/**
	 * @fn
	 * VOICEVOX COREのvoicevox_synthesisを実行
	 * @brief AudioQueryを音声データに変換する。
	 * @param[in] AudioQueryJson jsonフォーマットされた AudioQuery
	 * @param[in] SpeakerId 話者番号
	 * @param[in] bEnableInterrogativeUpspeak 疑問文の調整を有効にする
	 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
	 * @details
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	TArray<uint8> RunSynthesis(const char* AudioQueryJson, int64 SpeakerId, bool bEnableInterrogativeUpspeak) const;

	/**
	 * @fn
	 * VOICEVOX COREのvoicevox_synthesisを実行
	 * @brief AudioQueryを音声データに変換する。
	 * @param[in] AudioQuery jsonフォーマットされた AudioQuery構造体
	 * @param[in] SpeakerId 話者番号
	 * @param[in] bEnableInterrogativeUpspeak 疑問文の調整を有効にする
	 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
	 * @details
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	TArray<uint8> RunSynthesis(const FVoicevoxAudioQuery& AudioQuery, int64 SpeakerId, bool bEnableInterrogativeUpspeak) const;

	/**
	 * @fn
	 * VOICEVOX COREのvoicevox_synthesisを実行
	 * @brief AudioQueryアセットデータを音声データに変換する。
	 * @param[in] VoicevoxQuery AudioQueryアセットデータ
	 * @param[in] bEnableInterrogativeUpspeak 疑問文の調整を有効にする
	 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
	 * @details
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	TArray<uint8> RunSynthesis(const UVoicevoxQuery& VoicevoxQuery, bool bEnableInterrogativeUpspeak) const;

	//--------------------------------
	// VOICEVOX CORE LipSync関連
	//--------------------------------
	
	/**
	 * @brief VOICEVOX COREで取得したAudioQuery元に、中品質なLipSyncに必要なデータリストを取得(Blueprint公開ノード)
	 * @param[in] AudioQuery AudioQuery構造体
	 * @return AudioQuery情報を元に生成した、中品質のLipSyncに必要なデータリスト
	 */
	static TArray<FVoicevoxLipSync> GetLipSyncList(FVoicevoxAudioQuery AudioQuery);
	
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
	 * メタ情報から指定したSpeakerIDの名前を取得する
	 * @brief 指定したSpeakerIDの名前を取得する
	 * @return 指定したSpeakerIDの名前
	 */
	FString GetMetaName(int64 SpeakerID);
	
	//--------------------------------
	// VOICEVOX CORE Version関連
	//--------------------------------
	
	/**
	 * @brief VOICEVOX COREのバージョンを取得する
	 * @return SemVerでフォーマットされたバージョン
	 */
	 FString GetVoicevoxVersion(const FString& CoreName);

	//--------------------------------
	// VOICEVOX CORE GpuMode関連
	//--------------------------------
	
	/**
	 * @brief ハードウェアアクセラレーションがGPUモードか判定する
	 * @return GPUモードならtrue、そうでないならfalse
	 */
	bool IsGpuMode(const FString& CoreName);

	//--------------------------------
	// VOICEVOX CORE SupportedDevices関連
	//--------------------------------
	
	/**
	 * @brief サポートデバイス情報を取得する
	 * @return サポートデバイス情報の構造体
	 */
	FVoicevoxSupportedDevices GetSupportedDevices(const FString& CoreName);

	//--------------------------------
	// VOICEVOX CORE プラグイン名取得
	//--------------------------------
	
	/**
	 * @brief 初期化済みのネイティブコア名を取得
	 * @return 初期化済みのネイティブコア名のリスト
	 */
	TArray<FString> GetCoreNameList();

	//--------------------------------
	// VOICEVOX CORE PhonemeLength関連
	//--------------------------------
	
	/**
	 * @fn
	 * 音素ごとの長さを求める
	 * @brief 音素列から、音素ごとの長さを求める
	 * @param[in] Length 音素列の長さ
	 * @param[in] PhonemeList 音素列
	 * @param[in] SpeakerID 話者番号
	 * @return 音素ごとの長さ
	 *
	 * @warning 動作確認が取れていないため、クラッシュ、もしくは予期せぬ動作をする可能性が高いです。
	 */
	TArray<float> GetPhonemeLength(int64 Length, TArray<int64> PhonemeList, int64 SpeakerID) const;

	//--------------------------------
	// VOICEVOX CORE Mora関連
	//--------------------------------
	
	/**
	 * @fn
	 * モーラごとの音高を求める
	 * @brief モーラごとの音素列とアクセント情報から、モーラごとの音高を求める
	 * @param[in] Length モーラ列の長さ
	 * @param[in] VowelPhonemeList 母音の音素列
	 * @param[in] ConsonantPhonemeList 子音の音素列
	 * @param[in] StartAccentList アクセントの開始位置
	 * @param[in] EndAccentList アクセントの終了位置
	 * @param[in] StartAccentPhraseList アクセント句の開始位置
	 * @param[in] EndAccentPhraseList アクセント句の終了位置
	 * @param[in] SpeakerID 話者番号
	 * @return モーラごとの音高
	 *
	 * @warning 動作確認が取れていないため、クラッシュ、もしくは予期せぬ動作をする可能性が高いです。
	 */
	TArray<float> FindPitchEachMora(int64 Length, TArray<int64> VowelPhonemeList, TArray<int64> ConsonantPhonemeList,
											  TArray<int64> StartAccentList, TArray<int64> EndAccentList,
											  TArray<int64> StartAccentPhraseList, TArray<int64> EndAccentPhraseList,
											  int64 SpeakerID) const;

	//--------------------------------
	// VOICEVOX CORE DecodeForward関連
	//--------------------------------
	
	/**
	 * @fn
	 * 波形を求める
	 * @brief フレームごとの音素と音高から、波形を求める
	 * @param[in] Length フレームの長さ
	 * @param[in] PhonemeSize 音素の種類数
	 * @param[in] F0 フレームごとの音高
	 * @param[in] Phoneme フレームごとの音素
	 * @param[in] SpeakerID 話者番号
	 * @return 音声波形
	 *
	 * @warning 動作確認が取れていないため、クラッシュ、もしくは予期せぬ動作をする可能性が高いです。
	 */
	TArray<float> DecodeForward(int64 Length, int64 PhonemeSize, TArray<float> F0, TArray<float> Phoneme, int64 SpeakerID) const;
	
};