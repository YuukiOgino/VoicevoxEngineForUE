// Copyright Yuuki Ogino. All Rights Reserved.

#include "Components/VoicevoxEditorCommonHeaderBar.h"

/**
* @brief NativeDestruct override
*/
void UVoicevoxEditorCommonHeaderBar::NativeDestruct()
{
	OnAudioQuerySave.RemoveAll(this);
	OnAudioQueryLoad.RemoveAll(this);
	OnSoundWaveSave.RemoveAll(this);
	OnWavFileSave.RemoveAll(this);
}

/**
 * @brief NativeConstruct override
 */
void UVoicevoxEditorCommonHeaderBar::NativeConstruct()
{
	if (AudioQuerySaveButton)
	{
		AudioQuerySaveButton->OnClicked.AddDynamic(this, &UVoicevoxEditorCommonHeaderBar::OnAudioQuerySaveButtonClick);
	}

	if (AudioQueryLoadButton)
	{
		AudioQueryLoadButton->OnClicked.AddDynamic(this, &UVoicevoxEditorCommonHeaderBar::OnAudioQueryLoadButtonClick);
	}

	if (SoundWaveSaveButton)
	{
		SoundWaveSaveButton->OnClicked.AddDynamic(this, &UVoicevoxEditorCommonHeaderBar::OnSoundWaveSaveButtonClick);
	}
	
	if (WavSaveButton)
	{
		WavSaveButton->OnClicked.AddDynamic(this, &UVoicevoxEditorCommonHeaderBar::OnWaveFileSaveButtonClick);
	}
}

void UVoicevoxEditorCommonHeaderBar::OnAudioQuerySaveButtonClick()
{
	OnAudioQuerySave.Broadcast();
}

void UVoicevoxEditorCommonHeaderBar::OnAudioQueryLoadButtonClick()
{
	OnAudioQueryLoad.Broadcast();
}

void UVoicevoxEditorCommonHeaderBar::OnSoundWaveSaveButtonClick()
{
	OnSoundWaveSave.Broadcast();
}

void UVoicevoxEditorCommonHeaderBar::OnWaveFileSaveButtonClick()
{
	OnWavFileSave.Broadcast();
}