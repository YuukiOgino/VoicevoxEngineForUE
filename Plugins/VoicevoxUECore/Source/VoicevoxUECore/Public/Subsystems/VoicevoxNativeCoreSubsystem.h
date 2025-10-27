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

	//! 音声シンセサイザ
	VoicevoxSynthesizer* Synthesizer = nullptr;

	//! テキスト解析器としてのOpen JTalk。
	OpenJtalkRc* OpenJTalk = nullptr;

	//! 読み込んだモデルIDマップ 
	TMap<FString, TArray<uint8>> ModelIdMap;
	
	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------

public:
	
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
	 */
	VOICEVOXUECORE_API virtual void Finalize() override;

protected:

	/**
	 * @brief VoicevoxSynthesizer を<b>構築</b>する。
	 * @param [in] OnnxRuntime
	 * @param[in] bUseGPU			trueならGPU用、falseならCPU用の初期化を行う
	 * @param[in] CPUNumThreads		推論に用いるスレッド数を設定する。0の場合論理コア数の半分か、物理コア数が設定される
	 * @returns 結果
	 */
	VOICEVOXUECORE_API bool SynthesizerNew(const VoicevoxOnnxruntime& OnnxRuntime, bool bUseGPU, int CPUNumThreads);
	
	/**
	 * @brief VoicevoxSynthesizer を<b>破棄</b>する。
	 * 破棄対象への他スレッドでのアクセスが存在する場合、それらがすべて終わるのを待ってから破棄する。
	 * この関数の呼び出し後に破棄し終えた対象にアクセスすると、プロセスを異常終了する。
	 */
	VOICEVOXUECORE_API void SynthesizerDelete();
	
	//--------------------------------
	// VOICEVOX CORE ONNX Runtime関連
	//--------------------------------
	
	/**
	 * @brief VoicevoxOnnxruntime のインスタンスを得る。
	 * @returns VoicevoxOnnxruntime のインスタンス
	 */
	VOICEVOXUECORE_API const VoicevoxOnnxruntime* SynthesizerGetOnnxRuntime();
	
	/**
	 * @brief VoicevoxOnnxruntime のインスタンスが既に作られているならそれを得る。 作られていなければ`NULL`を返す。
	 * @returns VoicevoxOnnxruntime のインスタンス
	 */
	VOICEVOXUECORE_API const VoicevoxOnnxruntime* GetOnnxRuntime();

	/**
	 * @brief ONNX Runtimeをロードして初期化する。
	 * 一度成功したら、以後は引数を無視して同じ参照を返す。
	 *
	 * @param [in] Option オプション
	 * @returns VoicevoxOnnxruntime のインスタンス
	 */
	VOICEVOXUECORE_API const VoicevoxOnnxruntime* OnnxRuntimeLoadOnce(const VoicevoxLoadOnnxruntimeOptions& Option);
	
	/**
	 * @brief ONNX Runtimeを初期化する。
	 * 一度成功したら以後は同じ参照を返す。
	 *
	 * @returns VoicevoxOnnxruntime のインスタンス
	 */
	VOICEVOXUECORE_API const VoicevoxOnnxruntime* OnnxRuntimeInitOnce();

	/**
	 * @brief デフォルトのvoicevox_onnxruntime_load_once のオプションを生成する。
	 * @return デフォルトのvoicevox_onnxruntime_load_once のオプション
	 */
	VOICEVOXUECORE_API VoicevoxLoadOnnxruntimeOptions MakeDefaultLoadOnnxRuntimeOptions();
	
public:
	
	/**
	 * @brief ONNX Runtimeの動的ライブラリの、バージョン付きのファイル名を取得。
	 * WindowsとAndroidでは ::voicevox_get_onnxruntime_lib_unversioned_filename と同じ。
	 * @return ONNX Runtimeの動的ライブラリの、バージョン付きのファイル名
	 */
	VOICEVOXUECORE_API FString GetOnnxRuntimeLibVersionedFilename();

	/**
	 * @brief ONNX Runtimeの動的ライブラリの、バージョン無しのファイル名を取得。
	 * @return ONNX Runtimeの動的ライブラリの、バージョン無しのファイル名。
	 */
	VOICEVOXUECORE_API FString GetOnnxRuntimeLibUnversionedFilename();

	//--------------------------------
	// VOICEVOX CORE OpenJtake関連
	//--------------------------------
	
