// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SpeakerElementWidget.h"

/**
 * @brief NativeConstruct override
 */
void USpeakerElementWidget::NativeConstruct()
{

	if (SpeakerLabel)
	{
		SpeakerLabel->SetText(FText::AsCultureInvariant(FVoicevoxCoreUtil::GetMetaName(static_cast<int64>(Speaker))));
	}

}