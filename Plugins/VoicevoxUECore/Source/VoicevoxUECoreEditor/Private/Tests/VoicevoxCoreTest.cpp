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
	
	UE_LOG(LogTemp, Log, TEXT("VOICEVOX CORE AudioQuery:RunTest"));
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

#endif