// Copyright Yuuki Ogino. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioQueryEditorElementWidget.h"
#include "AudioQueryEditorWidget.generated.h"

/**
 * @class UAudioQueryEditorWidget
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API UAudioQueryEditorWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * @breif 編集したいAudioQueryを参照でセット
	 * @param [in] Param : VOICEVOX COREから生成したAudioQuery
	 */
	UFUNCTION(BlueprintCallable)
	void SetEditAudioQuery(UPARAM(ref) FVoicevoxAudioQuery& Param);
	
protected:

	//! 
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryEditorElementWidget* SpeedScale;

	//! 
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryEditorElementWidget* PitchScale;

	//! 
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryEditorElementWidget* IntonationScale;

	//! 
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryEditorElementWidget* VolumeScale;

	//! 
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryEditorElementWidget* PrePhonemeLength;

	//! 
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryEditorElementWidget* PostPhonemeLength;

	/**
	 * @brief NativeConstruct override
	 */
	virtual void NativeConstruct() override;	
};
