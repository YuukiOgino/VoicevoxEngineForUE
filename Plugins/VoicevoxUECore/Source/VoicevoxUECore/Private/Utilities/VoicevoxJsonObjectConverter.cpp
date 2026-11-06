// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief VOICEVOXのJSON変換を実行する CPPファイル
 * @author Yuuki Ogino
 */

#include "Utilities/VoicevoxJsonObjectConverter.h"
#include "JsonObjectConverter.h"

/**
 * @brief AudioQueryをJSON(AudioQuery型)へ変換する
 */
bool FVoicevoxJsonObjectConverter::AudioQueryToJsonObject(const FVoicevoxAudioQuery& AudioQuery, FString& OutJsonString)
{
	const TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	
	TArray<TSharedPtr<FJsonValue>> AccentPhrasesJsonArray;
	for (auto Value : AudioQuery.Accent_Phrases)
	{
		TSharedRef<FJsonObject> Obj = AccentPhrasesToJsonObject(Value);
		AccentPhrasesJsonArray.Add(MakeShared<FJsonValueObject>(Obj));
	}
	
	JsonObject->SetArrayField(TEXT("accent_phrases"), AccentPhrasesJsonArray);
	JsonObject->SetNumberField(TEXT("speedScale"),  AudioQuery.SpeedScale);
	JsonObject->SetNumberField(TEXT("pitchScale"),  AudioQuery.PitchScale);
	JsonObject->SetNumberField(TEXT("intonationScale"),  AudioQuery.IntonationScale);
	JsonObject->SetNumberField(TEXT("volumeScale"),  AudioQuery.VolumeScale);
	JsonObject->SetNumberField(TEXT("prePhonemeLength"),  AudioQuery.PrePhonemeLength);
	JsonObject->SetNumberField(TEXT("postPhonemeLength"),  AudioQuery.PostPhonemeLength);
	JsonObject->SetNumberField(TEXT("pauseLength"),  AudioQuery.PauseLength);
	JsonObject->SetNumberField(TEXT("pauseLengthScale"),  AudioQuery.PauseLengthScale);
	JsonObject->SetNumberField(TEXT("outputSamplingRate"),  AudioQuery.OutputSamplingRate);
	JsonObject->SetBoolField(TEXT("outputStereo"),  AudioQuery.bOutputStereo);
	JsonObject->SetStringField(TEXT("kana"),  AudioQuery.Kana);
	
	const auto Writer = TJsonWriterFactory<>::Create(&OutJsonString);
	FJsonSerializer::Serialize(JsonObject, Writer);
	return true;
}

/**
 * @brief AccentPhraseをJSON(AccentPhrase型)へ変換する
 */
bool FVoicevoxJsonObjectConverter::AccentPhrasesToJsonObject(const FVoicevoxAccentPhrase& AccentPhrase, FString& OutJsonString)
{
	const auto Writer = TJsonWriterFactory<>::Create(&OutJsonString);
	FJsonSerializer::Serialize(AccentPhrasesToJsonObject(AccentPhrase), Writer);
	return true;
}

/**
 * @brief AccentPhraseの配列を無名配列のJSONに変換する
 */
FString FVoicevoxJsonObjectConverter::AccentPhraseConverterToJsonString(TArray<FVoicevoxAccentPhrase> AccentPhrases)
{
	// JSON値配列を作成
	TArray<TSharedPtr<FJsonValue>> JsonArray;

	for (const auto& AccentPhrase: AccentPhrases)
	{
		JsonArray.Add(MakeShared<FJsonValueObject>(AccentPhrasesToJsonObject(AccentPhrase)));
	}

	// 配列を書き出し
	FString OutputString;
	const auto Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonArray, Writer);

	return OutputString;
}

/**
 * @brief AccentPhraseをJsonObjectへ変換する
 */
TSharedRef<FJsonObject> FVoicevoxJsonObjectConverter::AccentPhrasesToJsonObject(const FVoicevoxAccentPhrase& AccentPhrase)
{
	TSharedRef<FJsonObject> Obj = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> MoraJsonArray;

	for (const FVoicevoxMora& Mora : AccentPhrase.Moras)
	{
		MoraJsonArray.Add(MakeShared<FJsonValueObject>(MoraToJsonObject(Mora)));
	}
	Obj->SetArrayField(TEXT("moras"), MoraJsonArray);
	Obj->SetNumberField(TEXT("accent"), AccentPhrase.Accent);

	if (AccentPhrase.Pause_Mora.Text.IsEmpty())
	{
		Obj->SetObjectField(TEXT("pause_mora"), nullptr);
	}
	else
	{
		Obj->SetObjectField(TEXT("pause_mora"), MoraToJsonObject(AccentPhrase.Pause_Mora));
	}
		
	Obj->SetBoolField(TEXT("is_interrogative"), AccentPhrase.bIs_Interrogative);
	
	return Obj;
}

