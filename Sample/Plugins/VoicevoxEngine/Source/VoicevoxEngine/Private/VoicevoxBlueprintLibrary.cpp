// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOX COREのAPIへ接続するBlueprint公開ノードをまとめたCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

/**
 * @brief VOICEVOX CORE 終了処理(Blueprint公開ノード)
 */
void UVoicevoxBlueprintLibrary::Finalize()
{
	FVoicevoxCoreUtil::Finalize();
}

/**
 * @brief VOICEVOX CORE メタ情報を取得する(Blueprint公開ノード)
 */
void UVoicevoxBlueprintLibrary::GetMetasToList(TArray<FVoicevoxMeta>& Metas)
{
	Metas = FVoicevoxCoreUtil::GetMetaList();
}

/**
 * @brief サポートデバイス情報を取得する(Blueprint公開ノード)
 */
void UVoicevoxBlueprintLibrary::GetSupportedDevices(FVoicevoxSupportedDevices& SupportedDevices)
{
	SupportedDevices = FVoicevoxCoreUtil::GetSupportedDevices();
}

/**
 * @brief  VOICEVOX COREで変換したAudioQueryを取得する(Blueprint公開ノード)
 */
void UVoicevoxBlueprintLibrary::GetAudioQuery(FVoicevoxAudioQuery& AudioQuery, ESpeakerType SpeakerType, const FString Message, const bool bRunKana)
{
	AudioQuery = FVoicevoxCoreUtil::GetAudioQuery(static_cast<int64>(SpeakerType), Message, bRunKana);
}

void UVoicevoxBlueprintLibrary::SimplePlayTextToAudioQueryStruct(UObject* WorldContextObject, FVoicevoxAudioQuery AudioQuery, ESpeakerType SpeakerType, bool bRunKana)
{
	if (TArray<uint8> OutputWAV = FVoicevoxCoreUtil::RunSynthesis(AudioQuery, static_cast<int64>(SpeakerType), bRunKana); !OutputWAV.IsEmpty())
	{
		FString ErrorMessage = "";
		if (FWaveModInfo WaveInfo; WaveInfo.ReadWaveInfo(OutputWAV.GetData(), OutputWAV.Num(), &ErrorMessage))
		{
			USoundWave* Sound = NewObject<USoundWave>(USoundWave::StaticClass());
			const int32 ChannelCount = *WaveInfo.pChannels;
			const int32 SizeOfSample = *WaveInfo.pBitsPerSample / 8;
			const int32 NumSamples = WaveInfo.SampleDataSize / SizeOfSample;
			const int32 NumFrames = NumSamples / ChannelCount;
			
			Sound->RawPCMDataSize = WaveInfo.SampleDataSize;
			Sound->RawPCMData = static_cast<uint8*>(FMemory::Malloc(WaveInfo.SampleDataSize));
			FMemory::Memmove(Sound->RawPCMData, WaveInfo.SampleDataStart, WaveInfo.SampleDataSize);
			
			Sound->Duration = static_cast<float>(NumFrames) / *WaveInfo.pSamplesPerSec;
			Sound->SetSampleRate(*WaveInfo.pSamplesPerSec);
			Sound->NumChannels = ChannelCount;
			Sound->TotalSamples = *WaveInfo.pSamplesPerSec * Sound->Duration;

			if (IsValid(WorldContextObject))
			{
				UGameplayStatics::PlaySound2D(WorldContextObject->GetWorld(), Sound);
			}
		}
	}
}

USoundWave* UVoicevoxBlueprintLibrary::CreateSoundWave(TArray<uint8> PCMData)
{
	FString ErrorMessage = "";
	if (FWaveModInfo WaveInfo; WaveInfo.ReadWaveInfo(PCMData.GetData(), PCMData.Num(), &ErrorMessage))
	{
		USoundWave* Sound = NewObject<USoundWave>(USoundWave::StaticClass());
		const int32 ChannelCount = *WaveInfo.pChannels;
		const int32 SizeOfSample = *WaveInfo.pBitsPerSample / 8;
		const int32 NumSamples = WaveInfo.SampleDataSize / SizeOfSample;
		const int32 NumFrames = NumSamples / ChannelCount;
			
		Sound->RawPCMDataSize = WaveInfo.SampleDataSize;
		Sound->RawPCMData = static_cast<uint8*>(FMemory::Malloc(WaveInfo.SampleDataSize));
		FMemory::Memmove(Sound->RawPCMData, WaveInfo.SampleDataStart, WaveInfo.SampleDataSize);
			
		Sound->Duration = static_cast<float>(NumFrames) / *WaveInfo.pSamplesPerSec;
		Sound->SetSampleRate(*WaveInfo.pSamplesPerSec);
		Sound->NumChannels = ChannelCount;
		Sound->TotalSamples = *WaveInfo.pSamplesPerSec * Sound->Duration;
		return Sound;
	}

	return nullptr;
}