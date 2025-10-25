// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxApiObject.h
 * @brief  複数のVOICEVOX COREライブラリへアクセスするSubsystemを管理し、各APIへアクセスするヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/VoicevoxSubsystemCollection.h"
#include "UObject/Object.h"
#include "VoicevoxApiObject.generated.h"

class UVoicevoxCoreSubsystem;

/**
 * @class UVoicevoxApiObject
 * @brief COREライブラリへアクセスするSubsystemを管理し、各APIへアクセスするクラス
 * @details COREライブラリへアクセスするSubsystemを管理するオブジェクトです。
 *			UVoicevoxCoreSubsystem以外からはアクセス不可のため、APIを呼ぶ時はUVoicevoxCoreSubsystemを利用してください。
 */
UCLASS(transient)
class UVoicevoxApiObject : public UObject
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
	VOICEVOXUECORE_API bool CoreInitialize(bool bUseGPU, int CPUNumThreads = 0, bool bLoadAllModels = false);

	/**
	 * @brief デフォルトの初期化オプションを生成する
	 * @return デフォルト値が設定された初期化オプション
	 */
	VOICEVOXUECORE_API VoicevoxInitializeOptions MakeDefaultInitializeOptions();
	
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
	 * @brief　VVMファイルのロード実行
	 * @param [in] VvmFileName vvmファイル名
	 * @returns 読み込み結果
	 */
	VOICEVOXUECORE_API bool LoadVoiceModel(const FString& VvmFileName);

	/**
	 * @brief　全てのVVMファイルを開く。
	 * @returns 読み込み結果
	 */
	VOICEVOXUECORE_API bool AllLoadVoiceModel();
	
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
	VOICEVOXUECORE_API bool LoadModel(int64 SpeakerId);

	/**
	 * @breaf 音声モデルの読み込みを解除する。
	 * @param [in] VvmFileName vvmファイル名
	 * @returns 解除処理の結果
	 */
	VOICEVOXUECORE_API bool UnloadVoiceModel(const FString& VvmFileName);

	/**
	 * @breaf 指定したIDの音声モデルが読み込まれているか判定する。
	 * @param [in] VvmFileName vvmファイル名
	 * @returns モデルが読み込まれているかどうか
	 */
	VOICEVOXUECORE_API bool IsLoadedVoiceModel(const FString& VvmFileName);
	
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
	 * @brief デフォルトのテキスト音声合成オプションを生成する
	 * @return テキスト音声合成オプション
	 */
	VOICEVOXUECORE_API VoicevoxTtsOptions MakeDefaultTtsOptions();
	
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

	/**
	 * @brief デフォルトの `voicevox_synthesis` のオプションを生成する
	 * @return デフォルト値が設定された `voicevox_synthesis` のオプション
	 */
	VOICEVOXUECORE_API VoicevoxSynthesisOptions MakeDefaultSynthesisOptions();

	/**
	 * @fn
	 * メタ情報を取得する
	 * @brief 話者名や話者IDのリストを取得する
	 * @return メタ情報が格納されたjson形式の構造体
	 */
	VOICEVOXUECORE_API TArray<FVoicevoxMeta> GetMetaList();

	/**
	 * @fn
	 * 指定のVoicevoxVoiceModelFileからメタ情報を取得する
	 * @brief 話者名や話者IDのリストを取得する
	* @param [in] VvmFileName vvmファイル名
	 * @return メタ情報が格納されたjson形式の構造体リスト、ない場合は空のリスト
	 */
	VOICEVOXUECORE_API TArray<FVoicevoxMeta> GetVoiceModelFileMetaList(const FString& VvmFileName);
	
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