/**
 * @brief MoraをJSON(Mora型)へ変換する
 */
bool FVoicevoxJsonObjectConverter::MoraToJsonObject(const FVoicevoxMora& Mora, FString& OutJsonString)
{
	const auto Writer = TJsonWriterFactory<>::Create(&OutJsonString);
	FJsonSerializer::Serialize(MoraToJsonObject(Mora), Writer);
	return true;
}

/**
 * @brief MoraをJsonObjectへ変換する
 */
TSharedRef<FJsonObject> FVoicevoxJsonObjectConverter::MoraToJsonObject(const FVoicevoxMora& Mora)
{
	TSharedRef<FJsonObject> Obj = MakeShared<FJsonObject>();
	Obj->SetStringField(TEXT("text"),  Mora.Text);
	Obj->SetStringField(TEXT("vowel"),  Mora.Vowel);
	Obj->SetNumberField(TEXT("vowel_length"),Mora.Vowel_Length);
	
	if (Mora.Vowel == TEXT("pau"))
	{
		// pause_moraは子音のパラメータ（consonant、consonant_length）にnull以外が入るとインデックス範囲外エラーが起きる
		Obj->SetObjectField(TEXT("consonant"),  nullptr);
		Obj->SetObjectField(TEXT("consonant_length"),  nullptr);
		Obj->SetNumberField(TEXT("pitch"),  0);
	}
	else if (Mora.Vowel == TEXT("N"))
	{
		// Nは子音のパラメータ（consonant、consonant_length）にnull以外が入ると型チェックが通らなくなる
		Obj->SetObjectField(TEXT("consonant"),  nullptr);
		Obj->SetObjectField(TEXT("consonant_length"),  nullptr);
		Obj->SetNumberField(TEXT("pitch"),  Mora.Pitch);
	}
	else if (Mora.Consonant.IsEmpty())
	{
		// 母音のみは子音のパラメータ（consonant、consonant_length）にnull以外が入ると型チェックが通らなくなる
		Obj->SetObjectField(TEXT("consonant"),  nullptr);
		Obj->SetObjectField(TEXT("consonant_length"),  nullptr);
		Obj->SetNumberField(TEXT("pitch"),  Mora.Pitch);
	}
	else
	{
		Obj->SetStringField(TEXT("consonant"),  Mora.Consonant);
		Obj->SetNumberField(TEXT("consonant_length"),  Mora.Consonant_Length);
		Obj->SetNumberField(TEXT("pitch"),  Mora.Pitch);
	}

	return Obj;
}

/**
 * @brief 音符情報をJSON(Note型)に変換する
 */
bool FVoicevoxJsonObjectConverter::NoteToJsonObject(const FVoicevoxNote& Note, FString& OutJsonString)
{
	const auto Writer = TJsonWriterFactory<>::Create(&OutJsonString);
	FJsonSerializer::Serialize(NoteToJsonObject(Note), Writer);
	return true;
}

/**
 * @brief 音符情報をJsonObjectへ変換する
 */
TSharedRef<FJsonObject> FVoicevoxJsonObjectConverter::NoteToJsonObject(const FVoicevoxNote& Note)
{
	TSharedRef<FJsonObject> NoteObj = MakeShared<FJsonObject>();
		
	// IDにnullか文字列を入れる必要があるため、UStructToJsonObjectは使用しない
	if (Note.ID.IsEmpty())
	{
		NoteObj->SetObjectField(TEXT("id"), nullptr);
	}
	else
	{
		NoteObj->SetStringField(TEXT("id"),  Note.ID);
	}
	
	if (Note.Key < 0)
	{
		NoteObj->SetObjectField(TEXT("key"), nullptr);
	}
	else
	{
		NoteObj->SetNumberField(TEXT("key"),  Note.Key);
	}
	
	NoteObj->SetNumberField(TEXT("frame_length"),  Note.Frame_Length);
	NoteObj->SetStringField(TEXT("lyric"),  Note.Lyric);
	
	return NoteObj;
}

/**
 * @brief 楽譜情報をJSON(Score型)に変換する
 */
FString FVoicevoxJsonObjectConverter::ScoreConverterToJsonString(FVoicevoxScore Score)
{
	const TSharedRef<FJsonObject> Obj = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> NoteJsonArray;
	
	for (const auto& Note: Score.Notes)
	{
		NoteJsonArray.Add(MakeShared<FJsonValueObject>(NoteToJsonObject(Note)));
	}
	
	Obj->SetArrayField(TEXT("notes"), NoteJsonArray);
	
	FString OutputString;
	const auto Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(Obj, Writer);
	
	return OutputString;
}


