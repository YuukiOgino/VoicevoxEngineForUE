// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  各VOICEVOX COREのテストコードをまとめたCPPファイル
 * @author Yuuki Ogino
 */

#include "Misc/AutomationTest.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"
#include "Utilities/VoicevoxJsonObjectConverter.h"

#if WITH_DEV_AUTOMATION_TESTS 

/**
 * @brief JSON(AudioQuery型)を検証するテストコード
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVoicevoxCoreAQSimpleTest, "VOICEVOX.AudioQueryTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FVoicevoxCoreAQSimpleTest::RunTest(const FString& Parameters)
{
	// GEngineが有効か確認
	if (!TestNotNull(TEXT("GEngine"), GEngine))
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine null"));
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("VOICEVOX CORE AudioQuery:RunTest"));
	if (GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Initialize(true, 0, false))
	{
		if (GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->LoadModel(3))
		{
			const FVoicevoxAudioQuery AudioQuery = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAudioQuery(3, TEXT("おっけー、あぅ、くっくっく、ずんだもんは食べてもおいしくないのだ！　食うな、食うなーっ！"), false);
			
			// JSONに変換して正しい型になっているかをチェックする
			FString OutputJson = "";
			FVoicevoxJsonObjectConverter::AudioQueryToJsonObject(AudioQuery, OutputJson);

			const VoicevoxResultCode Code = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::AudioQuery, OutputJson);
			if (Code == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_AUDIO_QUERY_ERROR)
			{
				// 中のAccentPhrasesでエラーが起きているかチェック
				for (auto AccentPhrase : AudioQuery.Accent_Phrases)
				{
					// 配列を書き出し
					FString APOutputJson = "";
					FVoicevoxJsonObjectConverter::AccentPhrasesToJsonObject(AccentPhrase, APOutputJson);

					if (const VoicevoxResultCode APCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::AccentPhrase, APOutputJson); 
						APCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_ACCENT_PHRASE_ERROR)
					{
						// 中のMoraでエラーが起きているかチェック
						for (FVoicevoxMora MoraValue : AccentPhrase.Moras)
						{
							FString MOutputJson = "";
							FVoicevoxJsonObjectConverter::MoraToJsonObject(MoraValue, MOutputJson);

							if (const VoicevoxResultCode MCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::Mora, MOutputJson); 
								MCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_MORA_ERROR)
							{
								UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_MORA_ERROR"));
								UE_LOG(LogTemp, Error, TEXT("%s"), *MOutputJson);
								GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
								return false;
							}
						}
						UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_ACCENT_PHRASE_ERROR"));
						UE_LOG(LogTemp, Error, TEXT("%s"), *APOutputJson);
						GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
						return false;
					}
				}
				
				UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_AUDIO_QUERY_ERROR"));
				UE_LOG(LogTemp, Error, TEXT("%s"), *OutputJson);
				GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
				return false;
			}
			UE_LOG(LogTemp, Log, TEXT("VOICEVOX_RESULT:OK"));
			GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
			return Code == VoicevoxResultCode::VOICEVOX_RESULT_OK;
		}
		UE_LOG(LogTemp, Error, TEXT("No Loaded VVM StyleId 3"));
		GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
		return false;
	}
	
	UE_LOG(LogTemp, Error, TEXT("VOICEVOX CORE Initialize Failed"));
	return false;
}

/**
 * @brief JSON(AccentPhrases型)を検証するテストコード
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVoicevoxCoreAPSimpleTest, "VOICEVOX.AccentPhrasesTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FVoicevoxCoreAPSimpleTest::RunTest(const FString& Parameters)
{
	// GEngineが有効か確認
	if (!TestNotNull(TEXT("GEngine"), GEngine))
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine null"));
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("VOICEVOX CORE AccentPhrases:RunTest"));
	if (GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Initialize(true, 0, false))
	{
		if (GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->LoadModel(3))
		{
			TArray<FVoicevoxAccentPhrase> AccentPhrases = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SynthesizerCreateAccentPhrases(3, TEXT("おっけー、あぅ、くっくっく、ずんだもんは食べてもおいしくないのだ！　食うな、食うなーっ！"), false);
			
			
			// JSONに変換して正しい型になっているかをチェックする
			for (auto AccentPhrase : AccentPhrases)
			{
				// 配列を書き出し
				FString APOutputJson = "";
				FVoicevoxJsonObjectConverter::AccentPhrasesToJsonObject(AccentPhrase, APOutputJson);

				if (const VoicevoxResultCode APCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::AccentPhrase, APOutputJson); 
					APCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_ACCENT_PHRASE_ERROR)
				{
					// 中のMoraでエラーが起きているかチェック
					for (FVoicevoxMora MoraValue : AccentPhrase.Moras)
					{
						FString MOutputJson = "";
						FVoicevoxJsonObjectConverter::MoraToJsonObject(MoraValue, MOutputJson);

						if (const VoicevoxResultCode MCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::Mora, MOutputJson); 
							MCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_MORA_ERROR)
						{
							UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_MORA_ERROR"));
							UE_LOG(LogTemp, Error, TEXT("%s"), *MOutputJson);
							GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
							return false;
						}
					}
					UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_ACCENT_PHRASE_ERROR"));
					UE_LOG(LogTemp, Error, TEXT("%s"), *APOutputJson);
					GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
					return false;
				}
			}
				
			UE_LOG(LogTemp, Log, TEXT("VOICEVOX_RESULT:OK"));
			GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
			return true;
			
		}
		UE_LOG(LogTemp, Error, TEXT("No Loaded VVM StyleId 3"));
		GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
		return false;
	}
	
	UE_LOG(LogTemp, Error, TEXT("VOICEVOX CORE Initialize Failed"));
	return false;
}

/**
 * @brief JSON(Mora型)を検証するテストコード
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVoicevoxCoreMoraSimpleTest, "VOICEVOX.MoraTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FVoicevoxCoreMoraSimpleTest::RunTest(const FString& Parameters)
{
	// GEngineが有効か確認
	if (!TestNotNull(TEXT("GEngine"), GEngine))
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine null"));
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("VOICEVOX CORE Mora:RunTest"));
	// 母音、子音、ッ、句読点、句読点なし（アクセントの区切り）をチェック。バージョンアップによる正常パターンの変更をチェックする目的のため、エラーパターンは考慮しない
	TArray<FVoicevoxMora> Moras;
	Moras.Add({.Text = TEXT("オ"), .Consonant = TEXT(""), .Consonant_Length = 0, .Vowel = TEXT("o"), .Vowel_Length = 0.19f, .Pitch = 5.76f});
	Moras.Add({.Text = TEXT("ッ"), .Consonant = TEXT(""), .Consonant_Length = 0, .Vowel = TEXT("cl"),.Vowel_Length = 0.07f, .Pitch = 0});
	Moras.Add({.Text = TEXT("ケ"), .Consonant = TEXT("k"), .Consonant_Length = 0.08f, .Vowel = TEXT("e"),.Vowel_Length = 0.11f, .Pitch = 6.06f});
	Moras.Add({.Text = TEXT("、"), .Consonant = TEXT(""), .Consonant_Length = 0, .Vowel = TEXT("pau"),.Vowel_Length = 0.31f, .Pitch = 0});
	Moras.Add({.Text = TEXT(""),  .Consonant = TEXT(""), .Consonant_Length = 0, .Vowel = TEXT("pau"),.Vowel_Length = 0, .Pitch = 0});
			
	// JSONに変換して正しい型になっているかをチェックする
	for (FVoicevoxMora MoraValue : Moras)
	{
		FString MOutputJson = "";
		FVoicevoxJsonObjectConverter::MoraToJsonObject(MoraValue, MOutputJson);

		if (const VoicevoxResultCode MCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::Mora, MOutputJson); 
			MCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_MORA_ERROR)
		{
			UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_MORA_ERROR"));
			UE_LOG(LogTemp, Error, TEXT("%s"), *MOutputJson);
			return false;
		}
	}
				
	UE_LOG(LogTemp, Log, TEXT("VOICEVOX_RESULT:OK"));
	return true;
}

/**
 * @brief JSON(Note型)を検証するテストコード
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVoicevoxCoreNoteSimpleTest, "VOICEVOX.NoteTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FVoicevoxCoreNoteSimpleTest::RunTest(const FString& Parameters)
{
	// GEngineが有効か確認
	if (!TestNotNull(TEXT("GEngine"), GEngine))
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine null"));
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("VOICEVOX CORE Note:RunTest"));

	const FVoicevoxNote Note1{.ID = TEXT("あ2"), .Key = -1, .Frame_Length = 45, .Lyric = TEXT("")}; // 休符は空白、Keyを‐1に
	const FVoicevoxNote Note2{.ID = TEXT(""), .Key = 10, .Frame_Length = 45, .Lyric = TEXT("か")};
	const FVoicevoxNote Note3{.ID = TEXT("あ122"), .Key = 50, .Frame_Length = 45, .Lyric = TEXT("か")};
			
	// JSONに変換して正しい型になっているかをチェックする
	FString OutputJson = "";
	FVoicevoxJsonObjectConverter::NoteToJsonObject(Note1, OutputJson);

	if (const VoicevoxResultCode MCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::Note, OutputJson); 
		MCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_NOTE_ERROR)
	{
		UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_NOTE_ERROR"));
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutputJson);
		return false;
	}
	
	FVoicevoxJsonObjectConverter::NoteToJsonObject(Note2, OutputJson);
	if (const VoicevoxResultCode MCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::Note, OutputJson); 
	MCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_NOTE_ERROR)
	{
		UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_NOTE_ERROR"));
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutputJson);
		return false;
	}
	
	FVoicevoxJsonObjectConverter::NoteToJsonObject(Note3, OutputJson);
	if (const VoicevoxResultCode MCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::Note, OutputJson); 
	MCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_NOTE_ERROR)
	{
		UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_NOTE_ERROR"));
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutputJson);
		return false;
	}
				
	UE_LOG(LogTemp, Log, TEXT("VOICEVOX_RESULT:OK"));
	return true;
}

/**
 * @brief JSON(Score型)を検証するテストコード
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVoicevoxCoreScoreSimpleTest, "VOICEVOX.ScoreTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FVoicevoxCoreScoreSimpleTest::RunTest(const FString& Parameters)
{
	// GEngineが有効か確認
	if (!TestNotNull(TEXT("GEngine"), GEngine))
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine null"));
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("VOICEVOX CORE Score:RunTest"));
	
	FVoicevoxScore Score{};
	Score.Notes.Add({.ID = TEXT(""), .Key = -1, .Frame_Length = 45, .Lyric = TEXT("")}); // 休符は空白、Keyを‐1に
	Score.Notes.Add({.ID = TEXT(""), .Key = 10, .Frame_Length = 45, .Lyric = TEXT("か")});
	Score.Notes.Add({.ID = TEXT("あ122"), .Key = 50, .Frame_Length = 45, .Lyric = TEXT("か")});
			
	// JSONに変換して正しい型になっているかをチェックする
	const FString OutputJson = FVoicevoxJsonObjectConverter::ScoreConverterToJsonString(Score);

	if (const VoicevoxResultCode MCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::Score, OutputJson); 
		MCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_SCORE_ERROR)
	{
		for (auto Note : Score.Notes)
		{
			FString NOutputJson = "";
			FVoicevoxJsonObjectConverter::NoteToJsonObject(Note, NOutputJson);

			if (const VoicevoxResultCode NCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::Note, NOutputJson); 
				NCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_NOTE_ERROR)
			{
				UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_NOTE_ERROR"));
				UE_LOG(LogTemp, Error, TEXT("%s"), *NOutputJson);
				return false;
			}
		}
		UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_SCORE_ERROR"));
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutputJson);
		return false;
	}
				
	UE_LOG(LogTemp, Log, TEXT("VOICEVOX_RESULT:OK"));
	return true;
}

/**
 * @brief JSON(FrameAudioQuery型)を検証するテストコード
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVoicevoxCoreFAQSimpleTest, "VOICEVOX.FrameAudioQueryTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FVoicevoxCoreFAQSimpleTest::RunTest(const FString& Parameters)
{
	// GEngineが有効か確認
	if (!TestNotNull(TEXT("GEngine"), GEngine))
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine null"));
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("VOICEVOX CORE FrameAudioQuery:RunTest"));
	if (GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Initialize(true, 0, false))
	{
		// 波音リツ（ノーマル）　6000が実質APIでFrameAudioQueryを生成できる唯一のID
		if (constexpr int Singing_Teacher = 6000; 
			GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->LoadModel(Singing_Teacher))
		{
			FVoicevoxScore Score{};
			Score.Notes.Add({.ID = TEXT(""), .Key = -1, .Frame_Length = 10, .Lyric = TEXT("")}); // 休符は空白、Keyを‐1に
			Score.Notes.Add({.ID = TEXT(""), .Key = 10, .Frame_Length = 10, .Lyric = TEXT("か")});
			Score.Notes.Add({.ID = TEXT("あ122"), .Key = 50, .Frame_Length = 45, .Lyric = TEXT("あ")});
			Score.Notes.Add({.ID = TEXT("あ123"), .Key = -1, .Frame_Length = 90, .Lyric = TEXT("")}); 
			
			const FVoicevoxFrameAudioQuery AudioQuery = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SynthesizerCreateSingFrameAudioQuery(Score, Singing_Teacher);
			
			// JSONに変換して正しい型になっているかをチェックする
			const FString OutputJson = FVoicevoxJsonObjectConverter::FrameAudioQueryConverterToJsonString(AudioQuery);

			const VoicevoxResultCode Code = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::FrameAudioQuery, OutputJson);
			if (Code == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_FRAME_AUDIO_QUERY_ERROR)
			{
				UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:FRAME_AUDIO_QUERY_ERROR"));
				UE_LOG(LogTemp, Error, TEXT("%s"), *OutputJson);
				GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
				return false;
			}
			UE_LOG(LogTemp, Log, TEXT("VOICEVOX_RESULT:OK"));
			GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
			return Code == VoicevoxResultCode::VOICEVOX_RESULT_OK;
		}
		UE_LOG(LogTemp, Error, TEXT("No Loaded VVM StyleId 3"));
		GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
		return false;
	}
	
	UE_LOG(LogTemp, Error, TEXT("VOICEVOX CORE Initialize Failed"));
	return false;
}

/**
 * @brief JSON(FramePhonemeTest)を検証するテストコード
 */
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVoicevoxCoreFPSimpleTest, "VOICEVOX.FramePhonemeTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FVoicevoxCoreFPSimpleTest::RunTest(const FString& Parameters)
{
	// GEngineが有効か確認
	if (!TestNotNull(TEXT("GEngine"), GEngine))
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine null"));
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("VOICEVOX CORE FramePhoneme:RunTest"));

	const FVoicevoxFramePhoneme Phoneme1{.Phoneme = TEXT("pau"), .Frame_Length = 8, .Note_ID = TEXT("")};
	const FVoicevoxFramePhoneme Phoneme2{.Phoneme = TEXT("k"), .Frame_Length = 2, .Note_ID = TEXT("")};
	const FVoicevoxFramePhoneme Phoneme3{.Phoneme = TEXT("a"), .Frame_Length = 45, .Note_ID = TEXT("あ122")};
	const FVoicevoxFramePhoneme Phoneme4{.Phoneme = TEXT("pau"), .Frame_Length = 8, .Note_ID = TEXT("あ123")};
	
	// JSONに変換して正しい型になっているかをチェックする
	FString OutputJson = "";
	FVoicevoxJsonObjectConverter::FramePhonemeToJsonObject(Phoneme1, OutputJson);

	if (const VoicevoxResultCode MCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::FramePhoneme, OutputJson); 
		MCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_FRAME_PHONEME_ERROR)
	{
		UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_FRAME_PHONEME_ERROR"));
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutputJson);
		return false;
	}
	
	FVoicevoxJsonObjectConverter::FramePhonemeToJsonObject(Phoneme2, OutputJson);
	if (const VoicevoxResultCode MCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::FramePhoneme, OutputJson); 
	MCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_FRAME_PHONEME_ERROR)
	{
		UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_FRAME_PHONEME_ERROR"));
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutputJson);
		return false;
	}
	
	FVoicevoxJsonObjectConverter::FramePhonemeToJsonObject(Phoneme3, OutputJson);
	if (const VoicevoxResultCode MCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::FramePhoneme, OutputJson); 
	MCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_FRAME_PHONEME_ERROR)
	{
		UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_FRAME_PHONEME_ERROR"));
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutputJson);
		return false;
	}
	
	FVoicevoxJsonObjectConverter::FramePhonemeToJsonObject(Phoneme4, OutputJson);
	if (const VoicevoxResultCode MCode = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->VoicevoxValidate(EVoicevoxCoreValidateType::FramePhoneme, OutputJson); 
	MCode == VoicevoxResultCode::VOICEVOX_RESULT_INVALID_FRAME_PHONEME_ERROR)
	{
		UE_LOG(LogTemp, Error, TEXT("VOICEVOX_RESULT:INVALID_FRAME_PHONEME_ERROR"));
		UE_LOG(LogTemp, Error, TEXT("%s"), *OutputJson);
		return false;
	}
				
	UE_LOG(LogTemp, Log, TEXT("VOICEVOX_RESULT:OK"));
	return true;
}

#endif