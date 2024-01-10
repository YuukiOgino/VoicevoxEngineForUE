// Copyright Yuuki Ogino. All Rights Reserved.


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