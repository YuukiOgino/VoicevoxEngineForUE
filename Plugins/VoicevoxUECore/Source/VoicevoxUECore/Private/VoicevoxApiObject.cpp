// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  複数のVOICEVOX COREライブラリへアクセスするSubsystemを管理し、各APIへアクセスするCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxApiObject.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"
#include "Subsystems/VoicevoxNativeCoreSubsystem.h"

/**
 * @brief サブシステム管理オブジェクト初期化
 */
void UVoicevoxApiObject::Init()
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

/**
 * @brief サブシステム管理オブジェクト破棄
 */
void UVoicevoxApiObject::Shutdown()
{
	VoicevoxSubsystemCollection.Deinitialize();
	SubsystemClasses.Empty();
}

/**
 * @brief リファレンスオブジェクトをガベージコレクション対象外に登録
 */
void UVoicevoxApiObject::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
	UVoicevoxApiObject* This = CastChecked<UVoicevoxApiObject>(InThis);
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 0)
	This->VoicevoxSubsystemCollection.AddReferencedObjects(Collector);
#else
	This->VoicevoxSubsystemCollection.AddReferencedObjects(This, Collector);
#endif
	UObject::AddReferencedObjects(This, Collector);
}

//--------------------------------
// VOICEVOX CORE APIアクセス関数
//--------------------------------

//--------------------------------
// VOICEVOX CORE Initialize関連
//--------------------------------
	
/**
 * @brief 音声合成するための初期化を行う。VOICEVOXのAPIを正しく実行するには先に初期化が必要
 */
bool UVoicevoxApiObject::CoreInitialize(const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (const auto bIsSuccess = Subsystem->ApiInitialize(bUseGPU, CPUNumThreads, bLoadAllModels); !bIsSuccess)
		{
			return false;
		}

		GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->AddVoicevoxConfigData(
				Subsystem->GetVoicevoxCoreName(),
				Subsystem->GetSupportedDevices(),
				Subsystem->GetVoicevoxVersion(),
				Subsystem->IsGpuMode());
	}

	return true;
}

/**
 * @brief デフォルトの初期化オプションを生成する
 * @return デフォルト値が設定された初期化オプション
 */
VoicevoxInitializeOptions UVoicevoxApiObject::MakeDefaultInitializeOptions()
{
	// デフォルト値は最初に処理したSubsystemの値を返す
	const auto Element = *SubsystemClasses.begin();
	const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
	return Subsystem->MakeDefaultInitializeOptions();
}

//--------------------------------
// VOICEVOX CORE Finalize関連
//--------------------------------
	
/**
 * @brief 終了処理を行う。以降VOICEVOXのAPIを利用するためには再度Initializeメソッドを行う必要がある。
 */
void UVoicevoxApiObject::Finalize()
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = VoicevoxSubsystemCollection.GetSubsystem(Element);
		static_cast<UVoicevoxNativeCoreSubsystem*>(Subsystem)->Finalize();
	}
}

//--------------------------------
// VOICEVOX CORE OpenJtake関連
//--------------------------------
	
/**
 * @brief 日本語のテキストを解析する。
 */
TArray<FVoicevoxAccentPhrase> UVoicevoxApiObject::OpenJTalkRcAnalyze(const FString& Text)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (TArray<FVoicevoxAccentPhrase> Analyze = Subsystem->OpenJTalkRcAnalyze(Text); !Analyze.IsEmpty())
		{
			return Analyze;
		}
	}

	return TArray<FVoicevoxAccentPhrase>();
}

//--------------------------------
// VOICEVOX CORE Model関連(旧API)
//--------------------------------

/**
 * @brief モデルをロードする。
 */
bool UVoicevoxApiObject::LoadModel(const int64 SpeakerId)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (const auto bIsSuccess = Subsystem->LoadModel(SpeakerId); bIsSuccess)
		{
			return true;
		}
	}

	return false;
}

//--------------------------------
// VOICEVOX CORE VMM関連
//--------------------------------

/**
 * @brief　VVMファイルのロード実行
 */
bool UVoicevoxApiObject::LoadVoiceModel(const FString& VvmFileName)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (const auto bIsSuccess = Subsystem->LoadVoiceModel(VvmFileName); bIsSuccess)
		{
			return true;
		}
	}

	return false;
}

/**
 * @brief　全てのVVMファイルを開く。
 * @returns 読み込み結果
 */
bool UVoicevoxApiObject::AllLoadVoiceModel()
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (const auto bIsSuccess = Subsystem->AllLoadVoiceModel(); bIsSuccess)
		{
			return true;
		}
	}

	return false;
}

/**
 * @breaf 音声モデルの読み込みを解除する。
 */
bool UVoicevoxApiObject::UnloadVoiceModel(const FString& VvmFileName)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (const auto bIsSuccess = Subsystem->UnloadVoiceModel(VvmFileName); bIsSuccess)
		{
			return true;
		}
	}

	return false;
}

/**
 * @breaf 指定したIDの音声モデルが読み込まれているか判定する。
 */
bool UVoicevoxApiObject::IsLoadedVoiceModel(const FString& VvmFileName)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (const auto bIsSuccess = Subsystem->IsLoadedVoiceModel(VvmFileName); bIsSuccess)
		{
			return true;
		}
	}

	return false;
}

