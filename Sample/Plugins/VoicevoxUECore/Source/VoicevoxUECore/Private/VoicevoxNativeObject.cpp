// Copyright Yuuki Ogino. All Rights Reserved.

#include "VoicevoxNativeObject.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"
#include "Subsystems/VoicevoxNativeCoreSubsystem.h"

void UVoicevoxNativeObject::Init()
{
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 0)
	VoicevoxSubsystemCollection.Initialize(this);
#else
	if (!VoicevoxSubsystemCollection.IsInitialized())
	{
		VoicevoxSubsystemCollection.Initialize(this);
	}
#endif
	const UClass* BaseType = UVoicevoxNativeCoreSubsystem::StaticClass();
	GetDerivedClasses(BaseType, SubsystemClasses, true);
}

void UVoicevoxNativeObject::Shutdown()
{
	VoicevoxSubsystemCollection.Deinitialize();
	SubsystemClasses.Empty();
}

void UVoicevoxNativeObject::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
	UVoicevoxNativeObject* This = CastChecked<UVoicevoxNativeObject>(InThis);
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 0)
	This->VoicevoxSubsystemCollection.AddReferencedObjects(Collector);
#else
	This->VoicevoxSubsystemCollection.AddReferencedObjects(This, Collector);
#endif
	UObject::AddReferencedObjects(This, Collector);
}

void UVoicevoxNativeObject::CoreInitialize(const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (const auto bIsSuccess = Subsystem->CoreInitialize(bUseGPU, CPUNumThreads, bLoadAllModels); !bIsSuccess)
		{
			GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SetInitializeResult(false);
			return;
		}

		GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->AddVoicevoxConfigData(
				Subsystem->GetVoicevoxCoreName(),
				Subsystem->GetMetaList(),
				Subsystem->GetSupportedDevices(),
				Subsystem->GetVoicevoxVersion(),
				Subsystem->IsGpuMode());
	}

	GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SetInitializeResult(true);
}

void UVoicevoxNativeObject::Finalize()
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = VoicevoxSubsystemCollection.GetSubsystem(Element);
		static_cast<UVoicevoxNativeCoreSubsystem*>(Subsystem)->Finalize();
	}

	GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SetFinalizeResult(true);
}

/**
 * @fn
 * VOICEVOX COREのモデルをロード実行
 * @brief モデルをロードする。
 * @param SpeakerId 話者番号
 * @return 成功したらtrue、失敗したらfalse
 * @detail
 * 必ずしも話者とモデルが1:1対応しているわけではない。
 *
 * ※モデルによってはメインスレッドが暫く止まるほど重いので、その場合は非同期で処理してください。（UE::Tasks::Launch等）
 */
void UVoicevoxNativeObject::LoadModel(int64 SpeakerId)
{
	for (const auto Element : SubsystemClasses)
	{
		if (const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element)); Subsystem->IsModel(SpeakerId))
		{
			if (const auto bIsSuccess = Subsystem->LoadModel(SpeakerId); bIsSuccess)
			{
				GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SetLodeModelResult(true);
				return;
			}
			break;
		}
	}

	GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SetLodeModelResult(false);
}

/**
 * @fn
 * VOICEVOX COREのvoicevox_audio_queryを取得
 * @brief AudioQuery を取得する。
 * @param[in] SpeakerId 話者番号
 * @param[in] Message 音声データに変換するtextデータ
 * @param[in] bKana aquestalk形式のkanaとしてテキストを解釈する
 * @return AudioQueryをjsonでフォーマット後、構造体へ変換したもの。
 * @details
 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
 */
FVoicevoxAudioQuery UVoicevoxNativeObject::GetAudioQuery(int64 SpeakerId, const FString& Message, bool bKana)
{
	for (const auto Element : SubsystemClasses)
	{
		if (const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element)); Subsystem->IsModel(SpeakerId))
		{
			return Subsystem->GetAudioQuery(SpeakerId, Message, bKana);
		}
	}

	return FVoicevoxAudioQuery();
}

/**
 * @fn
 * VOICEVOX COREのtext to speechを実行
 * @brief Textデータを音声データに変換する。
 * @param[in] SpeakerId 話者番号
 * @param[in] Message 音声データに変換するtextデータ
 * @param[in] bKana aquestalk形式のkanaとしてテキストを解釈する
 * @param[in] bEnableInterrogativeUpspeak 疑問文の調整を有効にする
 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
 * @details
 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
 */
TArray<uint8> UVoicevoxNativeObject::RunTextToSpeech(int64 SpeakerId, const FString& Message, bool bKana, bool bEnableInterrogativeUpspeak)
{
	for (const auto Element : SubsystemClasses)
	{
		if (const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element)); Subsystem->IsModel(SpeakerId))
		{
			return Subsystem->RunTextToSpeech(SpeakerId, Message, bKana, bEnableInterrogativeUpspeak);
		}
	}

	return TArray<uint8>();
}

