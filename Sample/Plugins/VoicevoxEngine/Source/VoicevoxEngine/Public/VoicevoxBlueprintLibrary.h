// Copyright Yuuki Ogino. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VoicevoxCoreUtil.h"
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
	 *  VOICEVOX COREメタ情報を取得する(Blueprint公開ノード)
	 * @brief 話者名や話者IDのリストを取得する(Blueprint公開ノード)
	 * @return メタ情報が格納されたjson形式の構造体
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxMetas"))
	static void GetMetasToList(TArray<FVoicevoxMeta>& Metas);

	/**
	 * @brief サポートデバイス情報を取得する(Blueprint公開ノード)
	 * @return サポートデバイス情報の構造体
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxSupportedDevices"))
	static void GetSupportedDevices(FVoicevoxSupportedDevices& SupportedDevices);

	/**
	 * @fn
	 *  VOICEVOX COREで変換したAudioQueryを取得する(Blueprint公開ノード)
	 * @param[out] AudioQuery	AudioQuery構造体
	 * @param[in] SpeakerType	話者番号
	 * @param[in] Message		音声データに変換するtextデータ
	 * @param[in] bRunKana		AquesTalkライクな記法で実行するか
	 * @return AudioQuery情報が格納されたjson形式の構造体
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "GetVoicevoxAudioQuery"))
	static void GetAudioQuery(FVoicevoxAudioQuery& AudioQuery, ESpeakerType SpeakerType, FString Message, bool bRunKana=false);

	UFUNCTION(BlueprintCallable, Category="VOICEVOX Engine", meta=(Keywords="voicevox", DisplayName = "SimplePlayTextToAudioQueryStruct", WorldContext="WorldContextObject"))
	static void SimplePlayTextToAudioQueryStruct(UObject* WorldContextObject, FVoicevoxAudioQuery AudioQuery, ESpeakerType SpeakerType, bool bRunKana=false);

	/**
	 * @brief 生成した音声データからUSoundWaveを作成
	 * @param[in] PCMData  音声データ
	 * @return 合成音声を格納したUSoundWave
	 */
	static USoundWave* CreateSoundWave(TArray<uint8> PCMData);
};
