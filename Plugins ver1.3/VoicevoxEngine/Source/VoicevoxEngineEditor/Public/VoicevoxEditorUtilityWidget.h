// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @headerfile VoicevoxEditorUtilityWidget.h
 * @brief  VOICEVOX編集Editor Utility Widgetヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "VoicevoxQuery.h"
#include "VoicevoxEditorUtilityWidget.generated.h"

class AActor;
class UEditorPerProjectUserSettings;

//------------------------------------------------------------------------
// DELEGATE
//------------------------------------------------------------------------

//! AudioQueryアセットデータ読込成功デリゲートクラス
DECLARE_DELEGATE_OneParam(FOnAudioQueryPickingChosen, const TArray<FAssetData>&);

//! AudioQueryアセットデータ読込キャンセルデリゲートクラス
DECLARE_DELEGATE(FOnAudioQueryPickingCancelled);

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class UVoicevoxEditorUtilityWidget
 * @brief VOICEVOX編集Editor Utility Widget
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API UVoicevoxEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:

	//----------------------------------------------------------------
	// Variable
	//----------------------------------------------------------------
	
	//! 編集するAudioQuery
	UPROPERTY(Category=VOICEVOX, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	FVoicevoxAudioQuery EditorAudioQueryPtr;

	//----------------------------------------------------------------
	// Function
	//----------------------------------------------------------------

	/**
	 * @brief 編集対象のAudioQueryがセット済みか（Blueprint公開ノード）
	 * @return Trueならセット済み（AudioQuery編集可能）
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "IsSetEditorAudioQuery"))
	bool IsSetEditorAudioQuery() const;

	/**
	 * @brief 編集したAudioQueryをアセットデータに保存する（Blueprint公開ノード）
	 * @param [in] SpeakerType : スピーカータイプ
	 * @param [in] Text : 入力したテキスト
	 * @param [in] Yomikata : 読み方
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "SaveAudioQuearyAssets"))
	void SaveAudioQueryAssets(int64 SpeakerType, FString Text, FString Yomikata) const;

	/**
	 * @brief 編集したAudioQueryからSoundWaveアセットを生成して保存する（Blueprint公開ノード）
	 * @param [in] SpeakerType : スピーカータイプ
	 * @param [in] bEnableInterrogativeUpspeak 疑問文の調整を有効にする
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "SaveSoundWaveAssets"))
	void SaveSoundWaveAssets(int64 SpeakerType, bool bEnableInterrogativeUpspeak = true) const;

	/**
	 * @brief 編集したAudioQueryからWavファイルを生成して保存する（Blueprint公開ノード）
	 * @param [in] SpeakerType : スピーカータイプ
	 * @param [in] bEnableInterrogativeUpspeak : 疑問文の調整を有効にする
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "SaveWavFile"))
	void SaveWavFile(int64 SpeakerType, bool bEnableInterrogativeUpspeak = true) const;

	/**
	 * @brief AudioQueryアセットデータを読み込む（Blueprint公開ノード）
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "LoadAudioQuearyAssets"))
	void LoadAudioQueryAssets();

	/**
	 * @brief AudioQueryアセットデータ読込完了（Blueprint Eventノード）
	 * @param [in] SpeakerType : スピーカータイプ
	 * @param [in] Text : 入力したテキスト
	 * @param [in] Yomikata : 読み方
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnLoadAudioQuery(int64 SpeakerType, const FString& Text, const FString& Yomikata);
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxEditor, Log, All);
