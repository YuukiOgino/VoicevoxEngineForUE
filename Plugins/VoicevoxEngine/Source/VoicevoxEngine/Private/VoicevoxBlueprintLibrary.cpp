// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOX COREのAPIへ接続するBlueprint公開ノードをまとめたCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxBlueprintLibrary.h"
#include <Sound/SoundWaveProcedural.h>
#include "VoicevoxApiDefined.h"
#include "Audio.h"
#include "Engine/Engine.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"

/**
 * @brief 全てのVOICEVOX CORE 初期化が完了しているか
 */
bool UVoicevoxBlueprintLibrary::IsInitialize()
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetIsInitialize();
}

/**
 * @brief VOICEVOX CORE 終了処理(Blueprint公開ノード)
 */
void UVoicevoxBlueprintLibrary::Finalize()
{
	GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
}

/**
 * @brief 日本語のテキストを解析する。
 */
TArray<FVoicevoxAccentPhrase> UVoicevoxBlueprintLibrary::OpenJTalkRcAnalyze(const FString& Text)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->OpenJTalkRcAnalyze(Text);
}

/**
 * @breaf 音声モデルの読み込みを解除する。
 */
 bool UVoicevoxBlueprintLibrary::UnloadVoiceModel(const FString VvmFileName)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->UnloadVoiceModel(VvmFileName);
}

/**
 * @breaf 指定したIDの音声モデルが読み込まれているか判定する。
 */
bool UVoicevoxBlueprintLibrary::IsLoadedVoiceModel(const FString VvmFileName)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->IsLoadedVoiceModel(VvmFileName);
}

/**
 * @brief 初期化済みのVOICEVOX CORE名のリスト取得
 */
TArray<FString> UVoicevoxBlueprintLibrary::GetCoreNameList()
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetCoreNameList();
}

/**
 * @brief VOICEVOX CORE メタ情報を取得する(Blueprint公開ノード)
 */
TArray<FVoicevoxMeta> UVoicevoxBlueprintLibrary::GetMetasToList()
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetMetaList();
}

/**
 * @brief 指定のVoicevoxVoiceModelFileからメタ情報を取得する
 */
TArray<FVoicevoxMeta> UVoicevoxBlueprintLibrary::GetVoiceModelFileMetaList(const FString& VvmFileName)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetVoiceModelFileMetaList(VvmFileName);
}

/**
 * @brief 全てのVoicevoxVoiceModelFileからメタ情報を取得する
 */
TArray<FVoicevoxMeta> UVoicevoxBlueprintLibrary::GetAllVoiceModelFileMetaList()
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAllVoiceModelFileMetaList();
}

/**
 * @brief サポートデバイス情報を取得する(Blueprint公開ノード)
 */
FVoicevoxSupportedDevices UVoicevoxBlueprintLibrary::GetSupportedDevices(const FString& CoreName)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetSupportedDevices(CoreName);
}

/**
 * @brief VOICEVOX COREのバージョン情報を取得する
 */
FString UVoicevoxBlueprintLibrary::GetVoicevoxCoreVersion(const FString& CoreName)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetVoicevoxVersion(CoreName);
}

/**
 * @brief ハードウェアアクセラレーションがGPUモードか判定する
 */
bool UVoicevoxBlueprintLibrary::IsVoicevoxGpuMode(const FString& CoreName)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->IsGpuMode(CoreName);
}

/**
 * @brief VOICEVOX COREで変換した音声データを元にSoundWaveを生成(Blueprint公開ノード)
 */
USoundWave* UVoicevoxBlueprintLibrary::TextToSpeechOutput(const int32 StyleId, const FString Message, const bool bRunKana, const bool bEnableInterrogativeUpspeak)
{
	if (const TArray<uint8> OutputWAV = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->RunTextToSpeech(StyleId, Message, bRunKana, bEnableInterrogativeUpspeak);
		!OutputWAV.IsEmpty())
	{
		return CreateSoundWave(OutputWAV);
	}
	
	return nullptr;
}

/**
 * @brief 入力したテキストをVOICEVOX COREでAudioQueryに変換後、SoundWaveを生成(Blueprint公開ノード)
 */
USoundWave* UVoicevoxBlueprintLibrary::TextToAudioQueryOutput(const int32 StyleId, const FString Message, const bool bRunKana, const bool bEnableInterrogativeUpspeak)
{
	const FVoicevoxAudioQuery AudioQuery = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAudioQuery(StyleId, Message, bRunKana);

	if (const TArray<uint8> OutputWAV = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->RunSynthesis(AudioQuery, StyleId, bEnableInterrogativeUpspeak); !OutputWAV.IsEmpty())
	{
		return CreateSoundWave(OutputWAV);
	}
	return nullptr;
}

