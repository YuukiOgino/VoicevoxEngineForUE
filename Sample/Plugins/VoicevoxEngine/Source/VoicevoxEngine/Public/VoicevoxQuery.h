// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxQuery.h
 * @brief  Voicevox関連のデータを保存するための情報をまとめたヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "VoicevoxCoreUtil.h"
#include "VoicevoxQuery.generated.h"

/**
 * @classs UVoicevoxQuery
 * @brief Voicevoxで生成されるクエリ情報を保存するためのクラス
 */
UCLASS()
class VOICEVOXENGINE_API UVoicevoxQuery : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	FVoicevoxAudioQuery VoicevoxAudioQuery;
};
