// Copyright Yuuki Ogino. All Rights Reserved

/**
 * @headerfile AudioQueryParameterElement.cpp
 * @brief  AudioQueryのパラメータ値編集エレメントCPPファイル
 * @author Yuuki Ogino
 */

#include "Components/AudioQueryParameterElement.h"
#include "Kismet/KismetTextLibrary.h"

/**
 * @breif 編集したいAudioQueryを参照でセット
 */
void UAudioQueryParameterElement::SetEditAudioQuery(FVoicevoxAudioQuery& Param)
{
	EditorAudioQueryPtr = &Param;

	ParamEditableText->SetText(GetParamTextValue());
	ParamSlider->SetValue(GetParamValue());
}

/**
 * @brief NativeConstruct override
 */
void UAudioQueryParameterElement::NativeConstruct()
{
	if (ParamSlider)
	{
		ParamSlider->OnValueChanged.AddDynamic(this, &UAudioQueryParameterElement::OnValueChanged);

		switch (ParamType)
		{
		case EAudioQueryParameter::SpeedScale:
			ParamSlider->SetMinValue(0.5f);
			ParamSlider->SetMaxValue(2.0f);
			break;
		case EAudioQueryParameter::PitchScale:
			ParamSlider->SetMinValue(-0.15f);
			ParamSlider->SetMaxValue(0.15f);
			break;
		case EAudioQueryParameter::IntonationScale:
			ParamSlider->SetMinValue(0);
			ParamSlider->SetMaxValue(2.0f);
			break;
		case EAudioQueryParameter::VolumeScale:
			ParamSlider->SetMinValue(0);
			ParamSlider->SetMaxValue(2.0f);
			break;
		case EAudioQueryParameter::PrePhonemeLength:
			ParamSlider->SetMinValue(0);
			ParamSlider->SetMaxValue(1.5f);
			break;
		case EAudioQueryParameter::PostPhonemeLength:
			ParamSlider->SetMinValue(0);
			ParamSlider->SetMaxValue(1.5f);
			break;
		default:
			break;
		}
	}

	if (Label)
	{
		Label->SetText(FText::AsCultureInvariant(UEnum::GetDisplayValueAsText(ParamType)));
	}

	if (ParamEditableText)
	{
		ParamEditableText->SetText(GetParamTextValue());
		ParamEditableText->OnTextCommitted.AddDynamic(this, &UAudioQueryParameterElement::OnTextCommitted);
	}
}

/**
 * @brief スライダーの値が編集された際に呼ばれるデリゲート関数
 */
void UAudioQueryParameterElement::OnValueChanged(const float Value)
{
	ParamEditableText->SetText(UKismetTextLibrary::Conv_FloatToText(Value, HalfFromZero, false, true, 1, 324, 2, 2));
	
	if (!EditorAudioQueryPtr) return;
	
	switch (ParamType)
	{
	case EAudioQueryParameter::SpeedScale:
		EditorAudioQueryPtr->Speed_scale = Value;
		break;
	case EAudioQueryParameter::PitchScale:
		EditorAudioQueryPtr->Pitch_scale = Value;
		break;
	case EAudioQueryParameter::IntonationScale:
		EditorAudioQueryPtr->Intonation_scale = Value;
		break;
	case EAudioQueryParameter::VolumeScale:
		EditorAudioQueryPtr->Volume_scale = Value;
		break;
	case EAudioQueryParameter::PrePhonemeLength:
		EditorAudioQueryPtr->Pre_phoneme_length = Value;
		break;
	case EAudioQueryParameter::PostPhonemeLength:
		EditorAudioQueryPtr->Post_phoneme_length = Value;
		break;
	default:
		break;
	}
}

/**
 * @brief テキストの値が編集された際に呼ばれるデリゲート関数
 */
void UAudioQueryParameterElement::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	const float Value = FMath::Clamp(FCString::Atof(*Text.ToString()), ParamSlider->MinValue, ParamSlider->MaxValue);
	ParamSlider->SetValue(Value);
	ParamEditableText->SetText(UKismetTextLibrary::Conv_FloatToText(Value, HalfFromZero, false, true, 1, 324, 2, 2));

	if (!EditorAudioQueryPtr) return;

	switch (ParamType)
	{
	case EAudioQueryParameter::SpeedScale:
		EditorAudioQueryPtr->Speed_scale = Value;
		break;
	case EAudioQueryParameter::PitchScale:
		EditorAudioQueryPtr->Pitch_scale = Value;
		break;
	case EAudioQueryParameter::IntonationScale:
		EditorAudioQueryPtr->Intonation_scale = Value;
		break;
	case EAudioQueryParameter::VolumeScale:
		EditorAudioQueryPtr->Volume_scale = Value;
		break;
	case EAudioQueryParameter::PrePhonemeLength:
		EditorAudioQueryPtr->Pre_phoneme_length = Value;
		break;
	case EAudioQueryParameter::PostPhonemeLength:
		EditorAudioQueryPtr->Post_phoneme_length = Value;
		break;
	default:
		break;
	}
}

/**
 * @brief パラメータ値を取得
 */
float UAudioQueryParameterElement::GetParamValue() const
{
	if (!ParamEditableText) return 0;
	if (!EditorAudioQueryPtr) return ParamSlider->MinValue;
	
	switch (ParamType)
	{
	case EAudioQueryParameter::SpeedScale:
		return EditorAudioQueryPtr->Speed_scale;
	case EAudioQueryParameter::PitchScale:
		return EditorAudioQueryPtr->Pitch_scale;
	case EAudioQueryParameter::IntonationScale:
		return EditorAudioQueryPtr->Intonation_scale, HalfFromZero;
	case EAudioQueryParameter::VolumeScale:
		return EditorAudioQueryPtr->Volume_scale;
	case EAudioQueryParameter::PrePhonemeLength:
		return EditorAudioQueryPtr->Pre_phoneme_length;
	case EAudioQueryParameter::PostPhonemeLength:
		return EditorAudioQueryPtr->Post_phoneme_length;
	default:
		return 0;
	}
}

/**
 * @brief パラメータ値を取得
 */
FText UAudioQueryParameterElement::GetParamTextValue() const
{
	return UKismetTextLibrary::Conv_FloatToText(GetParamValue(), HalfFromZero, false, true, 1, 324, 2, 2);
}