/**
 * @brief  VOICEVOX COREで変換したAudioQueryを取得する(Blueprint公開ノード)
 */
FVoicevoxAudioQuery UVoicevoxBlueprintLibrary::GetAudioQuery(const int32 StyleId, const FString Message, const bool bRunKana)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAudioQuery(StyleId, Message, bRunKana);
}

/**
 * @brief AccentPhraseの配列からAudioQueryを作る(Blueprint公開ノード)
 */
FVoicevoxAudioQuery UVoicevoxBlueprintLibrary::GetAudioQueryFromAccentPhrases(const TArray<FVoicevoxAccentPhrase>& AccentPhrases)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAudioQueryFromAccentPhrases(AccentPhrases);
}

/**
 * @brief 日本語テキストから、AccentPhrase (アクセント句)の配列を生成する。(Blueprint公開ノード)
 */
TArray<FVoicevoxAccentPhrase> UVoicevoxBlueprintLibrary::GetSynthesizerAccentPhrase(const int32 StyleId, const FString& Text, const bool bKana)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SynthesizerCreateAccentPhrases(StyleId, Text, bKana);
}

/**
 * @brief AccentPhraseの配列の指定パラメータを、特定の声で生成しなおす。(Blueprint公開ノード)
 */
TArray<FVoicevoxAccentPhrase> UVoicevoxBlueprintLibrary::SynthesizerReplace(const EReplaceType ReplaceType, const TArray<FVoicevoxAccentPhrase>& AccentPhrases, const int32 StyleId)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->SynthesizerReplace(ReplaceType, AccentPhrases, StyleId);
}

/**
 * @brief VOICEVOX COREで取得したAudioQuery元にSoundWaveを作成(Blueprint公開ノード)
 */
USoundWave* UVoicevoxBlueprintLibrary::AudioQueryOutput(const FVoicevoxAudioQuery AudioQuery, const int32 StyleId, const bool bEnableInterrogativeUpspeak)
{
	if (const TArray<uint8> OutputWAV = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->RunSynthesis(AudioQuery, StyleId, bEnableInterrogativeUpspeak); !OutputWAV.IsEmpty())
	{
		return CreateSoundWave(OutputWAV);
	}

	return nullptr;
}

/**
 * @brief AudioQueryアセットからSoundWaveを作成(Blueprint公開ノード)
 * @param[in] VoicevoxQuery						Queryアセット
 * @param[in] bEnableInterrogativeUpspeak		疑問文の調整を有効にする
 * @return AudioQuery情報を元に作成された音楽データが格納されたUSoundWave
 */
USoundWave* UVoicevoxBlueprintLibrary::VoicevoxQueryOutput(UVoicevoxQuery* VoicevoxQuery, const bool bEnableInterrogativeUpspeak)
{
	if (VoicevoxQuery == nullptr) return nullptr;
	
	if (const TArray<uint8> OutputWAV = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->RunSynthesis(*VoicevoxQuery, bEnableInterrogativeUpspeak); !OutputWAV.IsEmpty())
	{
		return CreateSoundWave(OutputWAV);
	}

	return nullptr;
}

/**
 * @brief 生成した音声データからUSoundWaveを作成
 */
USoundWave* UVoicevoxBlueprintLibrary::CreateSoundWave(TArray<uint8> PCMData)
{
	FString ErrorMessage = "";
	
	if (FWaveModInfo WaveInfo; WaveInfo.ReadWaveInfo(PCMData.GetData(), PCMData.Num(), &ErrorMessage))
	{
		USoundWaveProcedural* Sound = NewObject<USoundWaveProcedural>(USoundWaveProcedural::StaticClass());
		const int32 ChannelCount = *WaveInfo.pChannels;
		const int32 SizeOfSample = *WaveInfo.pBitsPerSample / 8;
		const int32 NumSamples = WaveInfo.SampleDataSize / SizeOfSample;
		const int32 NumFrames = NumSamples / ChannelCount;
		
		Sound->RawPCMDataSize = WaveInfo.SampleDataSize;
		Sound->QueueAudio(WaveInfo.SampleDataStart, WaveInfo.SampleDataSize);
		
		Sound->Duration = static_cast<float>(NumFrames) / *WaveInfo.pSamplesPerSec;
		Sound->SetSampleRate(*WaveInfo.pSamplesPerSec);
		Sound->NumChannels = ChannelCount;
		Sound->TotalSamples = *WaveInfo.pSamplesPerSec * Sound->Duration;
		Sound->SoundGroup = SOUNDGROUP_Default;
		
		return Sound;
	}

	return nullptr;
}

