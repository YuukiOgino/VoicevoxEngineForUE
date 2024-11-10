// Copyright Yuuki Ogino. All Rights Reserved.

#include "Components/SpeakerElement.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"

/**
 * @brief NativeConstruct override
 */
void USpeakerElement::NativeConstruct()
{
	if (SpeakerLabel)
	{
		SpeakerLabel->SetText(FText::AsCultureInvariant(GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetMetaName(Speaker)));
	}
}