// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile SpeakerElement.h
 * @brief  スピーカー選択コンボボックスのラベル項目ヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SpeakerElement.generated.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class USpeakerElement
 * @brief スピーカー選択コンボボックスラベルエレメントクラス
 */
UCLASS()
class VOICEVOXUECOREEDITOR_API USpeakerElement : public UUserWidget
{
	  GENERATED_BODY()
 
public:

   //! 話者番号
   UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true));
   int64 Speaker;
	
protected:

   //! スピーカー名
   UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
   UTextBlock* SpeakerLabel;

   /**
    * @brief NativeConstruct override
    */
   virtual void NativeConstruct() override;
};
