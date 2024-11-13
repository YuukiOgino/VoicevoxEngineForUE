// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  AudioQueryのパラメータ値ウィジェットCPPファイル
 * @author Yuuki Ogino
 */

#include "Components/AudioQueryParameterList.h"

/**
 * @brief NativeConstruct override
 */
void UAudioQueryParameterList::NativeConstruct()
{
	if (SpeedScale)
	{
		SpeedScale->ParamType = EAudioQueryParameter::SpeedScale;
	}

	if (PitchScale)
	{
		PitchScale->ParamType = EAudioQueryParameter::PitchScale;
	}

	if (IntonationScale)
	{
		IntonationScale->ParamType = EAudioQueryParameter::IntonationScale;
	}
	if (VolumeScale)
	{
		VolumeScale->ParamType = EAudioQueryParameter::VolumeScale;
	}
	if (PrePhonemeLength)
	{
		PrePhonemeLength->ParamType = EAudioQueryParameter::PrePhonemeLength;
	}
	if (PostPhonemeLength)
	{
		PostPhonemeLength->ParamType = EAudioQueryParameter::PostPhonemeLength;
	}
}

/**
 * @breif 編集したいAudioQueryを参照でセット
 */
void UAudioQueryParameterList::SetEditAudioQuery(FVoicevoxAudioQuery& Param) const
{
	SpeedScale->SetEditAudioQuery(Param);
	PitchScale->SetEditAudioQuery(Param);
	IntonationScale->SetEditAudioQuery(Param);
	VolumeScale->SetEditAudioQuery(Param);
	PrePhonemeLength->SetEditAudioQuery(Param);
	PostPhonemeLength->SetEditAudioQuery(Param);
}


