// Copyright Yuuki Ogino. All Rights Reserved

#include "AudioQueryEditorElementWidget.h"
#include "Kismet/KismetTextLibrary.h"

/**
 * @breif 編集したいAudioQueryを参照でセット
 */
void UAudioQueryEditorElementWidget::SetEditAudioQuery(FVoicevoxAudioQuery& Param)
{
	EditorAudioQueryPtr = &Param;

	ParamEditableText->SetText(GetParamTextValue());
	ParamSlider->SetValue(GetParamValue());
}

/**
 * @brief NativeConstruct override
 */
void UAudioQueryEditorElementWidget::NativeConstruct()
{
	if (ParamSlider)
	{
		ParamSlider->OnValueChanged.AddDynamic(this, &UAudioQueryEditorElementWidget::OnValueChanged);

		switch (ParamType)
		{
		case EEditParamType::SpeedScale:
			ParamSlider->SetMinValue(0.5f);
			ParamSlider->SetMaxValue(2.0f);
			break;
		case EEditParamType::PitchScale:
			ParamSlider->SetMinValue(-0.15f);
			ParamSlider->SetMaxValue(0.15f);
			break;
		case EEditParamType::IntonationScale:
			ParamSlider->SetMinValue(0);
			ParamSlider->SetMaxValue(2.0f);
			break;
		case EEditParamType::VolumeScale:
			ParamSlider->SetMinValue(0);
			ParamSlider->SetMaxValue(2.0f);
			break;
		case EEditParamType::PrePhonemeLength:
			ParamSlider->SetMinValue(0);
			ParamSlider->SetMaxValue(1.5f);
			break;
		case EEditParamType::PostPhonemeLength:
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
		ParamEditableText->OnTextCommitted.AddDynamic(this, &UAudioQueryEditorElementWidget::OnTextCommitted);
	}
}

/**
 * @brief スライダーの値が編集された際に呼ばれるデリゲート関数
 */
void UAudioQueryEditorElementWidget::OnValueChanged(const float Value)
{
	ParamEditableText->SetText(UKismetTextLibrary::Conv_FloatToText(Value, HalfFromZero, false, true, 1, 324, 2, 2));
	
	if (!EditorAudioQueryPtr) return;
	
	switch (ParamType)
	{
	case EEditParamType::SpeedScale:
		EditorAudioQueryPtr->Speed_scale = Value;
		break;
	case EEditParamType::PitchScale:
		EditorAudioQueryPtr->Pitch_scale = Value;
		break;
	case EEditParamType::IntonationScale:
		EditorAudioQueryPtr->Intonation_scale = Value;
		break;
	case EEditParamType::VolumeScale:
		EditorAudioQueryPtr->Volume_scale = Value;
		break;
	case EEditParamType::PrePhonemeLength:
		EditorAudioQueryPtr->Pre_phoneme_length = Value;
		break;
	case EEditParamType::PostPhonemeLength:
		EditorAudioQueryPtr->Post_phoneme_length = Value;
		break;
	default:
		break;
	}
}

/**
 * @brief テキストの値が編集された際に呼ばれるデリゲート関数
 */
void UAudioQueryEditorElementWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	const float Value = FMath::Clamp(FCString::Atof(*Text.ToString()), ParamSlider->MinValue, ParamSlider->MaxValue);
	ParamSlider->SetValue(Value);
	ParamEditableText->SetText(UKismetTextLibrary::Conv_FloatToText(Value, HalfFromZero, false, true, 1, 324, 2, 2));

	if (!EditorAudioQueryPtr) return;

	switch (ParamType)
	{
	case EEditParamType::SpeedScale:
		EditorAudioQueryPtr->Speed_scale = Value;
		break;
	case EEditParamType::PitchScale:
		EditorAudioQueryPtr->Pitch_scale = Value;
		break;
	case EEditParamType::IntonationScale:
		EditorAudioQueryPtr->Intonation_scale = Value;
		break;
	case EEditParamType::VolumeScale:
		EditorAudioQueryPtr->Volume_scale = Value;
		break;
	case EEditParamType::PrePhonemeLength:
		EditorAudioQueryPtr->Pre_phoneme_length = Value;
		break;
	case EEditParamType::PostPhonemeLength:
		EditorAudioQueryPtr->Post_phoneme_length = Value;
		break;
	default:
		break;
	}
}

/**
 * @brief パラメータ値を取得
 */
float UAudioQueryEditorElementWidget::GetParamValue() const
{
	if (!ParamEditableText) return 0;
	if (!EditorAudioQueryPtr) return ParamSlider->MinValue;
	
	switch (ParamType)
	{
	case EEditParamType::SpeedScale:
		return EditorAudioQueryPtr->Speed_scale;
	case EEditParamType::PitchScale:
		return EditorAudioQueryPtr->Pitch_scale;
	case EEditParamType::IntonationScale:
		return EditorAudioQueryPtr->Intonation_scale, HalfFromZero;
	case EEditParamType::VolumeScale:
		return EditorAudioQueryPtr->Volume_scale;
	case EEditParamType::PrePhonemeLength:
		return EditorAudioQueryPtr->Pre_phoneme_length;
	case EEditParamType::PostPhonemeLength:
		return EditorAudioQueryPtr->Post_phoneme_length;
	default:
		return 0;
	}
}

/**
 * @brief パラメータ値を取得
 */
FText UAudioQueryEditorElementWidget::GetParamTextValue() const
{
	return UKismetTextLibrary::Conv_FloatToText(GetParamValue(), HalfFromZero, false, true, 1, 324, 2, 2);
}
