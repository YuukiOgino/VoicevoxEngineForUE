// Copyright Yuuki Ogino. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxCoreUtil.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "AudioQueryEditorElementWidget.generated.h"

//------------------------------------------------------------------------
// enum
//------------------------------------------------------------------------

/**
 * @enum EEditParamType
 * @brief このスライダーで変更するパラメータタイプ
 */
UENUM(BlueprintType)
enum class EEditParamType : uint8
{
	SpeedScale,
	PitchScale,
	IntonationScale,
	VolumeScale,
	PrePhonemeLength,
	PostPhonemeLength,
};

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class UAudioQueryEditorElementWidget
 * @brief AudioQueryの共通項目を編集するエレメントクラス
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API UAudioQueryEditorElementWidget : public UUserWidget
{
	GENERATED_BODY()

	//! VOICEVOX COREから生成したAudioQuery。元データのポインタを保持
	FVoicevoxAudioQuery* EditorAudioQueryPtr = nullptr;
	
public:

	//! 編集するプロパティのタイプ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEditParamType ParamType;

	/**
	 * @breif 編集したいAudioQueryを参照でセット
	 * @param [in] Param : VOICEVOX COREから生成したAudioQuery
	 */
	UFUNCTION(BlueprintCallable)
	void SetEditAudioQuery( UPARAM(ref) FVoicevoxAudioQuery& Param);
	
protected:

	//! 値編集テキスト
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UEditableText* ParamEditableText;

	//! 項目名
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* Label;

	//! 値編集スライダー
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	USlider* ParamSlider;

	/**
	 * @brief NativeConstruct override
	 */
	virtual void NativeConstruct() override;

	/**
	 * @brief スライダーの値が編集された際に呼ばれるデリゲート関数
	 * @param Value 
	 */
	UFUNCTION()
	void OnValueChanged(float Value);

	/**
	 * @brief テキストの値が編集された際に呼ばれるデリゲート関数
	 * @param Text
	 * @param CommitMethod
	 */
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	/**
	 * @brief パラメータ値を取得
	 * @return ParamTypeで指定したパラメータ
	 */
	UFUNCTION()
	float GetParamValue() const;

	/**
	 * @brief パラメータ値を取得
	 * @return ParamTypeで指定したパラメータ（FText）
	 */
	UFUNCTION()
	FText GetParamTextValue() const;
};