/**
 * @brief フレームごとの音声合成用のクエリをJSON(FrameAudioQuery型)に変換する
 */
FString FVoicevoxJsonObjectConverter::FrameAudioQueryConverterToJsonString(FVoicevoxFrameAudioQuery FrameAudioQuery)
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
	for (const auto& FramePhoneme : FrameAudioQuery.Phoneme)
	{
		PhonemeJsonArray.Add(MakeShared<FJsonValueObject>(FramePhonemeToJsonObject(FramePhoneme)));
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

/**
 * @brief JSON(FrameAudioQuery型)をフレームごとの音声合成用のクエリに変換する
 * @param [in] JsonString : JSON(FrameAudioQuery型)
 * @return フレームごとの音声合成用のクエリ
 */
FVoicevoxFrameAudioQuery FVoicevoxJsonObjectConverter::JsonObjectStringToxFrameAudioQuery(const FString& JsonString)
{
	FVoicevoxFrameAudioQuery FrameAudioQuery{};
	
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (TSharedPtr<FJsonValue> JsonValue; FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
	{
		if (JsonValue.IsValid() && JsonValue->Type == EJson::Object)
		{
			const TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();

			for (const TArray<TSharedPtr<FJsonValue>> F0JsonArray = JsonObject->GetArrayField("f0");
				const TSharedPtr<FJsonValue>& Element : F0JsonArray)
			{
				if (float F0 = 0.f; Element->TryGetNumber(F0))
				{
					FrameAudioQuery.F0.Add(F0);
				}
			}
					
			for (const TArray<TSharedPtr<FJsonValue>> VolumeJsonArray = JsonObject->GetArrayField("volume");
				const TSharedPtr<FJsonValue>& Element : VolumeJsonArray)
			{
				if (float Volume = 0.f; Element->TryGetNumber(Volume))
				{
					FrameAudioQuery.Volume.Add(Volume);
				}
			}
					
			for (const TArray<TSharedPtr<FJsonValue>> PhonemesJsonArray = JsonObject->GetArrayField("phonemes");
				const TSharedPtr<FJsonValue>& Element : PhonemesJsonArray)
			{
				FVoicevoxFramePhoneme Phoneme;
				if (FJsonObjectConverter::JsonObjectToUStruct(Element->AsObject().ToSharedRef(), FVoicevoxFramePhoneme::StaticStruct(), &Phoneme))
				{
					FrameAudioQuery.Phoneme.Add(Phoneme);
				}
			}
					
			FrameAudioQuery.bOutputStereo = JsonObject->GetBoolField(TEXT("outputStereo"));
			FrameAudioQuery.OutputSamplingRate = JsonObject->GetIntegerField(TEXT("outputSamplingRate"));
			FrameAudioQuery.VolumeScale = JsonObject->GetNumberField(TEXT("volumeScale"));
		}
				
	}
	
	return FrameAudioQuery;
}

/**
 * @brief 音素情報をJSON(FramePhoneme型)に変換する
 * @param [in] FramePhoneme : 音素
 * @param [out] OutJsonString : JSON(FramePhoneme型)
 * @return 出力処理が成功したか
 */
bool FVoicevoxJsonObjectConverter::FramePhonemeToJsonObject(const FVoicevoxFramePhoneme& FramePhoneme, FString& OutJsonString)
{
	const auto Writer = TJsonWriterFactory<>::Create(&OutJsonString);
	FJsonSerializer::Serialize(FramePhonemeToJsonObject(FramePhoneme), Writer);
	return true;
}

/**
 * @brief 音素情報をJsonObjectへ変換する
 * @param [in] FramePhoneme : 音素
 * @return FramePhonemeを変換したJsonObject
 */
TSharedRef<FJsonObject> FVoicevoxJsonObjectConverter::FramePhonemeToJsonObject(const FVoicevoxFramePhoneme& FramePhoneme)
{
	TSharedRef<FJsonObject> FramePhonemeObj = MakeShared<FJsonObject>();
		
	// note_idにnullか文字列を入れる必要があるため、UStructToJsonObjectは使用しない
	if (FramePhoneme.Note_ID.IsEmpty())
	{
		FramePhonemeObj->SetObjectField(TEXT("note_id"), nullptr);
	}
	else
	{
		FramePhonemeObj->SetStringField(TEXT("note_id"),  FramePhoneme.Note_ID);
	}
		
	FramePhonemeObj->SetStringField(TEXT("phoneme"),  FramePhoneme.Phoneme);
	FramePhonemeObj->SetNumberField(TEXT("frame_length"),  FramePhoneme.Frame_Length);
	
	return FramePhonemeObj;
}