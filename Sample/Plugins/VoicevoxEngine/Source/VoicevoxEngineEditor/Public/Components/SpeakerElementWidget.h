// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile SpeakerElementWidget.h
 * @brief  スピーカー選択コンボボックスのラベル項目ヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SpeakerElementWidget.generated.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class USpeakerElementWidget
 * @brief スピーカー選択コンボボックスラベルエレメントクラス
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API USpeakerElementWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	//! スピーカータイプID
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
