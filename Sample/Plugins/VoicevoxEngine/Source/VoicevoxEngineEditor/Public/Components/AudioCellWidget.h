// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @headerfile AudioCellWidget.h
 * @brief  合成音声テキスト編集ウィジェットヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "VoicevoxCoreUtil.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxKey.h"
#include "Components/EditableText.h"
#include "AudioCellWidget.generated.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class UAudioCellWidget
 * @brief 合成音声テキスト編集ウィジェット
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API UAudioCellWidget : public UUserWidget
{
	GENERATED_BODY()

	//! 編集中のSperkerId
	UPROPERTY();
	int64 Speaker;

	//! ロードデータ更新のための変更フラグ
	UPROPERTY();
	bool IsLoadData;
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEditableAudioQueryChangedEvent, const FVoicevoxAudioQuery&, AudioQuery);

	//! AudioQuery変更デリゲート
	UPROPERTY(BlueprintAssignable, Category="VOICEVOX Editor Event", meta=(DisplayName="OnAudioQueryChanged"))
	FOnEditableAudioQueryChangedEvent OnAudioQueryChanged;

	/**
	 * @brief SpeakerTypeをセットする
	 * @param [in] SpeakerType 
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "SetSpeakerType"))
	void SetSpeakerType(int64 SpeakerType);

	/**
	 * @brief アセットからロードしたデータをセットする
	 * @param [in] SpeakerType 
	 * @param [in] Text 
	 */
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "SetLoadData"))
	void SetLoadData(int64 SpeakerType, const FString& Text);
	
protected:

	//! スピーカーIDコンボボックス
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UComboBoxKey* SpeakerComboBox;
	
	//! 音変換編集テキスト
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UEditableText* AudioEditableText;

	//! 項目削除ボタン
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* DeleteButton;

	//! 音源再生ボタン
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* PlayButton;
	
	/**
	 * @brief NativeConstruct override
	 */
	virtual void NativeConstruct() override;
	
	/**
	 * @brief テキストの値が編集された際に呼ばれるデリゲート関数
	 * @param [in] Text
	 * @param [in] CommitMethod
	 */
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
