// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxNativeCoreSubsystem.h
 * @brief VOICEVOX COREのネイティブライブラリのAPIを実行する基礎Subsystemヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxApiSubsystem.h"
#include "Subsystems/Subsystem.h"
#include "VoicevoxNativeCoreSubsystem.generated.h"

/**
 * @class UVoicevoxNativeCoreSubsystem
 * @brief VOICEVOX COREのネイティブライブラリのAPIを実行する基礎Subsystemクラス
 */
UCLASS(Abstract, MinimalAPI)
class UVoicevoxNativeCoreSubsystem : public UVoicevoxApiSubsystem
{
	GENERATED_BODY()
protected:
	
	//----------------------------------------------------------------
	// Variable
	//----------------------------------------------------------------

	//! VOICEVOX COREライブラリハンドル
	void* CoreLibraryHandle = nullptr;

	//! 
	VoicevoxSynthesizer* Synthesizer;
	
	//!
	const VoicevoxOnnxruntime* Onnxruntime;
	
	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------

	/**
	 * @brief OpenJtakeのディレクトリ名を取得
	 * @return OpneJtakeのディレクトリ名
	 */
	virtual FString GetOpenJtakeDirectoryName() { return FString(); }
	
	/**
	 * @fn
	 * VOICEVOX COREのvoicevox_ttsで生成した音声データを開放
	 * @brief voicevox_tts等で生成した音声データを開放する
	 * @param Wav 開放する音声データのポインタ
	 */
	VOICEVOXUECORE_API void WavFree(uint8* Wav);

	/**
	 * @brief OpenJtalkRc を<b>破棄</b>(_destruct_)する。
	 * 破棄対象への他スレッドでのアクセスが存在する場合、それらがすべて終わるのを待ってから破棄する。
	 * この関数の呼び出し後に破棄し終えた対象にアクセスすると、プロセスを異常終了する。
	 * @param [in] OpenJTalk 破棄対象。nullable
	 */
	VOICEVOXUECORE_API void OpenJTalkRcDelete(OpenJtalkRc *OpenJTalk);

	/**
	 * @brief JSON文字列を解放する。
	 *
	 * @param [in] JsonData 解放するJSON文字列。nullable
	 */
	VOICEVOXUECORE_API void JsonFree(char* JsonData);
	
	/**
	 * エラー結果をメッセージに変換して表示
	 * @param[in] ApiName : エラーを起こしたAPI名
	 * @param [in] ResultCode メッセージに変換するエラーコード
	 */
	VOICEVOXUECORE_API void VoicevoxShowErrorResultMessage(const FString& ApiName, VoicevoxResultCode ResultCode);
	
public:

	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------
	
	//--------------------------------
	// コンストラクタ
	//--------------------------------

	/**
	 * @brief コンストラクタ
	 */
	UVoicevoxNativeCoreSubsystem() = default;
	
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
	VOICEVOXUECORE_API virtual bool ApiInitialize(bool bUseGPU, int CPUNumThreads = 0, bool bLoadAllModels = false) override;

	/**
	 * @brief デフォルトの初期化オプションを生成する
	 * @return デフォルト値が設定された初期化オプション
	 */
	VOICEVOXUECORE_API virtual VoicevoxInitializeOptions MakeDefaultInitializeOptions() override;
	
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
	VOICEVOXUECORE_API virtual void Finalize() override;

	//--------------------------------
	// VOICEVOX CORE Model関連
	//--------------------------------
	
	/**
	 * @brief　VVMファイルのロード実行
	 * @param [in] VvmFileName vvmファイル名
	 * @returns 読み込み結果
	 */
	VOICEVOXUECORE_API virtual bool LoadVoiceModel(const FString VvmFileName) override;
	
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
	VOICEVOXUECORE_API virtual bool LoadModel(int64 SpeakerId) override;

