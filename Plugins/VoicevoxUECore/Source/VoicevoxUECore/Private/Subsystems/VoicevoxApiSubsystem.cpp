// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief VOICEVOX COREのAPIを実行する基礎Subsystem CPPファイル
 * @author Yuuki Ogino
 */

#include "Subsystems/VoicevoxApiSubsystem.h"

#include "JsonObjectConverter.h"

DEFINE_LOG_CATEGORY(LogVoicevoxApi);

/**
 * @brief VOICEVOXから受信したエラーメッセージを表示
 * @param [in] MessageFormat : エラーメッセージのフォーマット
 */
void UVoicevoxApiSubsystem::ShowVoicevoxErrorMessage(const FString& MessageFormat)
{
	UE_LOG(LogVoicevoxApi, Error, TEXT("%s"), *MessageFormat);
	const FColor Col = FColor::Red;
	const FVector2D Scl = FVector2D(1.0f, 1.0f);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, Col, *MessageFormat, true, Scl);
}

/**
 * @brief VOICEVOXから受信したAccentPhraseの無名配列のJSONを変換する
 */
TArray<FVoicevoxAccentPhrase> UVoicevoxApiSubsystem::JsonObjectConverterToAccentPhrase(const FString& JsonString)
{
	TArray<FVoicevoxAccentPhrase> AccentPhrases;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (TSharedPtr<FJsonValue> JsonValue; FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
	{
		if (const TArray<TSharedPtr<FJsonValue>>* JsonArray = nullptr; JsonValue->TryGetArray(JsonArray))
		{
			for (const TSharedPtr<FJsonValue>& Element : *JsonArray)
			{
				FVoicevoxAccentPhrase AccentPhrase;
				if (FJsonObjectConverter::JsonObjectToUStruct(Element->AsObject().ToSharedRef(), FVoicevoxAccentPhrase::StaticStruct(), &AccentPhrase))
				{
					AccentPhrases.Add(AccentPhrase);
				}
			}
		}
	}
	return AccentPhrases;
}