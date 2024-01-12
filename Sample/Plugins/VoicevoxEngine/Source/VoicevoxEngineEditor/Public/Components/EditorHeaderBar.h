// Copyright Yuuki Ogino. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "EditorHeaderBar.generated.h"

/**
 * @class UEditorHeaderBar
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API UEditorHeaderBar : public UUserWidget
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAudioQuerySaveDelegate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAudioQueryLoadDelegate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSoundWaveSaveDelegate);
	
	UPROPERTY(BlueprintAssignable, Category="VOICEVOX Editor")
	FAudioQuerySaveDelegate OnAudioQuerySave;

	UPROPERTY(BlueprintAssignable, Category="VOICEVOX Editor")
	FAudioQueryLoadDelegate OnAudioQueryLoad;

	UPROPERTY(BlueprintAssignable, Category="VOICEVOX Editor")
	FSoundWaveSaveDelegate OnSoundWaveSave;
	
protected:

	//! AudioQuery保存ボタン
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* AudioQuerySaveButton;

	//! AudioQuery読込ボタン
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* AudioQueryLoadButton;
	
	//! SoundWave保存ボタン
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* SoundWaveSaveButton;

	/**
	 * @brief NativeConstruct override
	 */
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnAudioQuerySaveButtonClick();

	UFUNCTION()
	void OnAudioQueryLoadButtonClick();

	UFUNCTION()
	void OnSoundWaveSaveButtonClick();
};
