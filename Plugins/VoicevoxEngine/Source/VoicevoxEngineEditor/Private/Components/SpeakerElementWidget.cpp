// Copyright Yuuki Ogino. All Rights Reserved.


#include "Components/SpeakerElementWidget.h"
#include "VoicevoxCoreUtil.h"

/**
 * @brief NativeConstruct override
 */
void USpeakerElementWidget::NativeConstruct()
{
	if (SpeakerLabel)
	{
		SpeakerLabel->SetText(FText::AsCultureInvariant(FVoicevoxCoreUtil::GetMetaName(Speaker)));
	}

}