//--------------------------------
// VOICEVOX CORE AudioQuery関連
//--------------------------------

/**
 * @brief AudioQuery を取得する。
 */
FVoicevoxAudioQuery UVoicevoxApiObject::GetAudioQuery(const int64 SpeakerId, const FString& Message, const bool bKana)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (FVoicevoxAudioQuery AudioQuery = Subsystem->GetAudioQuery(SpeakerId, Message, bKana); !AudioQuery.Kana.IsEmpty()) return AudioQuery;
	}

	return FVoicevoxAudioQuery();
}

/**
 * @brief  AccentPhraseの配列からAudioQueryを作る。
 */
FVoicevoxAudioQuery UVoicevoxApiObject::GetAudioQueryFromAccentPhrases(const TArray<FVoicevoxAccentPhrase>& AccentPhrases)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (FVoicevoxAudioQuery AudioQuery = Subsystem->GetAudioQueryFromAccentPhrases(AccentPhrases); !AudioQuery.Accent_phrases.IsEmpty()) return AudioQuery;
	}

	return FVoicevoxAudioQuery();
}

//--------------------------------
// VOICEVOX CORE TextToSpeech関連
//--------------------------------

/**
 * @brief Textデータを音声データに変換する。
 */
TArray<uint8> UVoicevoxApiObject::RunTextToSpeech(int64 SpeakerId, const FString& Message, bool bKana, bool bEnableInterrogativeUpspeak)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (TArray<uint8> Wav = Subsystem->RunTextToSpeech(SpeakerId, Message, bKana, bEnableInterrogativeUpspeak); !Wav.IsEmpty()) return Wav;
	}

	return TArray<uint8>();
}

/**
 * @brief デフォルトのテキスト音声合成オプションを生成する
 * @return テキスト音声合成オプション
 */
VoicevoxTtsOptions UVoicevoxApiObject::MakeDefaultTtsOptions()
{
	// デフォルト値は最初に処理したSubsystemの値を返す
	const auto Element = *SubsystemClasses.begin();
	const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
	return Subsystem->MakeDefaultTtsOptions();
}

//--------------------------------
// VOICEVOX CORE Synthesis関連
//--------------------------------

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UVoicevoxApiObject::RunSynthesis(const char* AudioQueryJson, int64 SpeakerId, bool bEnableInterrogativeUpspeak)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (TArray<uint8> Wav = Subsystem->RunSynthesis(AudioQueryJson, SpeakerId, bEnableInterrogativeUpspeak); !Wav.IsEmpty()) return Wav;
	}

	return TArray<uint8>();
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UVoicevoxApiObject::RunSynthesis(const FVoicevoxAudioQuery& AudioQueryJson, int64 SpeakerId, bool bEnableInterrogativeUpspeak)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (TArray<uint8> Wav = Subsystem->RunSynthesis(AudioQueryJson, SpeakerId, bEnableInterrogativeUpspeak); !Wav.IsEmpty()) return Wav;
	}

	return TArray<uint8>();
}

/**
 * @brief デフォルトの `voicevox_synthesis` のオプションを生成する
 */
VoicevoxSynthesisOptions UVoicevoxApiObject::MakeDefaultSynthesisOptions()
{
	// デフォルト値は最初に処理したSubsystemの値を返す
	const auto Element = *SubsystemClasses.begin();
	const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
	return Subsystem->MakeDefaultSynthesisOptions();
}

/**
 * @brief 日本語テキストから、AccentPhrase (アクセント句)の配列を生成する。
 */
TArray<FVoicevoxAccentPhrase> UVoicevoxApiObject::SynthesizerCreateAccentPhrases(const VoicevoxStyleId StyleId, const FString& Text, const bool bKana)
{
	for (const auto Element : SubsystemClasses)
	{
		const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
		if (TArray<FVoicevoxAccentPhrase> Analyze = Subsystem->SynthesizerCreateAccentPhrases(StyleId, Text, bKana); !Analyze.IsEmpty())
		{
			return Analyze;
		}
	}

	return TArray<FVoicevoxAccentPhrase>();
}

//--------------------------------
// VOICEVOX CORE Property関連
//--------------------------------

/**
 * @brief 話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> UVoicevoxApiObject::GetMetaList()
{
	TArray<FVoicevoxMeta> MetaList;
	for (const auto Element : SubsystemClasses)
	{
		for (const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
			auto Meta : Subsystem->GetMetaList())
		{
			MetaList.Emplace(Meta);
		}
	}

	return MetaList;
}

/**
 * @brief 指定のVoicevoxVoiceModelFileからメタ情報を取得する
 */
TArray<FVoicevoxMeta> UVoicevoxApiObject::GetVoiceModelFileMetaList(const FString& VvmFileName)
{
	TArray<FVoicevoxMeta> MetaList;
	for (const auto Element : SubsystemClasses)
	{
		for (const auto Subsystem = static_cast<UVoicevoxNativeCoreSubsystem*>(VoicevoxSubsystemCollection.GetSubsystem(Element));
			auto Meta : Subsystem->GetVoiceModelFileMetaList(VvmFileName))
		{
			MetaList.Emplace(Meta);
		}
	}

	return MetaList;
}