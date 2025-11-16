// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxBlueprintLibrary.h
 * @brief  VOICEVOX COREのAPIへ接続するBlueprint公開ノードをまとめたヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxUEDefined.h"
#include "VoicevoxQuery.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VoicevoxBlueprintLibrary.generated.h"

/**
 * @class UVoicevoxBlueprintLibrary
 * @brief BlueprintでVOICEVOX COREのAPIへアクセスするためのBlueprintFunctionLibraryクラス
 * @details BlueprintでVOICEVOX COREのAPIへアクセスするためのBlueprintFunctionLibraryクラス。一部APIは非同期処理が必要なためLatentノードクラスで実行
 */
UCLASS()
class VOICEVOXENGINE_API UVoicevoxBlueprintLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:

	/**
	 * @brief 全てのVOICEVOX CORE 初期化が完了しているか
	 * @return 全てのCOREの初期化実行済みの場合はtrue
	 */
	UFUNCTION(BlueprintPure, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxIsInitialize"))
	static UPARAM(DisplayName="IsInitialize")  bool IsInitialize();
	
	/**
	 * @fn
	 * VOICEVOX CORE 終了処理
	 * @brief 終了処理を行う。以降VOICEVOXのAPIを利用するためには再度Initializeメソッドを行う必要がある。(Blueprint公開ノード)
	 * @detail
	 * VOICEVOXの終了処理は何度も実行可能。
	 * 実行せずにexitしても大抵の場合問題ないが、CUDAを利用している場合は終了処理を実行しておかないと例外が起こることがある。
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxFinalize"))
	static void Finalize();

	/**
	 * @brief 日本語のテキストを解析する。
	 * @param [in] Text 日本語テキスト
	 * @returns OpenJTakから解析されたAccentPhrase (アクセント句)の配列
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetOpenJTalkRcAnalyze"))
	static UPARAM(DisplayName="OpenJTalkAnalyze") TArray<FVoicevoxAccentPhrase> OpenJTalkRcAnalyze(const FString& Text);
	
	/**
	 * @breaf 音声モデルの読み込みを解除する。
	 * @param [in] VvmFileName vvmファイル名
	 * @returns 解除処理の結果
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxUnLoadVoiceModel"))
	static bool UnloadVoiceModel(FString VvmFileName);

	/**
	 * @breaf 指定したIDの音声モデルが読み込まれているか判定する。
	 * @param [in] VvmFileName vvmファイル名
	 * @returns モデルが読み込まれているかどうか
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxIsLoadedVoiceModel"))
	static bool IsLoadedVoiceModel(FString VvmFileName);
	
	/**
	 * @fn
	 *  初期化済みのVOICEVOX CORE名のリスト取得(Blueprint公開ノード)
	 * @brief 初期化済みのVOICEVOX CORE名のリスト取得
	 * @return 初期化済みのVOICEVOX CORE名のリスト
	 */
	UFUNCTION(BlueprintPure, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxCoreNameList"))
	static UPARAM(DisplayName="CoreNameList") TArray<FString> GetCoreNameList();
	
	/**
	 * @fn
	 *  VOICEVOX COREメタ情報を取得する(Blueprint公開ノード)
	 * @brief 話者名や話者IDのリストを取得する(Blueprint公開ノード)
	 * @return メタ情報が格納されたjson形式の構造体
	 */
	UFUNCTION(BlueprintPure, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxMetas"))
	static UPARAM(DisplayName="Metas") TArray<FVoicevoxMeta> GetMetasToList();

	/**
	 * @fn
	 * 指定のVoicevoxVoiceModelFileからメタ情報を取得する
	 * @brief 話者名や話者IDのリストを取得する
	 * @param [in] VvmFileName vvmファイル名
	 * @return メタ情報が格納されたjson形式の構造体リスト、ない場合は空のリスト
	 */
	UFUNCTION(BlueprintPure, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxModelFileMetas"))
	static UPARAM(DisplayName="Metas")  TArray<FVoicevoxMeta> GetVoiceModelFileMetaList(const FString& VvmFileName);

	/**
	 * @brief 全てのVoicevoxVoiceModelFileからメタ情報を取得する
	 * @return メタ情報が格納されたjson形式の構造体リスト、ない場合は空のリスト
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetAllVoicevoxModelFileMetas"))
	static UPARAM(DisplayName="Metas")   TArray<FVoicevoxMeta> GetAllVoiceModelFileMetaList();
	
	/**
	 * @brief サポートデバイス情報を取得する(Blueprint公開ノード)
	 * @return サポートデバイス情報の構造体
	 */
	UFUNCTION(BlueprintPure, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxSupportedDevices"))
	static UPARAM(DisplayName="SupportedDevices") FVoicevoxSupportedDevices GetSupportedDevices(const FString& CoreName);

	/**
	 * @brief VOICEVOX COREのバージョン情報を取得する
	 * @return VOICEVOX COREのバージョン情報
	 */
	UFUNCTION(BlueprintPure, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxVersion"))
	static UPARAM(DisplayName="VoicevoxCoreVersion") FString GetVoicevoxCoreVersion(const FString& CoreName);

	/**
	 * @brief ハードウェアアクセラレーションがGPUモードか判定する
	 * @return GPUモードならtrue、そうでないならfalse
	 */
	UFUNCTION(BlueprintPure, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "IsVoicevoxGpuMode"))
	static UPARAM(DisplayName="GPU Mode") bool IsVoicevoxGpuMode(const FString& CoreName);
	
	/**
	 * @brief VOICEVOX COREで変換した音声データを元にSoundWaveを生成(Blueprint公開ノード)
	 * @param[in] StyleId							スタイルID
	 * @param[in] Message							音声データに変換するtextデータ
	 * @param[in] bRunKana							AquesTalkライクな記法で実行するか
	 * @param[in] bEnableInterrogativeUpspeak		疑問文の調整を有効にする
	 * @return TextToSpeechで作成された音楽データが格納されたUSoundWave
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxTextToSpeechOutput"))
	static UPARAM(DisplayName="Sound") USoundWave* TextToSpeechOutput(int32 StyleId, FString Message, bool bRunKana = false, bool bEnableInterrogativeUpspeak = true);
	
	/**
	 * @brief 入力したテキストをVOICEVOX COREでAudioQueryに変換後、SoundWaveを生成(Blueprint公開ノード)
	 * @param[in] StyleId							スタイルID
	 * @param[in] Message							音声データに変換するtextデータ
	 * @param[in] bRunKana							AquesTalkライクな記法で実行するか
	 * @param[in] bEnableInterrogativeUpspeak		疑問文の調整を有効にする
	 * @return AudioQuery情報を元に作成された音楽データが格納されたUSoundWave
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxToTextAudioQueryOutput"))
	static UPARAM(DisplayName="Sound") USoundWave* TextToAudioQueryOutput(int32 StyleId, FString Message, bool bRunKana = false, bool bEnableInterrogativeUpspeak = true);
	
	/**
	 * @fn
	 *  VOICEVOX COREで変換したAudioQueryを取得する(Blueprint公開ノード)
	 * @param[in] StyleId		スタイルID
	 * @param[in] Message		音声データに変換するtextデータ
	 * @param[in] bRunKana		AquesTalkライクな記法で実行するか
	 * @return AudioQuery情報が格納されたjson形式の構造体
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxAudioQuery"))
	static UPARAM(DisplayName="AudioQuery") FVoicevoxAudioQuery GetAudioQuery(int32 StyleId, FString Message, bool bRunKana = false);

	/**
	 * @brief AccentPhraseの配列からAudioQueryを作る(Blueprint公開ノード)
	 * @param [in] AccentPhrases AccentPhraseの配列
	 * @return AudioQuery情報が格納されたjson形式の構造体
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxAudioQueryFromAccentPhrases"))
	static UPARAM(DisplayName="AudioQuery") FVoicevoxAudioQuery GetAudioQueryFromAccentPhrases(const TArray<FVoicevoxAccentPhrase>& AccentPhrases);

	/**
	 * @brief 日本語テキストから、AccentPhrase (アクセント句)の配列を生成する。(Blueprint公開ノード)
	 * @param [in] StyleId スタイルID
	 * @param [in] Text 音声データに変換する日本語テキスト
	 * @param [in] bKana AquesTalk形式のkanaとしてテキストを解釈する
	 * @returns AccentPhrase (アクセント句)の配列
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxAccentPhrase"))
	static UPARAM(DisplayName="AccentPhraseAnalyze") TArray<FVoicevoxAccentPhrase> GetSynthesizerAccentPhrase(int32 StyleId, const FString& Text, bool bKana);

	/**
	 * @brief AccentPhraseの配列の指定パラメータを、特定の声で生成しなおす。(Blueprint公開ノード)
	 * @param [in] ReplaceType 実行する再生成APIのタイプ
	 * @param [in] AccentPhrases AccentPhraseの配列
	 * @param [in] StyleId スタイルID
	 * @returns AccentPhrase (アクセント句)の配列
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxReplace"))
	static UPARAM(DisplayName="AccentPhraseAnalyze") TArray<FVoicevoxAccentPhrase> SynthesizerReplace(EReplaceType ReplaceType, const TArray<FVoicevoxAccentPhrase>& AccentPhrases, int32 StyleId);
	
	/**
	 * @brief VOICEVOX COREで取得したAudioQueryを元にSoundWaveを作成(Blueprint公開ノード)
	 * @param[in] AudioQuery						AudioQuery構造体
	 * @param[in] StyleId							スタイルID
	 * @param[in] bEnableInterrogativeUpspeak		疑問文の調整を有効にする
	 * @return AudioQuery情報を元に作成された音楽データが格納されたUSoundWave
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxAudioQueryOutput"))
	static UPARAM(DisplayName="Sound") USoundWave* AudioQueryOutput(FVoicevoxAudioQuery AudioQuery, int32 StyleId, bool bEnableInterrogativeUpspeak = true);

	/**
	 * @brief AudioQueryアセットからSoundWaveを作成(Blueprint公開ノード)
	 * @param[in] VoicevoxQuery						Queryアセット
	 * @param[in] bEnableInterrogativeUpspeak		疑問文の調整を有効にする
	 * @return AudioQuery情報を元に作成された音楽データが格納されたUSoundWave
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxQueryAssetOutput"))
	static UPARAM(DisplayName="Sound") USoundWave* VoicevoxQueryOutput(UVoicevoxQuery* VoicevoxQuery, bool bEnableInterrogativeUpspeak = true);
	
	/**
	 * @brief 生成した音声データからUSoundWaveを作成
	 * @param[in] PCMData  音声データ
	 * @return 合成音声を格納したUSoundWave
	 */
	static USoundWave* CreateSoundWave(TArray<uint8> PCMData);

	/**
	 * @brief VOICEVOX COREで取得したAudioQuery元に、中品質なLipSyncに必要なデータリストを取得(Blueprint公開ノード)
	 * @param[in] AudioQuery AudioQuery構造体
	 * @return AudioQuery情報を元に生成した、中品質のLipSyncに必要なデータリスト
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxLipSyncList"))
	static UPARAM(DisplayName="LipSyncList") TArray<FVoicevoxLipSync> GetLipSyncList(FVoicevoxAudioQuery AudioQuery);
	
	//--------------------------------
	// VOICEVOX CORE Dict関連
	//--------------------------------
	
	/**
	 * @brief VoicevoxUserDictWordを最低限のパラメータで作成する。(Blueprint公開ノード)
	 * @param [in] Surface 表記
	 * @param [in] Pronunciation 読み
	 * @param [in] AccentType アクセント型
	 * @returns VoicevoxUserDictWord
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxUserDictWordMake"))
	static UPARAM(DisplayName="UserDictWord")  FVoicevoxCoreUserDictWord UserDictWordMake(const FString& Surface, const FString& Pronunciation, int64 AccentType);
	
	/**
	 * @brief ユーザー辞書を構築する。(Blueprint公開ノード)
	 * @param [in] DictPath 読み込む辞書ファイルのパス
	 * @returns 初期化が成功したか
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxCoreUserDictInitialize"))
	static bool UserDictInitialize(const FString& DictPath = TEXT(""));
	
	/**
	 * @brief ユーザー辞書に単語を追加する。(Blueprint公開ノード)
	 *
	 * @param [in] Word 追加する単語
	 * @returns 追加した単語のUUID
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxUserDictAddWord"))
	static UPARAM(DisplayName="UUID") TArray<uint8> UserDictAddWord(const FVoicevoxCoreUserDictWord Word);

	/**
	 * @brief ユーザー辞書の単語を更新する。(Blueprint公開ノード)
	 *
	 * @param [in] WordUuid 更新する単語のUUID
	 * @param [in] Word 新しい単語のデータ
	 * @returns 結果
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxRewriteUserDictWord"))
	static bool RewriteUserDictWord(const TArray<uint8>& WordUuid, const FVoicevoxCoreUserDictWord Word);

	/**
	 * @brief ユーザー辞書から単語を削除する。(Blueprint公開ノード)
	 *
	 * @param [in] WordUuid 削除する単語のUUID
	 * @returns 結果
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxDeleteUserDictWord"))
	static bool DeleteUserDictWord(const TArray<uint8>& WordUuid);
	
	/**
	 * @brief ユーザー辞書の単語をJSON形式で出力する。(Blueprint公開ノード)
	 * @returns output_json
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxUserDictWord"))
	static FString GetUserDictWord();

	/**
	 * @brief ユーザー辞書をファイルに保存する。(Blueprint公開ノード)
	 * @param [in] Path 保存先のファイルパス
	 * @returns 結果
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxCoreUserDictSave"))
	static bool UserDictSave(const FString& Path);
	
	/**
	 * @brief ユーザー辞書を破棄する。(Blueprint公開ノード)
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxCoreUserDictDelete"))
    static void UserDictDelete();
};