/**
 * @fn
 * VOICEVOX COREのvoicevox_synthesisを実行
 * @brief AudioQueryを音声データに変換する。
 * @param[in] AudioQueryJson jsonフォーマットされた AudioQuery
 * @param[in] SpeakerId 話者番号
 * @param[in] bEnableInterrogativeUpspeak 疑問文の調整を有効にする
 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
 * @details
 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
 */
TArray<uint8> UVoicevoxNativeObject::RunSynthesis(const char* AudioQueryJson, int64 SpeakerId, bool bEnableInterrogativeUpspeak)
{
	for (const auto Element : SubsystemClasses)
	{
		if (const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element)); Subsystem->IsModel(SpeakerId))
		{
			return Subsystem->RunSynthesis(AudioQueryJson, SpeakerId, bEnableInterrogativeUpspeak);
		}
	}

	return TArray<uint8>();
}

/**
 * @fn
 * VOICEVOX COREのvoicevox_synthesisを実行
 * @brief AudioQueryを音声データに変換する。
 * @param[in] AudioQueryJson jsonフォーマットされた AudioQuery構造体
 * @param[in] SpeakerId 話者番号
 * @param[in] bEnableInterrogativeUpspeak 疑問文の調整を有効にする
 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
 * @details
 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
 */
TArray<uint8> UVoicevoxNativeObject::RunSynthesis(const FVoicevoxAudioQuery& AudioQueryJson, int64 SpeakerId, bool bEnableInterrogativeUpspeak)
{
	for (const auto Element : SubsystemClasses)
	{
		if (const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element)); Subsystem->IsModel(SpeakerId))
		{
			return Subsystem->RunSynthesis(AudioQueryJson, SpeakerId, bEnableInterrogativeUpspeak);
		}
	}

	return TArray<uint8>();
}

/**
 * @fn
 * 音素ごとの長さを求める
 * @brief 音素列から、音素ごとの長さを求める
 * @param[in] Length 音素列の長さ
 * @param[in] PhonemeList 音素列
 * @param[in] SpeakerID 話者番号
 * @return 音素ごとの長さ
 *
 * @warning 動作確認が取れていないため、クラッシュ、もしくは予期せぬ動作をする可能性が高いです。
 */
TArray<float> UVoicevoxNativeObject::GetPhonemeLength(int64 Length, TArray<int64> PhonemeList, int64 SpeakerID)
{
	for (const auto Element : SubsystemClasses)
	{
		if (const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element)); Subsystem->IsModel(SpeakerID))
		{
			return Subsystem->GetPhonemeLength(Length, PhonemeList, SpeakerID);
		}
	}

	return TArray<float>();
}

/**
 * @fn
 * モーラごとの音高を求める
 * @brief モーラごとの音素列とアクセント情報から、モーラごとの音高を求める
 * @param[in] Length モーラ列の長さ
 * @param[in] VowelPhonemeList 母音の音素列
 * @param[in] ConsonantPhonemeList 子音の音素列
 * @param[in] StartAccentList アクセントの開始位置
 * @param[in] EndAccentList アクセントの終了位置
 * @param[in] StartAccentPhraseList アクセント句の開始位置
 * @param[in] EndAccentPhraseList アクセント句の終了位置
 * @param[in] SpeakerID 話者番号
 * @return モーラごとの音高
 *
 * @warning 動作確認が取れていないため、クラッシュ、もしくは予期せぬ動作をする可能性が高いです。
 */
TArray<float> UVoicevoxNativeObject::FindPitchEachMora(int64 Length, TArray<int64> VowelPhonemeList, TArray<int64> ConsonantPhonemeList,
										  TArray<int64> StartAccentList, TArray<int64> EndAccentList,
										  TArray<int64> StartAccentPhraseList, TArray<int64> EndAccentPhraseList,
										  int64 SpeakerID)
{
	for (const auto Element : SubsystemClasses)
	{
		if (const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element)); Subsystem->IsModel(SpeakerID))
		{
			return Subsystem->FindPitchEachMora(Length, VowelPhonemeList, ConsonantPhonemeList, StartAccentList, EndAccentList, StartAccentPhraseList, EndAccentPhraseList, SpeakerID);
		}
	}

	return TArray<float>();
}
	
/**
 * @fn
 * 波形を求める
 * @brief フレームごとの音素と音高から、波形を求める
 * @param[in] Length フレームの長さ
 * @param[in] PhonemeSize 音素の種類数
 * @param[in] F0 フレームごとの音高
 * @param[in] Phoneme フレームごとの音素
 * @param[in] SpeakerID 話者番号
 * @return 音声波形
 *
 * @warning 動作確認が取れていないため、クラッシュ、もしくは予期せぬ動作をする可能性が高いです。
 */
TArray<float> UVoicevoxNativeObject::DecodeForward(int64 Length, int64 PhonemeSize, TArray<float> F0, TArray<float> Phoneme, int64 SpeakerID)
{
	for (const auto Element : SubsystemClasses)
	{
		if (const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element)); Subsystem->IsModel(SpeakerID))
		{
			return Subsystem->DecodeForward(Length, PhonemeSize, F0, Phoneme, SpeakerID);
		}
	}

	return TArray<float>();
}