protected:
	
	/**
	 * @brief OpenJtakeのディレクトリ名を取得
	 * @return OpenJtakeのディレクトリ名
	 */
	virtual FString GetOpenJTakeDirectoryName() { return FString(); }

	/**
	 * @brief テキスト解析器としてのOpen JTalkを作成する。
	 * @return 取得結果
	 */
	VOICEVOXUECORE_API bool OpenJTalkRcNew();
	
	/**
	 * @brief OpenJtalkRc を<b>破棄</b>(_destruct_)する。
	 * 破棄対象への他スレッドでのアクセスが存在する場合、それらがすべて終わるのを待ってから破棄する。
	 * この関数の呼び出し後に破棄し終えた対象にアクセスすると、プロセスを異常終了する。
	 * @param [in] Rc 破棄対象。nullable
	 */
	VOICEVOXUECORE_API void OpenJTalkRcDelete(OpenJtalkRc* Rc);
	
public:

	/**
	 * @brief 日本語のテキストを解析する。
	 * @param [in] Text 日本語テキスト
	 * @returns OpenJTakから解析されたデータ（FVoicevoxOpenJTalkAnalyze）
	 */
	VOICEVOXUECORE_API FVoicevoxAccentPhraseAnalyze OpenJTalkRcAnalyze(const FString& Text);

	//--------------------------------
	// VOICEVOX CORE Model関連(旧API)
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
	// VOICEVOX CORE VMM関連
	//--------------------------------

protected:
	/**
	 * @brief VVMファイルを開く。
	 *
	 * @param [in] Path vvmファイルへのUTF-8のファイルパス
	 * @param [out] Model 構築先
	 *
	 * @returns 結果
	 */
	VOICEVOXUECORE_API bool VoiceModelFileOpen(const FString& Path, VoicevoxVoiceModelFile** Model);

	/**
	 * 音声モデルを読み込む。
	 * @param [in] Model 音声モデル
	 *
	 * @returns 結果
	 */
	VOICEVOXUECORE_API bool SynthesizerLoadVoiceModel(const VoicevoxVoiceModelFile& Model);
	
	/**
	 * @brief VoicevoxVoiceModelFile を、所有しているファイルディスクリプタを閉じた上で<b>破棄</b>(_destruct_)する。ファイルの削除(_delete_)<b>ではない</b>。
	 * 破棄対象への他スレッドでのアクセスが存在する場合、それらがすべて終わるのを待ってから破棄する。
	 * この関数の呼び出し後に破棄し終えた対象にアクセスすると、プロセスを異常終了する。
	 *
	 * @param [in] Model 破棄対象。nullable
	 */
	VOICEVOXUECORE_API void VoiceModelFileDelete(VoicevoxVoiceModelFile& Model);

	/**
	 * @brief  VoicevoxVoiceModelFile からIDを取得する。
	 *
	 * @param [in] Model 音声モデル
	 * @return 音声モデルID。
	 */
	VOICEVOXUECORE_API TArray<uint8_t> VoiceModelFileId(const VoicevoxVoiceModelFile& Model);

