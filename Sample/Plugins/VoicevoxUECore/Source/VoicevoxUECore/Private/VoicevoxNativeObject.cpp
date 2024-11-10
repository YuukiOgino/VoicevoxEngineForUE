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