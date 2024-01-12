// Copyright Yuuki Ogino. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioQueryEditorElementWidget.h"
#include "AudioQueryEditorWidget.generated.h"

/**
 * @class UAudioQueryEditorWidget
 * @brief AudioQueryパラメータリスト編集ウィジェットクラス
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
	void SetEditAudioQuery(UPARAM(ref) FVoicevoxAudioQuery& Param) const;
	
protected:

	//! 話速編集エレメント
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryEditorElementWidget* SpeedScale;

	//! 音高編集エレメント
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryEditorElementWidget* PitchScale;

	//! 抑揚編集エレメント
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryEditorElementWidget* IntonationScale;

	//! 音量編集エレメント
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryEditorElementWidget* VolumeScale;

	//! 開始無音編集エレメント
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryEditorElementWidget* PrePhonemeLength;

	//! 終了無音編集エレメント
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryEditorElementWidget* PostPhonemeLength;

	/**
	 * @brief NativeConstruct override
	 */
	virtual void NativeConstruct() override;	
};