public:
	
	/**
	 * @brief　VVMファイルのロード実行
	 * @param [in] VvmFileName vvmファイル名
	 * @returns 読み込み結果
	 *
	 * @detail
	 * モデルによってはメインスレッドが暫く止まるほど重いので、その場合は非同期で処理してください。（UE::Tasks::Launch等）
	 */
	VOICEVOXUECORE_API virtual bool LoadVoiceModel(const FString VvmFileName) override;

	/**
	 * @brief　全てのVVMファイルを開く。
	 * @returns 読み込み結果
	 *
	 * @detail
	 * メインスレッドが暫く止まるほど重いので、その場合は非同期で処理してください。（UE::Tasks::Launch等）
	 */
	VOICEVOXUECORE_API virtual bool AllLoadVoiceModel() override;

	/**
	 * @breaf 音声モデルの読み込みを解除する。
	 * @param [in] VvmFileName vvmファイル名
	 * @returns 解除処理の結果
	 */
	VOICEVOXUECORE_API virtual bool UnloadVoiceModel(FString VvmFileName) override;

	/**
	 * @breaf 指定したIDの音声モデルが読み込まれているか判定する。
	 * @param [in] VvmFileName vvmファイル名
	 * @returns モデルが読み込まれているかどうか
	 */
	VOICEVOXUECORE_API virtual bool IsLoadedVoiceModel(FString VvmFileName) override;
	
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

	/**
	 * @brief AccentPhraseの配列からAudioQueryを作る
	 * @param [in] AccentPhrases AccentPhraseの配列
	 * @return AudioQuery情報が格納されたjson形式の構造体
	 */
	VOICEVOXUECORE_API FVoicevoxAudioQuery GetAudioQueryFromAccentPhrases(TArray<FVoicevoxAccentPhrase> AccentPhrases);
	
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
	 * @brief 日本語テキストから、AccentPhrase (アクセント句)の配列を生成する。
	 * @param [in] StyleId スタイルID
	 * @param [in] Text 音声データに変換する日本語テキスト
	 * @param [in] bKana AquesTalk形式のkanaとしてテキストを解釈する
	 * @returns AccentPhrase (アクセント句)の配列を格納した構造体（FVoicevoxAccentPhraseAnalyze）
	 */
	VOICEVOXUECORE_API virtual FVoicevoxAccentPhraseAnalyze SynthesizerCreateAccentPhrases(VoicevoxStyleId StyleId, const FString& Text, bool bKana) override;
	
	/**
	 * @brief デフォルトの `voicevox_synthesis` のオプションを生成する
	 * @return デフォルト値が設定された `voicevox_synthesis` のオプション
	 */
	VOICEVOXUECORE_API virtual VoicevoxSynthesisOptions MakeDefaultSynthesisOptions() override;
	
	//--------------------------------
	// VOICEVOX CORE Property関連
	//--------------------------------
	
protected:

	/**
	 * @brief VoicevoxVoiceModelFile からメタ情報を取得する。
	 * @param [in] Model 音声モデル
	 * @returns VoicevoxVoiceModelFileのメタ情報
	 */
	VOICEVOXUECORE_API TArray<FVoicevoxMeta> VoiceModelFileCreateMetas(const VoicevoxVoiceModelFile& Model);

	/**
	 * @brief コアライブラリをロード済みか
	 * @return ローダ済みで参照可能な場合はtrue、それ以外はfalse
	 */
	VOICEVOXUECORE_API bool IsValidCoreLibraryHandle();

	/**
	 * @brief プラットフォームのフォルダ名を取得
	 * @return プラットフォームのフォルダ名。空の場合は非対応
	 */
	VOICEVOXUECORE_API FString GetPlatformFolderName();
	
public:
	/**
	 * @fn
	 * メタ情報を取得する
	 * @brief 話者名や話者IDのリストを取得する
	 * @return メタ情報が格納されたjson形式の構造体リスト
	 */
	VOICEVOXUECORE_API virtual TArray<FVoicevoxMeta> GetMetaList() override;

	/**
	 * @fn
	 * 指定のVoicevoxVoiceModelFileからメタ情報を取得する
	 * @brief 話者名や話者IDのリストを取得する
	* @param [in] VvmFileName vvmファイル名
	 * @return メタ情報が格納されたjson形式の構造体リスト、ない場合は空のリスト
	 */
	VOICEVOXUECORE_API virtual TArray<FVoicevoxMeta> GetVoiceModelFileMetaList(const FString VvmFileName) override;

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
	// VOICEVOX CORE Free関連
	//--------------------------------

protected:
	
	/**
	 * @fn
	 * VOICEVOX COREのvoicevox_ttsで生成した音声データを開放
	 * @brief voicevox_tts等で生成した音声データを開放する
	 * @param Wav 開放する音声データのポインタ
	 */
	VOICEVOXUECORE_API void WavFree(uint8* Wav);

	/**
	 * @brief JSON文字列を解放する。
	 *
	 * @param [in] JsonData 解放するJSON文字列。nullable
	 */
	VOICEVOXUECORE_API void JsonFree(char* JsonData);

	//--------------------------------
	// VOICEVOX CORE Error関連
	//--------------------------------
	
	/**
	 * エラー結果をメッセージに変換して表示
	 * @param[in] ApiName : エラーを起こしたAPI名
	 * @param [in] ResultCode メッセージに変換するエラーコード
	 */
	VOICEVOXUECORE_API void VoicevoxShowErrorResultMessage(const FString& ApiName, VoicevoxResultCode ResultCode);

	/**
	 * DLL読み込み失敗をメッセージに変換して表示
	 * @param[in] ApiName : エラーを起こしたAPI名
	 */
	VOICEVOXUECORE_API void ShowDllErrorMessage(const FString& ApiName);
	
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxNativeCore, Log, All);