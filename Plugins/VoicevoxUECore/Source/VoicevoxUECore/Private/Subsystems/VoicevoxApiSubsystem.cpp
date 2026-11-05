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


/**
 * @brief VOICEVOXから受信したRegisteredUserDictWordの連想配列のJSONを変換する
 */
TMap<FString, FVoicevoxRegisteredUserDictWord> UVoicevoxApiSubsystem::JsonObjectConverterToRegisteredUserDictWord(const FString& JsonString)
{
	TMap<FString, FVoicevoxRegisteredUserDictWord> Result;
	TSharedPtr<FJsonObject> RootObject;
	if (const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString); !FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
	{
		UE_LOG(LogVoicevoxApi, Error, TEXT("JSON Deserialize failed"));
		return Result;
	}
	
	for (const auto& Pair : RootObject->Values)
	{
		FString Key = GetData(Pair.Key);

		TSharedPtr<FJsonObject> EntryObj = Pair.Value->AsObject();
		if (!EntryObj.IsValid()) continue;

		FVoicevoxRegisteredUserDictWord UserDict;
		if (FJsonObjectConverter::JsonObjectToUStruct(EntryObj.ToSharedRef(), FVoicevoxRegisteredUserDictWord::StaticStruct(), &UserDict))
		{
			Result.Add(Key, UserDict);
		}
	}
	
	return Result;
}

/**
 * @brief AccentPhraseの配列を無名配列のJSONに変換する
 */
FString UVoicevoxApiSubsystem::ScoreConverterToJsonString(FVoicevoxScore Score)
{
	const TSharedRef<FJsonObject> Obj = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> NoteJsonArray;
	
	for (const auto& [ID, Key, Frame_Length, Lyric] : Score.Notes)
	{
		TSharedPtr<FJsonObject> NoteObj = MakeShared<FJsonObject>();
		
		// IDにnullか文字列を入れる必要があるため、UStructToJsonObjectは使用しない
		if (ID.IsEmpty())
		{
			NoteObj->SetObjectField(TEXT("id"), nullptr);
		}
		else
		{
			NoteObj->SetStringField(TEXT("id"),  ID);
		}
		
		NoteObj->SetNumberField(TEXT("key"),  Key);
		NoteObj->SetNumberField(TEXT("frame_length"),  Frame_Length);
		NoteObj->SetStringField(TEXT("lyric"),  Lyric);
		
		NoteJsonArray.Add(MakeShared<FJsonValueObject>(NoteObj));
	}
	
	Obj->SetArrayField(TEXT("notes"), NoteJsonArray);
	
	FString OutputString;
	const auto Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(Obj, Writer);
	
	return OutputString;
}

/**
 * @brief 楽譜情報をJSONに変換する
 * @param [in] FrameAudioQuery : フレームごとの音声合成用のクエリ
 * @return FrameAudioQueryのJSON
 */
FString UVoicevoxApiSubsystem::FrameAudioQueryConverterToJsonString(FVoicevoxFrameAudioQuery FrameAudioQuery)
{
	const TSharedRef<FJsonObject> Obj = MakeShared<FJsonObject>();
	
	TArray<TSharedPtr<FJsonValue>> F0JsonArray;
	for (const float& f : FrameAudioQuery.F0)
	{
		F0JsonArray.Add(MakeShared<FJsonValueNumber>(f));
	}
	
	Obj->SetArrayField(TEXT("f0"), F0JsonArray);
	
	TArray<TSharedPtr<FJsonValue>> VolumeJsonArray;
	for (const float& v : FrameAudioQuery.Volume)
	{
		VolumeJsonArray.Add(MakeShared<FJsonValueNumber>(v));
	}
	
	Obj->SetArrayField(TEXT("volume"), VolumeJsonArray);
	
	TArray<TSharedPtr<FJsonValue>> PhonemeJsonArray;
	for (const auto& [Phoneme, Frame_Length, Note_ID] : FrameAudioQuery.Phoneme)
	{
		TSharedPtr<FJsonObject> FramePhonemeObj = MakeShared<FJsonObject>();
		
		// note_idにnullか文字列を入れる必要があるため、UStructToJsonObjectは使用しない
		if (Note_ID.IsEmpty())
		{
			FramePhonemeObj->SetObjectField(TEXT("note_id"), nullptr);
		}
		else
		{
			FramePhonemeObj->SetStringField(TEXT("note_id"),  Note_ID);
		}
		
		FramePhonemeObj->SetStringField(TEXT("phoneme"),  Phoneme);
		FramePhonemeObj->SetNumberField(TEXT("frame_length"),  Frame_Length);
		
		PhonemeJsonArray.Add(MakeShared<FJsonValueObject>(FramePhonemeObj));
	}
	
	Obj->SetArrayField(TEXT("phonemes"), PhonemeJsonArray);
	Obj->SetNumberField(TEXT("volumeScale"), FrameAudioQuery.VolumeScale);
	Obj->SetNumberField(TEXT("outputSamplingRate"), FrameAudioQuery.OutputSamplingRate);
	Obj->SetBoolField(TEXT("outputStereo"), FrameAudioQuery.bOutputStereo);
	
	FString OutputString;
	const auto Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(Obj, Writer);
		
	return OutputString;
}