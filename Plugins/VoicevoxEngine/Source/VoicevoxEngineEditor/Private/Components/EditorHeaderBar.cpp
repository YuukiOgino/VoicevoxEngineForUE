// Copyright Yuuki Ogino. All Rights Reserved.

#include "Components/EditorHeaderBar.h"

/**
 * @brief NativeConstruct override
 */
void UEditorHeaderBar::NativeConstruct()
{
	if (AudioQuerySaveButton)
	{
		AudioQuerySaveButton->OnClicked.AddDynamic(this, &UEditorHeaderBar::OnAudioQuerySaveButtonClick);
	}

	if (AudioQueryLoadButton)
	{
		AudioQueryLoadButton->OnClicked.AddDynamic(this, &UEditorHeaderBar::OnAudioQueryLoadButtonClick);
	}

	if (SoundWaveSaveButton)
	{
		SoundWaveSaveButton->OnClicked.AddDynamic(this, &UEditorHeaderBar::OnSoundWaveSaveButtonClick);
	}
	
	if (WavSaveButton)
	{
		WavSaveButton->OnClicked.AddDynamic(this, &UEditorHeaderBar::OnWaveFileSaveButtonClick);
	}
}

void UEditorHeaderBar::OnAudioQuerySaveButtonClick()
{
	OnAudioQuerySave.Broadcast();
}

void UEditorHeaderBar::OnAudioQueryLoadButtonClick()
{
	OnAudioQueryLoad.Broadcast();
}

void UEditorHeaderBar::OnSoundWaveSaveButtonClick()
{
	OnSoundWaveSave.Broadcast();
}

void UEditorHeaderBar::OnWaveFileSaveButtonClick()
{
	OnWavFileSave.Broadcast();
}