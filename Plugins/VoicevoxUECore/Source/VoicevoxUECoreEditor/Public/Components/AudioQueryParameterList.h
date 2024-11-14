// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile AudioQueryParameterList.h
 * @brief  AudioQueryのパラメータ値ウィジェットヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "AudioQueryParameterElement.h"
#include "Blueprint/UserWidget.h"
#include "AudioQueryParameterList.generated.h"

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class UAudioQueryParameterList
 * @brief AudioQueryパラメータリスト編集ウィジェットクラス
 */
UCLASS()
class VOICEVOXUECOREEDITOR_API UAudioQueryParameterList : public UUserWidget
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

	//! 話速編集エレメント
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryParameterElement* SpeedScale;

	//! 音高編集エレメント
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryParameterElement* PitchScale;

	//! 抑揚編集エレメント
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryParameterElement* IntonationScale;

	//! 音量編集エレメント
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryParameterElement* VolumeScale;

	//! 開始無音編集エレメント
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryParameterElement* PrePhonemeLength;

	//! 終了無音編集エレメント
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UAudioQueryParameterElement* PostPhonemeLength;

	/**
	 * @brief NativeConstruct override
	 */
	virtual void NativeConstruct() override;	
};
