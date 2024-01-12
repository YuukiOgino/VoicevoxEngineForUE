// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AudioQueryEditorWidget.h"

/**
 * @brief NativeConstruct override
 */
void UAudioQueryEditorWidget::NativeConstruct()
{
	if (SpeedScale)
	{
		SpeedScale->ParamType = EEditParamType::SpeedScale;
	}

	if (PitchScale)
	{
		PitchScale->ParamType = EEditParamType::PitchScale;
	}

	if (IntonationScale)
	{
		IntonationScale->ParamType = EEditParamType::IntonationScale;
	}
	if (VolumeScale)
	{
		VolumeScale->ParamType = EEditParamType::VolumeScale;
	}
	if (PrePhonemeLength)
	{
		PrePhonemeLength->ParamType = EEditParamType::PrePhonemeLength;
	}
	if (PostPhonemeLength)
	{
		PostPhonemeLength->ParamType = EEditParamType::PostPhonemeLength;
	}
}

/**
 * @breif 編集したいAudioQueryを参照でセット
 */
void UAudioQueryEditorWidget::SetEditAudioQuery(FVoicevoxAudioQuery& Param) const
{
	SpeedScale->SetEditAudioQuery(Param);
	PitchScale->SetEditAudioQuery(Param);
	IntonationScale->SetEditAudioQuery(Param);
	VolumeScale->SetEditAudioQuery(Param);
	PrePhonemeLength->SetEditAudioQuery(Param);
	PostPhonemeLength->SetEditAudioQuery(Param);
}

