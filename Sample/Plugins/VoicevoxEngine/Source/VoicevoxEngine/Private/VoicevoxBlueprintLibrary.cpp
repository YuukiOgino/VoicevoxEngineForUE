// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOX COREのAPIへ接続するBlueprint公開ノードをまとめたCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxBlueprintLibrary.h"
#include <Sound/SoundWaveProcedural.h>

#include "Subsystems/VoicevoxCoreSubsystem.h"

/**
 * @brief VOICEVOX CORE 終了処理(Blueprint公開ノード)
 */
void UVoicevoxBlueprintLibrary::Finalize()
{
	GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Finalize();
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
USoundWave* UVoicevoxBlueprintLibrary::TextToSpeechOutput(int SpeakerType, const FString Message, const bool bRunKana, const bool bEnableInterrogativeUpspeak)
{
	if (const TArray<uint8> OutputWAV = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->RunTextToSpeech(SpeakerType, Message, bRunKana, bEnableInterrogativeUpspeak);
		!OutputWAV.IsEmpty())
	{
		return CreateSoundWave(OutputWAV);
	}
	
	return nullptr;
}

/**
 * @brief 入力したテキストをVOICEVOX COREでAudioQueryに変換後、SoundWaveを生成(Blueprint公開ノード)
 */
USoundWave* UVoicevoxBlueprintLibrary::TextToAudioQueryOutput(int SpeakerType, const FString Message, const bool bRunKana, const bool bEnableInterrogativeUpspeak)
{
	const FVoicevoxAudioQuery AudioQuery = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAudioQuery(SpeakerType, Message, bRunKana);

	if (const TArray<uint8> OutputWAV = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->RunSynthesis(AudioQuery, SpeakerType, bEnableInterrogativeUpspeak); !OutputWAV.IsEmpty())
	{
		return CreateSoundWave(OutputWAV);
	}
	return nullptr;
}

/**
 * @brief  VOICEVOX COREで変換したAudioQueryを取得する(Blueprint公開ノード)
 */
FVoicevoxAudioQuery UVoicevoxBlueprintLibrary::GetAudioQuery(int SpeakerType, const FString Message, const bool bRunKana)
{
	return GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAudioQuery(SpeakerType, Message, bRunKana);
}

/**
 * @brief VOICEVOX COREで取得したAudioQuery元にSoundWaveを作成(Blueprint公開ノード)
 */
USoundWave* UVoicevoxBlueprintLibrary::AudioQueryOutput(const FVoicevoxAudioQuery AudioQuery, int SpeakerType, bool bEnableInterrogativeUpspeak)
{
	if (const TArray<uint8> OutputWAV = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->RunSynthesis(AudioQuery, SpeakerType, bEnableInterrogativeUpspeak); !OutputWAV.IsEmpty())
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
		//Sound->RawPCMData = static_cast<uint8*>(FMemory::Malloc(WaveInfo.SampleDataSize));
		//FMemory::Memmove(Sound->RawPCMData, WaveInfo.SampleDataStart, WaveInfo.SampleDataSize);
		
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
