// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile EditorHeaderBar.h
 * @brief  VOICEVOX編集エディターのヘッダーバーをまとめたヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "EditorHeaderBar.generated.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class UEditorHeaderBar
 * @brief VOICEVOX編集エディターのヘッダーバークラス
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API UEditorHeaderBar : public UUserWidget
{
	GENERATED_BODY()

	/**
	 * @brief AudioQueryアセット保存ボタンクリック（イベントハンドラー）
	 */
	UFUNCTION()
	void OnAudioQuerySaveButtonClick();

	/**
	 * @brief AudioQueryアセット読み込みボタンクリック（イベントハンドラー）
	 */
	UFUNCTION()
	void OnAudioQueryLoadButtonClick();

	/**
	 * @brief SoundWaveアセット保存ボタンクリック（イベントハンドラー）
	 */
	UFUNCTION()
	void OnSoundWaveSaveButtonClick();
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAudioQuerySaveDelegate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAudioQueryLoadDelegate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSoundWaveSaveDelegate);

	//! AudioQuery保存実行イベントディスパッチャー
	UPROPERTY(BlueprintAssignable, Category="VOICEVOX Editor")
	FAudioQuerySaveDelegate OnAudioQuerySave;

	//! AudioQuery読み込み実行イベントディスパッチャー
	UPROPERTY(BlueprintAssignable, Category="VOICEVOX Editor")
	FAudioQueryLoadDelegate OnAudioQueryLoad;

	//! SoundWave保存実行イベントディスパッチャー
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
};
