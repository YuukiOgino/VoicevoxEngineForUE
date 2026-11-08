// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxSingFrameQuery.h
 * @brief  Voicevoxのソング機能で使用するフレームごとの音声合成用のクエリデータを保存するための情報をまとめたヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "VoicevoxUEDefined.h"
#include "UObject/Object.h"
#include "VoicevoxSingFrameQuery.generated.h"

/**
 * @classs UVoicevoxSingFrameQuery
 * @brief Voicevoxのソング機能で使用するフレームごとの音声合成用のクエリ情報を保存するためのクラス
 */
UCLASS()
class VOICEVOXUECORE_API UVoicevoxSingFrameQuery : public UObject
{
	GENERATED_BODY()
public:
	//! FrameAudioQuery
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX CORE")
	FVoicevoxFrameAudioQuery FrameAudioQuery;

	//! スタイルID
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX CORE")
	int32 StyleId = 6000;
};