/**
 * @brief VOICEVOX COREで取得したAudioQuery元に、中品質なLipSyncに必要なデータリストを取得(Blueprint公開ノード)
 */
TArray<FVoicevoxLipSync> UVoicevoxBlueprintLibrary::GetLipSyncList(const FVoicevoxAudioQuery AudioQuery)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetLipSyncList(AudioQuery);
}

//--------------------------------
// VOICEVOX CORE Dict関連
//--------------------------------
	
/**
 * @brief VoicevoxUserDictWordを最低限のパラメータで作成する。(Blueprint公開ノード)
 */
FVoicevoxCoreUserDictWord UVoicevoxBlueprintLibrary::UserDictWordMake(const FString& Surface, const FString& Pronunciation, const int64 AccentType)
{
	FVoicevoxCoreUserDictWord DictWord;
	auto [surface, pronunciation, accent_type, word_type, priority] = 
		GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->UserDictWordMake(Surface, Pronunciation, static_cast<uintptr_t>(AccentType));
	DictWord.Pronunciation = UTF8_TO_TCHAR(pronunciation);
	DictWord.Surface = UTF8_TO_TCHAR(surface);
	DictWord.Priority = static_cast<int>(priority);
	DictWord.AccentType = static_cast<int64>(accent_type);
	DictWord.WordType = static_cast<EVoicevoxCoreUserDictWordType>(word_type);
	return DictWord;
}

/**
 * @brief ユーザー辞書を構築する。(Blueprint公開ノード)
 */
bool UVoicevoxBlueprintLibrary::UserDictInitialize(const FString& DictPath)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->UserDictInitialize(DictPath);
}
	
/**
 * @brief ユーザー辞書に単語を追加する。(Blueprint公開ノード)
 */
TArray<uint8> UVoicevoxBlueprintLibrary::UserDictAddWord(const FVoicevoxCoreUserDictWord Word)
{
	VoicevoxUserDictWord DictWord;
	DictWord.priority = static_cast<uint32_t>(Word.Priority);
	DictWord.accent_type = static_cast<uintptr_t>(Word.AccentType);
	DictWord.pronunciation = TCHAR_TO_UTF8(*Word.Pronunciation);
	DictWord.surface = TCHAR_TO_UTF8(*Word.Surface);
	DictWord.word_type = static_cast<VoicevoxUserDictWordType>(Word.WordType);
	TArray<uint8_t> UUID = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->UserDictAddWord(&DictWord);
	return UUID;
}

/**
 * @brief ユーザー辞書の単語を更新する。(Blueprint公開ノード)
 */
bool UVoicevoxBlueprintLibrary::RewriteUserDictWord(const TArray<uint8>& WordUuid, const FVoicevoxCoreUserDictWord Word)
{
	VoicevoxUserDictWord DictWord;
	DictWord.priority = static_cast<uint32_t>(Word.Priority);
	DictWord.accent_type = static_cast<uintptr_t>(Word.AccentType);
	DictWord.pronunciation = TCHAR_TO_UTF8(*Word.Pronunciation);
	DictWord.surface = TCHAR_TO_UTF8(*Word.Surface);
	DictWord.word_type = static_cast<VoicevoxUserDictWordType>(Word.WordType);
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->RewriteUserDictWord(WordUuid, &DictWord);
}

/**
 * @brief ユーザー辞書から単語を削除する。(Blueprint公開ノード)
 */
bool UVoicevoxBlueprintLibrary::DeleteUserDictWord(const TArray<uint8>& WordUuid)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->DeleteUserDictWord(WordUuid);
}
	
/**
 * @brief ユーザー辞書の単語を取得する。(Blueprint公開ノード)
 * @returns 登録したユーザー辞書の単語マップ
 */
TMap<FString, FVoicevoxRegisteredUserDictWord> UVoicevoxBlueprintLibrary::GetUserDictWord()
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetUserDictWord();
}

/**
 * @brief ユーザー辞書をファイルに保存する。(Blueprint公開ノード)
 */
bool UVoicevoxBlueprintLibrary::UserDictSave(const FString& Path)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->UserDictSave(Path);
}
	
/**
 * @brief ユーザー辞書を破棄する。(Blueprint公開ノード)
 */
void UVoicevoxBlueprintLibrary::UserDictDelete()
{
	GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->UserDictDelete();
}