	/**
	 * @fn
	 * VOICEVOX COREに該当のスピーカーモデルが存在するか
	 * @brief 使用するCOREにスピーカーモデルが存在するか
	 * @param SpeakerId 話者番号
	 * @return 存在したらtrue、無い場合はfalse
	 */
	VOICEVOXUECORE_API virtual bool IsModel(int64 SpeakerId) override;

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
	VOICEVOXUECORE_API virtual FVoicevoxAudioQuery GetAudioQuery(int64 SpeakerId, const FString& Message, bool bKana) override;
	
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
	VOICEVOXUECORE_API virtual TArray<uint8> RunTextToSpeech(int64 SpeakerId, const FString& Message, bool bKana, bool bEnableInterrogativeUpspeak) override;

	/**
	 * @brief デフォルトのテキスト音声合成オプションを生成する
	 * @return テキスト音声合成オプション
	 */
	VOICEVOXUECORE_API virtual VoicevoxTtsOptions MakeDefaultTtsOptions() override;
	
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
	VOICEVOXUECORE_API virtual TArray<uint8> RunSynthesis(const char* AudioQueryJson, int64 SpeakerId, bool bEnableInterrogativeUpspeak) override;

	/**
	 * @fn
	 * VOICEVOX COREのvoicevox_synthesisを実行
	 * @brief AudioQueryを音声データに変換する。
	 * @param[in] AudioQueryJson jsonフォーマットされた AudioQuery構造体
	 * @param[in] SpeakerId 話者番号
	 * @param[in] bEnableInterrogativeUpspeak 疑問文の調整を有効にする
	 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
	 * @details
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	VOICEVOXUECORE_API virtual TArray<uint8> RunSynthesis(const FVoicevoxAudioQuery& AudioQueryJson, int64 SpeakerId, bool bEnableInterrogativeUpspeak) override;

	/**
	 * @brief デフォルトの `voicevox_synthesis` のオプションを生成する
	 * @return デフォルト値が設定された `voicevox_synthesis` のオプション
	 */
	VOICEVOXUECORE_API virtual VoicevoxSynthesisOptions MakeDefaultSynthesisOptions() override;
	
	//--------------------------------
	// VOICEVOX CORE Property関連
	//--------------------------------
	
	/**
	 * @fn
	 * メタ情報を取得する
	 * @brief 話者名や話者IDのリストを取得する
	 * @return メタ情報が格納されたjson形式の構造体
	 */
	VOICEVOXUECORE_API virtual TArray<FVoicevoxMeta> GetMetaList() override;

	/**
	 * @brief サポートデバイス情報を取得する
	 * @return サポートデバイス情報の構造体
	 */
	VOICEVOXUECORE_API virtual FVoicevoxSupportedDevices GetSupportedDevices() override;

	/**
	 * @brief VOICEVOX COREのバージョンを取得する
	 * @return SemVerでフォーマットされたバージョン
	 */
	VOICEVOXUECORE_API virtual FString GetVoicevoxVersion() override;

	/**
	 * @brief ハードウェアアクセラレーションがGPUモードか判定する
	 * @return GPUモードならtrue、そうでないならfalse
	 */
	VOICEVOXUECORE_API virtual bool IsGpuMode() override;

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
	VOICEVOXUECORE_API virtual TArray<float> GetPhonemeLength(int64 Length, TArray<int64> PhonemeList, int64 SpeakerID) override;

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
	VOICEVOXUECORE_API virtual TArray<float> FindPitchEachMora(int64 Length, TArray<int64> VowelPhonemeList, TArray<int64> ConsonantPhonemeList,
	                                                            TArray<int64> StartAccentList, TArray<int64> EndAccentList,
	                                                            TArray<int64> StartAccentPhraseList, TArray<int64> EndAccentPhraseList,
	                                                            int64 SpeakerID) override;

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
	VOICEVOXUECORE_API virtual TArray<float> DecodeForward(int64 Length, int64 PhonemeSize, TArray<float> F0, TArray<float> Phoneme, int64 SpeakerID) override;
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxNativeCore, Log, All);