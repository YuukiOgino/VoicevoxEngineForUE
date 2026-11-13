// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxSingScore.h
 * @brief  Voicevoxのソング機能で使用する楽譜データを保存するための情報をまとめたヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "VoicevoxUEDefined.h"
#include "UObject/Object.h"
#include "VoicevoxSingScore.generated.h"

/**
 * @classs UVoicevoxSingScore
 * @brief Voicevoxのソング機能で使用する楽譜情報を保存するためのクラス
 */
UCLASS(HideCategories=Object, BlueprintType, EditInlineNew)
class VOICEVOXUECORE_API UVoicevoxSingScore : public UObject
{
	GENERATED_BODY()
public:
	//! 楽譜情報
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX CORE")
	FVoicevoxScore Score;
};
