// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  各VOICEVOX COREのAPIを呼び出すSubsystem　CPPファイル
 * @author Yuuki Ogino
 */

#include "Subsystems/VoicevoxCoreSubsystem.h"
#include "VoicevoxNativeObject.h"

//--------------------------------
// override
//--------------------------------

/**
 * @brief Initialize
 */
void UVoicevoxCoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UClass* NativeClass = UVoicevoxNativeObject::StaticClass();
	NativeInstance = NewObject<UVoicevoxNativeObject>(this, NativeClass);
}

/**
 * @brief Deinitialize
 */
void UVoicevoxCoreSubsystem::Deinitialize()
{
	Super::Deinitialize();
	OnInitializeComplete.Unbind();

	NativeInstance->Shutdown();
}

//--------------------------------
// Subsystem管理インスタンス初期化
//--------------------------------

/**
 * @brief VoicevoxNativeCoreSubsystem管理インスタンスの初期化処理実行
 */
void UVoicevoxCoreSubsystem::NativeInitialize() const
{
	NativeInstance->Init();
}

//----------------------------------------------------------------
// VOICEVOX CORE APIアクセス関数
//----------------------------------------------------------------

//--------------------------------
// VOICEVOX CORE Initialize関連
//--------------------------------

/**
 * @brief VOICEVOX CORE 初期化
 */
