// Copyright Yuuki Ogino. All Rights Reserved

#include "Components/AudioCellWidget.h"

void UAudioCellWidget::SetSpeakerType(const ESpeakerType SpeakerType)
{
	Speaker = SpeakerType;
	EditorAudioQuery = FVoicevoxCoreUtil::GetAudioQuery(static_cast<int64>(Speaker), AudioEditableText->GetText().ToString(), false);
	OnAudioQueryChanged.Broadcast(EditorAudioQuery);
}

/**
 * @brief NativeConstruct override
 */
void UAudioCellWidget::NativeConstruct()
{

	if (AudioEditableText)
	{
		AudioEditableText->OnTextCommitted.AddDynamic(this, &UAudioCellWidget::OnTextCommitted);
	}

	if (DeleteButton)
	{
		DeleteButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UAudioCellWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	EditorAudioQuery = FVoicevoxCoreUtil::GetAudioQuery(static_cast<int64>(Speaker), Text.ToString(), false);
	OnAudioQueryChanged.Broadcast(EditorAudioQuery);
}