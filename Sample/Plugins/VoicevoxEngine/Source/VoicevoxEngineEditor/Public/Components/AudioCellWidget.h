// Copyright Yuuki Ogino. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxCoreUtil.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxKey.h"
#include "Components/EditableText.h"
#include "AudioCellWidget.generated.h"

/**
 * @class UAudioCellWidget
 * @brief 音編集ウィジェット
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API UAudioCellWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY();
	ESpeakerType Speaker;
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEditableAudioQueryChangedEvent, const FVoicevoxAudioQuery&, AudioQuery);

	UPROPERTY(BlueprintAssignable, Category="VOICEVOX Editor Event", meta=(DisplayName="OnAudioQueryChanged"))
	FOnEditableAudioQueryChangedEvent OnAudioQueryChanged;
	
	UPROPERTY(Category=VOICEVOX, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	FVoicevoxAudioQuery EditorAudioQuery;

	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "SetSpeakerType"))
	void SetSpeakerType(ESpeakerType SpeakerType);
	
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

	/**
	 * @brief NativeConstruct override
	 */
	virtual void NativeConstruct() override;
	
	/**
	 * @brief テキストの値が編集された際に呼ばれるデリゲート関数
	 * @param Text
	 * @param CommitMethod
	 */
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};