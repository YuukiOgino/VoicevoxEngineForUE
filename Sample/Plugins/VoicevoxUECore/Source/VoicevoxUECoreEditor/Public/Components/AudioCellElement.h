// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @headerfile AudioCellElement.h
 * @brief  合成音声テキスト編集ウィジェットヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/ComboBoxKey.h"
#include "Components/EditableText.h"
#include "VoicevoxUEDefined.h"
#include "AudioCellElement.generated.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class UAudioCellElement
 * @brief 合成音声テキスト編集ウィジェット
 */
UCLASS()
class VOICEVOXUECOREEDITOR_API UAudioCellElement : public UUserWidget
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
      * @param [in] SpeakerType : 話者番号
      */
     UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "SetSpeakerType"))
     void SetSpeakerType(int64 SpeakerType);

     /**
      * @brief アセットからロードしたデータをセットする
      * @param [in] SpeakerType : 話者番号
      * @param [in] Text : 入力テキスト
      */
     UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "SetLoadData"))
     void SetLoadData(int64 SpeakerType, const FString& Text);

     /**
      * @brief スピーカー項目コンポーネントを作成イベント（Blueprint Event）
      */
     UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
     void OnCreateSpeakerElement();
 
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
