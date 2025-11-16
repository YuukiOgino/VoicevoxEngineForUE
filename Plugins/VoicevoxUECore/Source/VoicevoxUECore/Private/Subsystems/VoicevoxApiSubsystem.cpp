// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief VOICEVOX COREのAPIを実行する基礎Subsystem CPPファイル
 * @author Yuuki Ogino
 */

#include "Subsystems/VoicevoxApiSubsystem.h"

#include "JsonObjectConverter.h"
#include "Engine/Engine.h"

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

/**
 * @brief AccentPhraseの配列を無名配列のJSONに変換する
 */
FString UVoicevoxApiSubsystem::AccentPhraseConverterToJsonString(TArray<FVoicevoxAccentPhrase> AccentPhrases)
{
	// JSON値配列を作成
	TArray<TSharedPtr<FJsonValue>> JsonArray;

	for (const auto& [Moras, Accent, Pause_mora, Is_interrogative] : AccentPhrases)
	{
		TSharedPtr<FJsonObject> Obj = MakeShared<FJsonObject>();
		TArray<TSharedPtr<FJsonValue>> MoraJsonArray;

		for (const FVoicevoxMora& Mora : Moras)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
			FJsonObjectConverter::UStructToJsonObject(FVoicevoxMora::StaticStruct(), &Mora, JsonObject.ToSharedRef(), 0, 0);
			MoraJsonArray.Add(MakeShared<FJsonValueObject>(JsonObject));
		}
		Obj->SetArrayField(TEXT("moras"), MoraJsonArray);
		Obj->SetNumberField(TEXT("accent"), Accent);

		if (Pause_mora.Text.IsEmpty())
		{
			Obj->SetObjectField(TEXT("pause_mora"), nullptr);
		}
		else
		{
			// pause_moraは子音のパラメータ（consonant、consonant_length）にnull以外が入るとインデックス範囲外エラーが起きるため、UStructToJsonObjectは使用しない
			TSharedPtr<FJsonObject> StructObject = MakeShared<FJsonObject>();
			StructObject->SetStringField(TEXT("text"),  Pause_mora.Text);
			StructObject->SetObjectField(TEXT("consonant"),  nullptr);
			StructObject->SetObjectField(TEXT("consonant_length"),  nullptr);
			StructObject->SetStringField(TEXT("vowel"),  Pause_mora.Vowel);
			StructObject->SetNumberField(TEXT("vowel_length"),  0);
			StructObject->SetNumberField(TEXT("pitch"),  0);
				
			Obj->SetObjectField(TEXT("pause_mora"), StructObject);
		}
			
		Obj->SetBoolField(TEXT("is_interrogative"), Is_interrogative);
		JsonArray.Add(MakeShared<FJsonValueObject>(Obj));
	}

	// 配列を書き出し
	FString OutputString;
	const auto Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonArray, Writer);

	return OutputString;
}