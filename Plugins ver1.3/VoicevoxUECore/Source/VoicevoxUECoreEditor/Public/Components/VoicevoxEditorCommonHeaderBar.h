// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxEditorCommonHeaderBar.h
 * @brief  VOICEVOX編集エディターのヘッダーバーをまとめたヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "VoicevoxEditorCommonHeaderBar.generated.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class UVoicevoxEditorCommonHeaderBar
 * @brief VOICEVOX編集エディターのヘッダーバークラス
 */
UCLASS()
class VOICEVOXUECOREEDITOR_API UVoicevoxEditorCommonHeaderBar : public UUserWidget
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

 /**
  * @brief WavFile保存ボタンクリック（イベントハンドラー）
  */
 UFUNCTION()
 void OnWaveFileSaveButtonClick();
	
public:

 DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAudioQuerySaveDelegate);
 DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAudioQueryLoadDelegate);
 DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSoundWaveSaveDelegate);
 DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWavSaveDelegate);

 //! AudioQuery保存実行イベントディスパッチャー
 UPROPERTY(BlueprintAssignable, Category="VOICEVOX Editor")
 FAudioQuerySaveDelegate OnAudioQuerySave;

 //! AudioQuery読み込み実行イベントディスパッチャー
 UPROPERTY(BlueprintAssignable, Category="VOICEVOX Editor")
 FAudioQueryLoadDelegate OnAudioQueryLoad;

 //! SoundWave保存実行イベントディスパッチャー
 UPROPERTY(BlueprintAssignable, Category="VOICEVOX Editor")
 FSoundWaveSaveDelegate OnSoundWaveSave;

 //! WavFile保存実行イベントディスパッチャー
 UPROPERTY(BlueprintAssignable, Category="VOICEVOX Editor")
 FWavSaveDelegate OnWavFileSave;
	
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

 //! WavFile保存ボタン
 UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
 UButton* WavSaveButton;

 /**
 * @brief NativeDestruct override
 */
 virtual void NativeDestruct() override;
 
 /**
  * @brief NativeConstruct override
  */
 virtual void NativeConstruct() override;
};