void UVoicevoxCoreSubsystem::Initialize(const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
{
	MetaList.Empty();
	SupportedDevicesMap.Empty();
	VoicevoxCoreVersionMap.Empty();
	CoreNameList.Empty();
	
	NativeInstance->CoreInitialize(bUseGPU, CPUNumThreads, bLoadAllModels);
}

/**
 * @brief 音声合成するための初期化処理の結果をセット
 */
void UVoicevoxCoreSubsystem::SetInitializeResult(const bool bIsSuccess)
{
	bIsInitialized = bIsSuccess;
	
	if (!bIsSuccess)
	{
		if (!OnInitializeComplete.ExecuteIfBound(false))
		{
			UE_LOG(LogTemp, Error, TEXT("OnInitializeComplete Not Execute!!"));
		}

		return;
	}

	if (!OnInitializeComplete.ExecuteIfBound(true))
	{
		UE_LOG(LogTemp, Error, TEXT("OnInitializeComplete Not Execute!!"));
	}
	
}

/**
 * @brief 全てのVOICEVOX CORE 初期化が完了しているか
 */
bool UVoicevoxCoreSubsystem::GetIsInitialize() const
{
	return bIsInitialized;	
}

/**
 * @brief VOICEVOX CORE初期化完了のデリゲート関数登録
 */
void UVoicevoxCoreSubsystem::SetOnInitializeCompleteDelegate(const FVoicevoxCoreCompleteDelegate& OpenDelegate)
{
	if (OnInitializeComplete.IsBound()) OnInitializeComplete.Unbind();
	OnInitializeComplete = OpenDelegate;
}

//--------------------------------
// VOICEVOX CORE Finalize関連
//--------------------------------

/**
 * @brief VOICEVOX CORE 終了処理
 */
void UVoicevoxCoreSubsystem::Finalize() const
{
	NativeInstance->Finalize();
}

/**
 * @brief VOICEVOX CORE終了処理完了のデリゲート関数登録
 */
void UVoicevoxCoreSubsystem::SetOnFinalizeCompleteDelegate(const FVoicevoxCoreCompleteDelegate& OpenDelegate)
{
	if (OnFinalizeComplete.IsBound()) OnFinalizeComplete.Unbind();
	OnFinalizeComplete = OpenDelegate;
}

/**
 * @brief VOICEVOX CORE終了処理の結果をセット
 */
void UVoicevoxCoreSubsystem::SetFinalizeResult(const bool bIsSuccess)
{
	if (!bIsSuccess)
	{
		if (!OnFinalizeComplete.ExecuteIfBound(false))
		{
			UE_LOG(LogTemp, Display, TEXT("OnFinalizeComplete Not Execute!!"));
		}

		return;
	}

	MetaList.Empty();
	SupportedDevicesMap.Empty();
	VoicevoxCoreVersionMap.Empty();
	CoreNameList.Empty();
	
	if (!OnFinalizeComplete.ExecuteIfBound(true))
	{
		UE_LOG(LogTemp, Display, TEXT("OnFinalizeComplete Not Execute!!"));
	}
}

//--------------------------------
// VOICEVOX CORE LoadModel関連
//--------------------------------

/**
 * @brief モデルをロードする。
 */
void UVoicevoxCoreSubsystem::LoadModel(const int64 SpeakerId) const
{
	NativeInstance->LoadModel(SpeakerId);
}

/**
 * @brief VOICEVOX COREモデル読み込み処理完了のデリゲート関数登録
 */
void UVoicevoxCoreSubsystem::SetOnLoadModelCompleteDelegate(const FVoicevoxCoreCompleteDelegate& OpenDelegate)
{
	if (OnLodeModelComplete.IsBound()) OnLodeModelComplete.Unbind();
	OnLodeModelComplete = OpenDelegate;
}

/**
 * @brief VOICEVOX COREモデル読み込みの結果をセット
 */
void UVoicevoxCoreSubsystem::SetLodeModelResult(const bool bIsSuccess) const
{
	if (!bIsSuccess)
	{
		if (!OnLodeModelComplete.ExecuteIfBound(false))
		{
			UE_LOG(LogTemp, Display, TEXT("OnModelLodeComplete Not Execute!!"));
		}

		return;
	}

	if (!OnLodeModelComplete.ExecuteIfBound(true))
	{
		UE_LOG(LogTemp, Display, TEXT("OnModelLodeComplete Not Execute!!"));
	}
}

//--------------------------------
// VOICEVOX CORE AudioQuery関連
//--------------------------------

/**
 * @brief AudioQuery を取得する。
 */
FVoicevoxAudioQuery UVoicevoxCoreSubsystem::GetAudioQuery(int64 SpeakerId, const FString& Message, bool bKana) const
{
	return NativeInstance->GetAudioQuery(SpeakerId, Message, bKana);
}

//--------------------------------
// VOICEVOX CORE TextToSpeech関連
//--------------------------------

/**
 * @brief VOICEVOX COREのtext to speechを実行
 */
TArray<uint8> UVoicevoxCoreSubsystem::RunTextToSpeech(const int64 SpeakerId, const FString& Message, const bool bKana, const bool bEnableInterrogativeUpspeak) const
{
	return NativeInstance->RunTextToSpeech(SpeakerId, Message, bKana, bEnableInterrogativeUpspeak);
}

//--------------------------------
// VOICEVOX CORE Synthesis関連
//--------------------------------

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UVoicevoxCoreSubsystem::RunSynthesis(const char* AudioQueryJson, const int64 SpeakerId, bool bEnableInterrogativeUpspeak) const
{
	return NativeInstance->RunSynthesis(AudioQueryJson, SpeakerId,  bEnableInterrogativeUpspeak);
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UVoicevoxCoreSubsystem::RunSynthesis(const FVoicevoxAudioQuery& AudioQuery, const int64 SpeakerId, const bool bEnableInterrogativeUpspeak) const
{
	return NativeInstance->RunSynthesis(AudioQuery, SpeakerId,  bEnableInterrogativeUpspeak);
}

/**
 * @brief AudioQueryアセットデータを音声データに変換する。
 */
TArray<uint8> UVoicevoxCoreSubsystem::RunSynthesis(const UVoicevoxQuery& VoicevoxQuery, bool bEnableInterrogativeUpspeak) const
{
	return NativeInstance->RunSynthesis(VoicevoxQuery.VoicevoxAudioQuery, VoicevoxQuery.SpeakerType,  bEnableInterrogativeUpspeak);
}

//--------------------------------
// VOICEVOX CORE LipSync関連
//--------------------------------
	
/**
 * @brief VOICEVOX COREで取得したAudioQuery元に、中品質なLipSyncに必要なデータリストを取得
 */
TArray<FVoicevoxLipSync> UVoicevoxCoreSubsystem::GetLipSyncList(FVoicevoxAudioQuery AudioQuery)
{
	TArray<FVoicevoxLipSync> List;
	List.Empty();

	TMap<FString, ELipSyncVowelType> FruitMap =
	{
		{TEXT("a"), ELipSyncVowelType::A},
		{TEXT("i"), ELipSyncVowelType::I},
		{TEXT("u"), ELipSyncVowelType::U},
		{TEXT("e"), ELipSyncVowelType::E},
		{TEXT("o"), ELipSyncVowelType::O},
		{TEXT("cl"), ELipSyncVowelType::U},
		{TEXT("N"), ELipSyncVowelType::Non},
		{TEXT("pau"), ELipSyncVowelType::Non},
	};
	
	for (auto [Moras, Accent, Pause_mora, Is_interrogative] : AudioQuery.Accent_phrases)
	{
		for (auto [Text, Consonant, Consonant_length, Vowel, Vowel_length, Pitch] : Moras)
		{
			List.Add({FruitMap[Vowel], Vowel_length + Consonant_length});
		}

		if (Pause_mora.Vowel.Equals(TEXT("pau"), ESearchCase::IgnoreCase))
		{
			List.Add({ELipSyncVowelType::Non, Pause_mora.Vowel_length});
		}
	}
	
	return List;
}

//--------------------------------
// VOICEVOX CORE Meta関連
//--------------------------------

/**
 * @brief 話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> UVoicevoxCoreSubsystem::GetMetaList()
{
	return MetaList;
}

/**
 * @brief 指定したSpeakerIDの名前を取得する
 */
FString UVoicevoxCoreSubsystem::GetMetaName(const int64 SpeakerID)
{
	TArray<FVoicevoxMeta> List = GetMetaList();
	for  (auto [Name, Styles, Speaker_uuid, Version] : List)
	{
		for (const auto Style :Styles)
		{
			if (Style.Id == SpeakerID)
			{
				return FString::Printf(TEXT("%s(%s)"), *Name, *Style.Name);
			}
		}
	}
	
	return TEXT("");
}

//--------------------------------
// VOICEVOX CORE Version関連
//--------------------------------

/**
 * @brief VOICEVOX COREのバージョンを取得する
 */
FString UVoicevoxCoreSubsystem::GetVoicevoxVersion(const FString& CoreName)
{
	return VoicevoxCoreVersionMap[CoreName];
}

//--------------------------------
// VOICEVOX CORE GpuMode関連
//--------------------------------

/**
 * @brief ハードウェアアクセラレーションがGPUモードか判定する
 */
bool UVoicevoxCoreSubsystem::IsGpuMode(const FString& CoreName)
{
	return IsGpuModeMap[CoreName];
}

//--------------------------------
// VOICEVOX CORE SupportedDevices関連
//--------------------------------

/**
 * @brief サポートデバイス情報を取得する
 */
FVoicevoxSupportedDevices UVoicevoxCoreSubsystem::GetSupportedDevices(const FString& CoreName)
{
	return SupportedDevicesMap[CoreName];
}

//--------------------------------
// VOICEVOX CORE Property関連
//--------------------------------

/**
 * @brief 各VOICEVOX COREの話者名や話者IDのリスト、サポートデバイス、バージョン情報を各変数へ追加
 */
void UVoicevoxCoreSubsystem::AddVoicevoxConfigData(const FString& CoreName, TArray<FVoicevoxMeta> List, FVoicevoxSupportedDevices SupportedDevices, const FString& Version, const bool bIsGpuMode)
{
	for (auto Element : List)
	{
		MetaList.Emplace(Element);
	}
	CoreNameList.Add(CoreName);
	SupportedDevicesMap.Add(CoreName, SupportedDevices);
	VoicevoxCoreVersionMap.Add(CoreName, Version);
	IsGpuModeMap.Add(CoreName, bIsGpuMode);
}

//--------------------------------
// VOICEVOX CORE プラグイン名取得
//--------------------------------

/**
 * @brief 初期化済みのネイティブコア名を取得
 */
TArray<FString> UVoicevoxCoreSubsystem::GetCoreNameList()
{
	return CoreNameList;
}

//--------------------------------
// VOICEVOX CORE PhonemeLength関連
//--------------------------------

/**
 * @brief 音素列から、音素ごとの長さを求める
 */
TArray<float> UVoicevoxCoreSubsystem::GetPhonemeLength(int64 Length, TArray<int64> PhonemeList, int64 SpeakerID) const
{
	return NativeInstance->GetPhonemeLength(Length, PhonemeList,  SpeakerID);
}

//--------------------------------
// VOICEVOX CORE Mora関連
//--------------------------------

/**
 * @brief モーラごとの音素列とアクセント情報から、モーラごとの音高を求める
 */
TArray<float> UVoicevoxCoreSubsystem::FindPitchEachMora(int64 Length, TArray<int64> VowelPhonemeList, TArray<int64> ConsonantPhonemeList,
										  TArray<int64> StartAccentList, TArray<int64> EndAccentList,
										  TArray<int64> StartAccentPhraseList, TArray<int64> EndAccentPhraseList,
										  int64 SpeakerID) const
{
	return NativeInstance->FindPitchEachMora(Length, VowelPhonemeList,  ConsonantPhonemeList, StartAccentList, EndAccentList, StartAccentPhraseList, EndAccentPhraseList, SpeakerID);
}

//--------------------------------
// VOICEVOX CORE DecodeForward関連
//--------------------------------
	
/**
 * @brief フレームごとの音素と音高から、波形を求める
 */
TArray<float> UVoicevoxCoreSubsystem::DecodeForward(int64 Length, int64 PhonemeSize, TArray<float> F0, TArray<float> Phoneme, int64 SpeakerID) const
{
	return NativeInstance->DecodeForward(Length, PhonemeSize, F0, Phoneme, SpeakerID);
}