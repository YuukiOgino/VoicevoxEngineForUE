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
	 * @param[in] SpeakerType						話者番号
	 * @param[in] Message							音声データに変換するtextデータ
	 * @param[in] bRunKana							AquesTalkライクな記法で実行するか
	 * @param[in] bEnableInterrogativeUpspeak		疑問文の調整を有効にする
	 * @return TextToSpeechで作成された音楽データが格納されたUSoundWave
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxTextToSpeechOutput"))
	static UPARAM(DisplayName="Sound") USoundWave* TextToSpeechOutput(int SpeakerType, FString Message, bool bRunKana = false, bool bEnableInterrogativeUpspeak = true);
	
	/**
	 * @brief 入力したテキストをVOICEVOX COREでAudioQueryに変換後、SoundWaveを生成(Blueprint公開ノード)
	 * @param[in] SpeakerType						話者番号
	 * @param[in] Message							音声データに変換するtextデータ
	 * @param[in] bRunKana							AquesTalkライクな記法で実行するか
	 * @param[in] bEnableInterrogativeUpspeak		疑問文の調整を有効にする
	 * @return AudioQuery情報を元に作成された音楽データが格納されたUSoundWave
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxToTextAudioQueryOutput"))
	static UPARAM(DisplayName="Sound") USoundWave* TextToAudioQueryOutput(int SpeakerType, FString Message, bool bRunKana = false, bool bEnableInterrogativeUpspeak = true);
	
	/**
	 * @fn
	 *  VOICEVOX COREで変換したAudioQueryを取得する(Blueprint公開ノード)
	 * @param[in] SpeakerType	話者番号
	 * @param[in] Message		音声データに変換するtextデータ
	 * @param[in] bRunKana		AquesTalkライクな記法で実行するか
	 * @return AudioQuery情報が格納されたjson形式の構造体
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxAudioQuery"))
	static UPARAM(DisplayName="AudioQuery") FVoicevoxAudioQuery GetAudioQuery(int SpeakerType, FString Message, bool bRunKana = false);

	/**
	 * @brief VOICEVOX COREで取得したAudioQuery元にSoundWaveを作成(Blueprint公開ノード)
	 * @param[in] AudioQuery						AudioQuery構造体
	 * @param[in] SpeakerType						話者番号
	 * @param[in] bEnableInterrogativeUpspeak		疑問文の調整を有効にする
	 * @return AudioQuery情報を元に作成された音楽データが格納されたUSoundWave
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "VoicevoxAudioQueryOutput"))
	static UPARAM(DisplayName="Sound") USoundWave* AudioQueryOutput(FVoicevoxAudioQuery AudioQuery, int SpeakerType, bool bEnableInterrogativeUpspeak = true);

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
};
