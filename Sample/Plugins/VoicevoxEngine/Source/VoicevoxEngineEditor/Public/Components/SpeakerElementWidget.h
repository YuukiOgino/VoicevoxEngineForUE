// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxCoreUtil.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SpeakerElementWidget.generated.h"

/**
 * @class USpeakerElementWidget
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API USpeakerElementWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true));
	EVoicevoxSpeakerType Speaker;
	
protected:

	//! スピーカー名
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* SpeakerLabel;

	/**
	 * @brief NativeConstruct override
	 */
	virtual void NativeConstruct() override;
};
