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
class VOICEVOXENGINE_API UVoicevoxQuery : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	FVoicevoxAudioQuery VoicevoxAudioQuery;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	int64 SpeakerType = 3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	FString Text;
};
