// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxNativeObject.h
 * @brief  複数のVOICEVOX COREライブラリへアクセスするSubsystemを管理し、各APIへアクセスするヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/VoicevoxSubsystemCollection.h"
#include "UObject/Object.h"
#include "VoicevoxNativeObject.generated.h"

class UVoicevoxCoreSubsystem;

/**
 * @class UVoicevoxNativeObject
 * @brief COREライブラリへアクセスするSubsystemを管理し、各APIへアクセスするクラス
 * @details COREライブラリへアクセスするSubsystemを管理するオブジェクトです。
 *			UVoicevoxCoreSubsystem以外からはアクセス不可のため、APIを呼ぶとき
 */
UCLASS(transient)
class UVoicevoxNativeObject : public UObject
{
	GENERATED_BODY()

	//----------------------------------------------------------------
	// friend class
	//----------------------------------------------------------------
	
	friend class UVoicevoxCoreSubsystem;

	//----------------------------------------------------------------
	// Variable
	//----------------------------------------------------------------
	
	//! VOICEVOX Native Subsystem管理オブジェクトに登録したクラスリスト
	TArray<UClass*> SubsystemClasses;

	//! VOICEVOX Native Subsystem管理オブジェクト
	FVoicevoxSubsystemCollection VoicevoxSubsystemCollection;
	
	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------

	/**
	 * @brief サブシステム管理オブジェクト初期化
	 */
	VOICEVOXUECORE_API void Init();

	/**
	 * @brief サブシステム管理オブジェクト破棄
	 */
	VOICEVOXUECORE_API void Shutdown();
	
	//--------------------------------
	// VOICEVOX CORE APIアクセス関数
	//--------------------------------

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
	 *
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	VOICEVOXUECORE_API void CoreInitialize(bool bUseGPU, int CPUNumThreads = 0, bool bLoadAllModels = false);

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
	VOICEVOXUECORE_API void Finalize();

	//--------------------------------
	// VOICEVOX CORE Model関連
	//--------------------------------
	
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
	VOICEVOXUECORE_API void LoadModel(int64 SpeakerId);

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
	VOICEVOXUECORE_API FVoicevoxAudioQuery GetAudioQuery(int64 SpeakerId, const FString& Message, bool bKana);

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
	VOICEVOXUECORE_API TArray<uint8> RunTextToSpeech(int64 SpeakerId, const FString& Message, bool bKana, bool bEnableInterrogativeUpspeak);

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
	VOICEVOXUECORE_API TArray<uint8> RunSynthesis(const char* AudioQueryJson, int64 SpeakerId, bool bEnableInterrogativeUpspeak);

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
	VOICEVOXUECORE_API TArray<uint8> RunSynthesis(const FVoicevoxAudioQuery& AudioQueryJson, int64 SpeakerId, bool bEnableInterrogativeUpspeak);

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
	VOICEVOXUECORE_API TArray<float> GetPhonemeLength(int64 Length, TArray<int64> PhonemeList, int64 SpeakerID);

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
	VOICEVOXUECORE_API TArray<float> FindPitchEachMora(int64 Length, TArray<int64> VowelPhonemeList, TArray<int64> ConsonantPhonemeList,
											  TArray<int64> StartAccentList, TArray<int64> EndAccentList,
											  TArray<int64> StartAccentPhraseList, TArray<int64> EndAccentPhraseList,
											  int64 SpeakerID);

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
	VOICEVOXUECORE_API TArray<float> DecodeForward(int64 Length, int64 PhonemeSize, TArray<float> F0, TArray<float> Phoneme, int64 SpeakerID);

public:

	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------
	
	/**
	 * @brief リファレンスオブジェクトをガベージコレクション対象外に登録
	 * @param [in] InThis 登録するオブジェクト。
	 * @param [in] Collector FReferenceCollectorオブジェクト
	 */
	static VOICEVOXUECORE_API void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);
};
