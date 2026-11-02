// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  スピーカー選択コンボボックスのラベル項目CPPファイル
 * @author Yuuki Ogino
 */

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