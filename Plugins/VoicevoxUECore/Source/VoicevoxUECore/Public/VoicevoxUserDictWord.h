// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxUserDictWord.h
 * @brief  Voicevoxのユーザー辞書データを保存するための情報をまとめたヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "CoreMinimal.h"
#include "VoicevoxUEDefined.h"
#include "UObject/Object.h"
#include "VoicevoxUserDictWord.generated.h"

/**
 * @classs UVoicevoxUserDictWord
 * @brief Voicevoxのユーザー辞書を保存するためのクラス
 */
UCLASS()
class VOICEVOXUECORE_API UVoicevoxUserDictWord : public UObject
{
	GENERATED_BODY()
public:
	//! UserDictWord
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX CORE")
	TArray<FVoicevoxCoreUserDictWord> UserDictWordList;
};
