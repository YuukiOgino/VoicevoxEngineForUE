// Copyright Yuuki Ogino. All Rights Reserved.

#include "Components/AudioCellElement.h"
#include "Kismet/KismetStringLibrary.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"

void UAudioCellElement::SetSpeakerType(const int64 SpeakerType)
{
	Speaker = SpeakerType;
	if (!IsLoadData)
	{
		OnAudioQueryChanged.Broadcast(GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAudioQuery(Speaker, AudioEditableText->GetText().ToString(), false));
	}
	else
	{
		IsLoadData = false;	
	}
}

void UAudioCellElement::SetLoadData(const int64 SpeakerType, const FString& Text)
{
	IsLoadData = true;
	SpeakerComboBox->SetSelectedOption(UKismetStringLibrary::Conv_StringToName(UKismetStringLibrary::Conv_Int64ToString(SpeakerType)));
	AudioEditableText->SetText(FText::FromString(Text));
}

/**
 * @brief NativeConstruct override
 */
void UAudioCellElement::NativeConstruct()
{

	if (AudioEditableText)
	{
		AudioEditableText->OnTextCommitted.AddUniqueDynamic(this, &UAudioCellElement::OnTextCommitted);
	}

	if (DeleteButton)
	{
		// 一つだけなら削除ボタンは不要のため非表示（２つ以上表示する予定があれば判定を入れるかも）
		DeleteButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UAudioCellElement::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	OnAudioQueryChanged.Broadcast(GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAudioQuery(Speaker, Text.ToString(), false));
}