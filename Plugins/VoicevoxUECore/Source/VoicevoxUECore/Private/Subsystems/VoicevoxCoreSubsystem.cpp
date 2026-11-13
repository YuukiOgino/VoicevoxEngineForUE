// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  各VOICEVOX COREのAPIを呼び出すSubsystem　CPPファイル
 * @author Yuuki Ogino
 */
#include "Subsystems/VoicevoxCoreSubsystem.h"
#include "VoicevoxApiObject.h"
#include "Utilities/VoicevoxUserDictUUIDConverter.h"

//--------------------------------
// override
//--------------------------------

/**
 * @brief Initialize
 */
void UVoicevoxCoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UClass* NativeClass = UVoicevoxApiObject::StaticClass();
	NativeInstance = NewObject<UVoicevoxApiObject>(this, NativeClass);
}

/**
 * @brief Deinitialize
 */
void UVoicevoxCoreSubsystem::Deinitialize()
{
	Super::Deinitialize();

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
bool UVoicevoxCoreSubsystem::Initialize(const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
{
	SupportedDevicesMap.Empty();
	VoicevoxCoreVersionMap.Empty();
	CoreNameList.Empty();

	bIsInitialized = NativeInstance->CoreInitialize(bUseGPU, CPUNumThreads, bLoadAllModels);
	return bIsInitialized;
}

/**
 * @brief 全てのVOICEVOX CORE 初期化が完了しているか
 */
bool UVoicevoxCoreSubsystem::GetIsInitialize() const
{
	return bIsInitialized;	
}

//--------------------------------
// VOICEVOX CORE Finalize関連
//--------------------------------

/**
 * @brief VOICEVOX CORE 終了処理
 */
void UVoicevoxCoreSubsystem::Finalize()
{
	SupportedDevicesMap.Empty();
	VoicevoxCoreVersionMap.Empty();
	CoreNameList.Empty();
	
	NativeInstance->Finalize();
}

//--------------------------------
// VOICEVOX CORE OpenJtake関連
//--------------------------------
	
/**
 * @brief 日本語のテキストを解析する。
 */
TArray<FVoicevoxAccentPhrase> UVoicevoxCoreSubsystem::OpenJTalkRcAnalyze(const FString& Text) const
{
	return NativeInstance->OpenJTalkRcAnalyze(Text);
}

//--------------------------------
// VOICEVOX CORE LoadModel関連
//--------------------------------

/**
 * @brief　VVMファイルのロード実行
 */
bool UVoicevoxCoreSubsystem::LoadVoiceModel(const FString& VvmFileName) const
{
	return NativeInstance->LoadVoiceModel(VvmFileName);
}

/**
 * @brief　全てのVVMファイルを開く。
 * @returns 読み込み結果
 */
bool UVoicevoxCoreSubsystem::AllLoadVoiceModel() const
{
	return NativeInstance->AllLoadVoiceModel();
}

/**
 * @brief モデルをロードする。
 */
bool UVoicevoxCoreSubsystem::LoadModel(const VoicevoxStyleId StyleId) const
{
	return NativeInstance->LoadModel(StyleId);
}

/**
 * @breaf 音声モデルの読み込みを解除する。
 */
bool UVoicevoxCoreSubsystem::UnloadVoiceModel(const FString& VvmFileName) const
{
	return NativeInstance->UnloadVoiceModel(VvmFileName);
}

/**
 * @breaf 指定したvvmファイル名が読み込まれているか判定する。
 */
bool UVoicevoxCoreSubsystem::IsLoadedVoiceModel(const FString& VvmFileName) const
{
	return NativeInstance->IsLoadedVoiceModel(VvmFileName);
}

//--------------------------------
// VOICEVOX CORE AudioQuery関連
//--------------------------------

/**
 * @brief AudioQuery を取得する。
 */
FVoicevoxAudioQuery UVoicevoxCoreSubsystem::GetAudioQuery(const VoicevoxStyleId StyleId, const FString& Message, const bool bKana) const
{
	return NativeInstance->GetAudioQuery(StyleId, Message, bKana);
}

/**
 * @brief AccentPhraseの配列からAudioQueryを作る。
 */
FVoicevoxAudioQuery UVoicevoxCoreSubsystem::GetAudioQueryFromAccentPhrases(const TArray<FVoicevoxAccentPhrase>& AccentPhrases) const
{
	return NativeInstance->GetAudioQueryFromAccentPhrases(AccentPhrases);
}

//--------------------------------
// VOICEVOX CORE TextToSpeech関連
//--------------------------------

/**
 * @brief VOICEVOX COREのtext to speechを実行
 */
TArray<uint8> UVoicevoxCoreSubsystem::RunTextToSpeech(const VoicevoxStyleId StyleId, const FString& Message, const bool bKana, const bool bEnableInterrogativeUpspeak) const
{
	return NativeInstance->RunTextToSpeech(StyleId, Message, bKana, bEnableInterrogativeUpspeak);
}

//--------------------------------
// VOICEVOX CORE Synthesis関連
//--------------------------------

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UVoicevoxCoreSubsystem::RunSynthesis(const char* AudioQueryJson, const VoicevoxStyleId StyleId, const bool bEnableInterrogativeUpspeak) const
{
	return NativeInstance->RunSynthesis(AudioQueryJson, StyleId, bEnableInterrogativeUpspeak);
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UVoicevoxCoreSubsystem::RunSynthesis(const FVoicevoxAudioQuery& AudioQuery, const VoicevoxStyleId StyleId, const bool bEnableInterrogativeUpspeak) const
{
	return NativeInstance->RunSynthesis(AudioQuery, StyleId, bEnableInterrogativeUpspeak);
}

/**
 * @brief AudioQueryアセットデータを音声データに変換する。
 */
TArray<uint8> UVoicevoxCoreSubsystem::RunSynthesis(const UVoicevoxQuery& VoicevoxQuery, const bool bEnableInterrogativeUpspeak) const
{
	return NativeInstance->RunSynthesis(VoicevoxQuery.VoicevoxAudioQuery, VoicevoxQuery.StyleId, bEnableInterrogativeUpspeak);
}

/**
 * @brief 日本語テキストから、AccentPhrase (アクセント句)の配列を生成する。
 */
TArray<FVoicevoxAccentPhrase> UVoicevoxCoreSubsystem::SynthesizerCreateAccentPhrases(const VoicevoxStyleId StyleId, const FString& Text, const bool bKana) const
{
	return NativeInstance->SynthesizerCreateAccentPhrases(StyleId, Text, bKana);
}

/**
 * @brief AccentPhraseの配列の指定パラメータを、特定の声で生成しなおす。
 */
TArray<FVoicevoxAccentPhrase> UVoicevoxCoreSubsystem::SynthesizerReplace(const EReplaceType ReplaceType, const TArray<FVoicevoxAccentPhrase>& AccentPhrases, const VoicevoxStyleId StyleId) const
{
	return NativeInstance->SynthesizerReplace(ReplaceType, AccentPhrases, StyleId);
}

//--------------------------------
// VOICEVOX CORE Sing関連
//--------------------------------

/**
 * @brief 楽譜から歌唱音声合成用のクエリを作成する。
 */
FVoicevoxFrameAudioQuery UVoicevoxCoreSubsystem::SynthesizerCreateSingFrameAudioQuery(const FVoicevoxScore& Score, const VoicevoxStyleId StyleId) const
{
	return NativeInstance->SynthesizerCreateSingFrameAudioQuery(Score, StyleId);
}

/**
 * @brief 楽譜から歌唱音声合成用のクエリを作成する。
 */
FVoicevoxFrameAudioQuery UVoicevoxCoreSubsystem::SynthesizerCreateSingFrameAudioQuery(const UVoicevoxSingScore& Score, const VoicevoxStyleId StyleId) const
{
	return NativeInstance->SynthesizerCreateSingFrameAudioQuery(Score.Score, StyleId);
}

/**
 * @brief 楽譜と歌唱音声合成用のクエリから、フレームごとの基本周波数を生成する。
 */
TArray<float> UVoicevoxCoreSubsystem::SynthesizerCreateSingFrameF0(const FVoicevoxScore& Score, const FVoicevoxFrameAudioQuery& FrameAudioQuery, VoicevoxStyleId StyleId) const
{
	return NativeInstance->SynthesizerCreateSingFrameF0(Score, FrameAudioQuery, StyleId);
}

/**
 * @brief 楽譜と歌唱音声合成用のクエリから、フレームごとの基本周波数を生成する。
 */
TArray<float> UVoicevoxCoreSubsystem::SynthesizerCreateSingFrameF0(const FVoicevoxScore& Score, const UVoicevoxSingFrameQuery& FrameAudioQuery) const
{
	return NativeInstance->SynthesizerCreateSingFrameF0(Score, FrameAudioQuery.FrameAudioQuery, FrameAudioQuery.StyleId);
}

/**
 * @brief 楽譜と歌唱音声合成用のクエリから、フレームごとの基本周波数を生成する。
 */
TArray<float> UVoicevoxCoreSubsystem::SynthesizerCreateSingFrameF0(const UVoicevoxSingScore& Score, const UVoicevoxSingFrameQuery& FrameAudioQuery) const
{
	return NativeInstance->SynthesizerCreateSingFrameF0(Score.Score, FrameAudioQuery.FrameAudioQuery, FrameAudioQuery.StyleId);
}

/**
 * @brief 楽譜と歌唱音声合成用のクエリから、フレームごとの音量を生成する。
 */
TArray<float> UVoicevoxCoreSubsystem::SynthesizerCreateSingFrameVolume(const FVoicevoxScore& Score, const FVoicevoxFrameAudioQuery& FrameAudioQuery, VoicevoxStyleId StyleId) const
{
	return NativeInstance->SynthesizerCreateSingFrameVolume(Score, FrameAudioQuery, StyleId);
}

/**
 * @brief 楽譜と歌唱音声合成用のクエリから、フレームごとの音量を生成する。
 */
TArray<float> UVoicevoxCoreSubsystem::SynthesizerCreateSingFrameVolume(const FVoicevoxScore& Score, const UVoicevoxSingFrameQuery& FrameAudioQuery) const
{
	return NativeInstance->SynthesizerCreateSingFrameVolume(Score, FrameAudioQuery.FrameAudioQuery, FrameAudioQuery.StyleId);
}

/**
 * @brief 楽譜と歌唱音声合成用のクエリから、フレームごとの音量を生成する。
 */
TArray<float> UVoicevoxCoreSubsystem::SynthesizerCreateSingFrameVolume(const UVoicevoxSingScore& Score, const UVoicevoxSingFrameQuery& FrameAudioQuery) const
{
	return NativeInstance->SynthesizerCreateSingFrameVolume(Score.Score, FrameAudioQuery.FrameAudioQuery, FrameAudioQuery.StyleId);
}

/**
 * @brief 歌唱音声合成を行う。
 */
TArray<uint8> UVoicevoxCoreSubsystem::SynthesizerFrameSynthesis(const FVoicevoxFrameAudioQuery& FrameAudioQuery, const VoicevoxStyleId StyleId) const
{
	return NativeInstance->SynthesizerFrameSynthesis(FrameAudioQuery, StyleId);
}

/**
 * @brief 歌唱音声合成を行う。
 */
TArray<uint8> UVoicevoxCoreSubsystem::SynthesizerFrameSynthesis(const UVoicevoxSingFrameQuery& FrameAudioQuery) const
{
	return NativeInstance->SynthesizerFrameSynthesis(FrameAudioQuery.FrameAudioQuery, FrameAudioQuery.StyleId);
}

//--------------------------------
// VOICEVOX CORE validate関連
//--------------------------------
	
/**
 * @brief 与えられたJSONが指定した型として不正であるときエラーを返す。
 */
VoicevoxResultCode UVoicevoxCoreSubsystem::VoicevoxValidate(const EVoicevoxCoreValidateType Type, const FString& Json) const
{
	return NativeInstance->VoicevoxValidate(Type, Json);
}

/**
 * @brief 与えられた2つのJSONの組み合わせが不正であるときエラーを返す。
 */
VoicevoxResultCode UVoicevoxCoreSubsystem::VoicevoxValidate(EVoicevoxCoreMultiValidateType Type, const FString& Json, const FString& Json2) const
{
	return NativeInstance->VoicevoxValidate(Type, Json, Json2);
}

//--------------------------------
// VOICEVOX CORE LipSync関連
//--------------------------------
	
/**
 * @brief VOICEVOX COREで取得したAudioQuery元に、中品質なLipSyncに必要なデータリストを取得
 */
TArray<FVoicevoxLipSync> UVoicevoxCoreSubsystem::GetLipSyncList(FVoicevoxAudioQuery AudioQuery, bool bIsSimple, float PitchModulation)
{
	TArray<FVoicevoxLipSync> List;
	List.Empty();

	TMap<FString, ELipSyncVowelType> FruitMap;
	FruitMap.Reserve(8);
	FruitMap.Add(TEXT("a"), ELipSyncVowelType::A);
	FruitMap.Add(TEXT("i"), ELipSyncVowelType::I);
	FruitMap.Add(TEXT("u"), ELipSyncVowelType::U);
	FruitMap.Add(TEXT("e"), ELipSyncVowelType::E);
	FruitMap.Add(TEXT("o"), ELipSyncVowelType::O);
	FruitMap.Add(TEXT("cl"), ELipSyncVowelType::CL);
	FruitMap.Add(TEXT("N"), ELipSyncVowelType::Non);
	FruitMap.Add(TEXT("pau"), ELipSyncVowelType::Non);

	List.Add({ELipSyncVowelType::Non, AudioQuery.PrePhonemeLength / AudioQuery.SpeedScale, false, false});
	
	for (auto [Moras, Accent, Pause_mora, Is_interrogative] : AudioQuery.Accent_Phrases)
	{
		for (auto [Text, Consonant, Consonant_length, Vowel, Vowel_length, Pitch] : Moras)
		{
			if (bIsSimple)
			{
				List.Add({FruitMap[Vowel], Vowel_length / AudioQuery.SpeedScale + Consonant_length / AudioQuery.SpeedScale });
			}
			else
			{
				if (!Consonant.IsEmpty())
				{
					FString C = Consonant.ToLower();
					bool IsLabialOrPlosive = C.Equals(TEXT("w")) || C.Equals(TEXT("m")) || C.Equals(TEXT("b")) || C.Equals(TEXT("p")) || C.Equals(TEXT("f")) || C.Equals(TEXT("v"));
					List.Add({FruitMap[Vowel], Consonant_length / AudioQuery.SpeedScale, true, IsLabialOrPlosive});
				}
				List.Add({FruitMap[Vowel], Vowel_length / AudioQuery.SpeedScale, false, false});
			}
			
		}

		if (Pause_mora.Vowel.Equals(TEXT("pau"), ESearchCase::IgnoreCase))
		{
			List.Add({.VowelType = ELipSyncVowelType::Non, .Length = Pause_mora.Vowel_Length / AudioQuery.SpeedScale, .IsConsonant = false, .IsLabialOrPlosive = false});
		}
	}

	List.Add({.VowelType = ELipSyncVowelType::Non, .Length = AudioQuery.PostPhonemeLength / AudioQuery.SpeedScale, .IsConsonant = false, .IsLabialOrPlosive = false});
	
	return List;
}

//--------------------------------
// VOICEVOX CORE Meta関連
//--------------------------------

/**
 * @brief 話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> UVoicevoxCoreSubsystem::GetMetaList() const
{
	return NativeInstance->GetMetaList();
}

/**
 * @brief 指定のVoicevoxVoiceModelFileから話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> UVoicevoxCoreSubsystem::GetVoiceModelFileMetaList(const FString& VvmFileName) const
{
	return NativeInstance->GetVoiceModelFileMetaList(VvmFileName);
}

/**
 * @brief 全てのVoicevoxVoiceModelFileからメタ情報を取得する
 */
TArray<FVoicevoxMeta> UVoicevoxCoreSubsystem::GetAllVoiceModelFileMetaList(const EGetMetaType Type) const
{
	return NativeInstance->GetAllVoiceModelFileMetaList(Type);
}

/**
 * @brief 指定したStyleIDの名前を取得する
 */
FString UVoicevoxCoreSubsystem::GetMetaName(const VoicevoxStyleId StyleId) const
{
	for  (TArray<FVoicevoxMeta> List = GetAllVoiceModelFileMetaList(EGetMetaType::All);
		auto [Name, Styles, Speaker_uuid, Version] : List)
	{
		for (const auto Style :Styles)
		{
			if (Style.Id == StyleId)
			{
				return FString::Printf(TEXT("%s(%s)"), *Name, *Style.Name);
			}
		}
	}
	
	return TEXT("");
}

/**
 * @brief StyleIdをキーとした全てのVoicevoxVoiceModelFile名を格納したTMapを取得
 */
TMap<int, FString> UVoicevoxCoreSubsystem::GetVvmFileNameMapToStyleId() const
{
	return NativeInstance->GetVvmFileNameMapToStyleId();
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
void UVoicevoxCoreSubsystem::AddVoicevoxConfigData(const FString& CoreName, FVoicevoxSupportedDevices SupportedDevices, const FString& Version, const bool bIsGpuMode)
{
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
// VOICEVOX CORE Dict関連
//--------------------------------

/**
 * @brief VoicevoxUserDictWordを最低限のパラメータで作成する。
 */
VoicevoxUserDictWord UVoicevoxCoreSubsystem::UserDictWordMake(const FString& Surface, const FString& Pronunciation, const uintptr_t AccentType) const
{
	return NativeInstance->UserDictWordMake(Surface, Pronunciation, AccentType);
}
	
/**
 * @brief ユーザー辞書を構築する。
 */
bool UVoicevoxCoreSubsystem::UserDictInitialize(const FString& DictPath) const
{
	return NativeInstance->UserDictInitialize(DictPath);
}

/**
 * @brief ユーザー辞書アセットから単語を登録する
 */
TMap<FGuid, FVoicevoxCoreUserDictWord> UVoicevoxCoreSubsystem::UserDictAssetAddWord(UVoicevoxUserDictWord& UserDictWordAsset) const
{
	TMap<FGuid, FVoicevoxCoreUserDictWord> Map;
	for (auto Value : UserDictWordAsset.UserDictWordList)
	{
		VoicevoxUserDictWord Word
		{
			.surface = TCHAR_TO_UTF8(*Value.Surface), 
			.pronunciation = TCHAR_TO_UTF8(*Value.Pronunciation), 
			.accent_type = static_cast<uintptr_t>(Value.AccentType), 
			.word_type = static_cast<VoicevoxUserDictWordType>(Value.WordType), 
			.priority = static_cast<uint32_t>(Value.Priority)
		};
		
		Map.Add(FVoicevoxUserDictUUIDConverter::UUIDConverterToGuid(NativeInstance->UserDictAddWord(&Word)), Value);
	}
	
	return Map;
}

/**
 * @brief ユーザー辞書に単語を追加する。
 */
TArray<uint8_t> UVoicevoxCoreSubsystem::UserDictAddWord(const VoicevoxUserDictWord* Word) const
{
	return NativeInstance->UserDictAddWord(Word);
}

/**
 * @brief ユーザー辞書に単語を追加する。
 */
FGuid UVoicevoxCoreSubsystem::UserDictAddWordToCreateGuid(const VoicevoxUserDictWord* Word) const
{
	return FVoicevoxUserDictUUIDConverter::UUIDConverterToGuid(NativeInstance->UserDictAddWord(Word));
}

/**
 * @brief ユーザー辞書の単語を更新する。
 */
bool UVoicevoxCoreSubsystem::RewriteUserDictWord(const TArray<uint8_t>& WordUuid, const VoicevoxUserDictWord* Word) const
{
	return NativeInstance->RewriteUserDictWord(WordUuid, Word);
}

/**
 * @brief ユーザー辞書の単語を更新する。
 */
bool UVoicevoxCoreSubsystem::RewriteUserDictWord(const FGuid WordUuid, const VoicevoxUserDictWord* Word) const
{
	return NativeInstance->RewriteUserDictWord(FVoicevoxUserDictUUIDConverter::GuidConverterToUUID(WordUuid), Word);
}

/**
 * @brief ユーザー辞書から単語を削除する。
 */
bool UVoicevoxCoreSubsystem::DeleteUserDictWord(const TArray<uint8_t>& WordUuid) const
{
	return NativeInstance->DeleteUserDictWord(WordUuid);
}

/**
 * @brief ユーザー辞書から単語を削除する。
 *
 * @param [in] WordUuid 削除する単語のUUID
 * @returns 結果
 */
bool UVoicevoxCoreSubsystem::DeleteUserDictWord(const FGuid WordUuid) const
{
	return NativeInstance->DeleteUserDictWord(FVoicevoxUserDictUUIDConverter::GuidConverterToUUID(WordUuid));
}
	
/**
 * @brief ユーザー辞書の単語を取得する。
 */
TMap<FGuid, FVoicevoxRegisteredUserDictWord> UVoicevoxCoreSubsystem::GetUserDictWord() const
{
	return NativeInstance->GetUserDictWord();
}

/**
 * @brief ユーザー辞書をファイルに保存する。
 */
bool UVoicevoxCoreSubsystem::UserDictSave(const FString& Path) const
{
	return NativeInstance->UserDictSave(Path);
}
	
/**
 * @brief ユーザー辞書を破棄する。
 */
void UVoicevoxCoreSubsystem::UserDictDelete() const
{
	NativeInstance->UserDictDelete();
}
