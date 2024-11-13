// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxQuery.h
 * @brief  Voicevox関連のデータを保存するための情報をまとめたヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "VoicevoxUEDefined.h"
#include "VoicevoxQuery.generated.h"

/**
 * @classs UVoicevoxQuery
 * @brief Voicevoxで生成されるクエリ情報を保存するためのクラス
 */
UCLASS(HideCategories=Object, BlueprintType, EditInlineNew)
class VOICEVOXUECORE_API UVoicevoxQuery : public UObject
{
	GENERATED_BODY()
public:
	//! AudioQuery
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX CORE")
	FVoicevoxAudioQuery VoicevoxAudioQuery;

	//! 話者番号
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX CORE")
	int64 SpeakerType = 3;

	//! 入力したテキスト
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX CORE")
	FString Text;
};
