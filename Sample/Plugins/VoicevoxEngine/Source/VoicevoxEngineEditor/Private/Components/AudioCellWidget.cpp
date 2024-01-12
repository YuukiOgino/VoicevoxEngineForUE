// Copyright Yuuki Ogino. All Rights Reserved

#include "Components/AudioCellWidget.h"

#include "Kismet/KismetStringLibrary.h"

void UAudioCellWidget::SetSpeakerType(const int64 SpeakerType)
{
	Speaker = SpeakerType;
	if (!IsLoadData)
	{
		OnAudioQueryChanged.Broadcast(FVoicevoxCoreUtil::GetAudioQuery(Speaker, AudioEditableText->GetText().ToString(), false));
	}
	else
	{
		IsLoadData = false;	
	}
}

void UAudioCellWidget::SetLoadData(const int64 SpeakerType, const FString& Text)
{
	IsLoadData = true;
	SpeakerComboBox->SetSelectedOption(UKismetStringLibrary::Conv_StringToName(UKismetStringLibrary::Conv_Int64ToString(SpeakerType)));
	AudioEditableText->SetText(FText::FromString(Text));
}

/**
 * @brief NativeConstruct override
 */
void UAudioCellWidget::NativeConstruct()
{

	if (AudioEditableText)
	{
		AudioEditableText->OnTextCommitted.AddUniqueDynamic(this, &UAudioCellWidget::OnTextCommitted);
	}

	if (DeleteButton)
	{
		// 一つだけなら削除ボタンは不要のため非表示（２つ以上表示する予定があれば判定を入れるかも）
		DeleteButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UAudioCellWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	OnAudioQueryChanged.Broadcast(FVoicevoxCoreUtil::GetAudioQuery(Speaker, Text.ToString(), false));
}