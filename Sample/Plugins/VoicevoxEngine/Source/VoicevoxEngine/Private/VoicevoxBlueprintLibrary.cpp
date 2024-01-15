// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOX COREのAPIへ接続するBlueprint公開ノードをまとめたCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxBlueprintLibrary.h"

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
TArray<FVoicevoxMeta> UVoicevoxBlueprintLibrary::GetMetasToList()
{
	return FVoicevoxCoreUtil::GetMetaList();
}

/**
 * @brief サポートデバイス情報を取得する(Blueprint公開ノード)
 */
FVoicevoxSupportedDevices UVoicevoxBlueprintLibrary::GetSupportedDevices()
{
	return FVoicevoxCoreUtil::GetSupportedDevices();
}

/**
 * @brief VOICEVOX COREのバージョン情報を取得する
 */
FString UVoicevoxBlueprintLibrary::GetVoicevoxCoreVersion()
{
	return FVoicevoxCoreUtil::GetVoicevoxVersion();
}

/**
 * @brief ハードウェアアクセラレーションがGPUモードか判定する
 */
bool UVoicevoxBlueprintLibrary::IsVoicevoxGpuMode()
{
	return FVoicevoxCoreUtil::IsGpuMode();
}

/**
 * @brief VOICEVOX COREで変換した音声データを元にSoundWaveを生成(Blueprint公開ノード)
 */
USoundWave* UVoicevoxBlueprintLibrary::TextToSpeechOutput(EVoicevoxSpeakerType SpeakerType, const FString Message, const bool bRunKana, const bool bEnableInterrogativeUpspeak)
{
	if (const TArray<uint8> OutputWAV = FVoicevoxCoreUtil::RunTextToSpeech(static_cast<int64>(SpeakerType), Message, bRunKana, bEnableInterrogativeUpspeak);
		!OutputWAV.IsEmpty())
	{
		return CreateSoundWave(OutputWAV);
	}
	
	return nullptr;
}

/**
 * @brief 入力したテキストをVOICEVOX COREでAudioQueryに変換後、SoundWaveを生成(Blueprint公開ノード)
 */
USoundWave* UVoicevoxBlueprintLibrary::TextToAudioQueryOutput(EVoicevoxSpeakerType SpeakerType, const FString Message, const bool bRunKana, const bool bEnableInterrogativeUpspeak)
{
	const FVoicevoxAudioQuery AudioQuery = FVoicevoxCoreUtil::GetAudioQuery(static_cast<int64>(SpeakerType), Message, bRunKana);

	if (const TArray<uint8> OutputWAV = FVoicevoxCoreUtil::RunSynthesis(AudioQuery, static_cast<int64>(SpeakerType), bEnableInterrogativeUpspeak); !OutputWAV.IsEmpty())
	{
		return CreateSoundWave(OutputWAV);
	}
	return nullptr;
}

/**
 * @brief  VOICEVOX COREで変換したAudioQueryを取得する(Blueprint公開ノード)
 */
FVoicevoxAudioQuery UVoicevoxBlueprintLibrary::GetAudioQuery(EVoicevoxSpeakerType SpeakerType, const FString Message, const bool bRunKana)
{
	return FVoicevoxCoreUtil::GetAudioQuery(static_cast<int64>(SpeakerType), Message, bRunKana);
}

/**
 * @brief VOICEVOX COREで取得したAudioQuery元にSoundWaveを作成(Blueprint公開ノード)
 */
USoundWave* UVoicevoxBlueprintLibrary::AudioQueryOutput(const FVoicevoxAudioQuery AudioQuery, EVoicevoxSpeakerType SpeakerType, bool bEnableInterrogativeUpspeak)
{
	if (const TArray<uint8> OutputWAV = FVoicevoxCoreUtil::RunSynthesis(AudioQuery, static_cast<int64>(SpeakerType), bEnableInterrogativeUpspeak); !OutputWAV.IsEmpty())
	{
		return CreateSoundWave(OutputWAV);
	}

	return nullptr;
}

/**
 * @brief 生成した音声データからUSoundWaveを作成
 */
USoundWave* UVoicevoxBlueprintLibrary::CreateSoundWave(TArray<uint8> PCMData, UObject* InParent, const FName Name)
{
	FString ErrorMessage = "";
	
	if (FWaveModInfo WaveInfo; WaveInfo.ReadWaveInfo(PCMData.GetData(), PCMData.Num(), &ErrorMessage))
	{
		USoundWave* Sound;
		if (InParent != nullptr)
		{
			constexpr EObjectFlags Flags = RF_Public | RF_Standalone;
			Sound = NewObject<USoundWave>(InParent, Name, Flags);
		}
		else
		{
			Sound = NewObject<USoundWave>(USoundWave::StaticClass());
		}
		
		const int32 ChannelCount = *WaveInfo.pChannels;
		const int32 SizeOfSample = *WaveInfo.pBitsPerSample / 8;
		const int32 NumSamples = WaveInfo.SampleDataSize / SizeOfSample;
		const int32 NumFrames = NumSamples / ChannelCount;
		
		Sound->RawPCMDataSize = WaveInfo.SampleDataSize;
		Sound->RawPCMData = static_cast<uint8*>(FMemory::Malloc(WaveInfo.SampleDataSize));
		FMemory::Memmove(Sound->RawPCMData, WaveInfo.SampleDataStart, WaveInfo.SampleDataSize);

#if (ENGINE_MINOR_VERSION == 0)
		Sound->RawData.Lock(LOCK_READ_WRITE);
		void* LockedData = Sound->RawData.Realloc(PCMData.Num());
		FMemory::Memcpy(LockedData, PCMData.GetData(), PCMData.Num());
		Sound->RawData.Unlock();
#else
		const FSharedBuffer UpdatedBuffer = FSharedBuffer::Clone(PCMData.GetData(), PCMData.Num());
		Sound->RawData.UpdatePayload(UpdatedBuffer);
#endif
		
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
TArray<FVoicevoxLipSync> UVoicevoxBlueprintLibrary::GetLipSyncList(FVoicevoxAudioQuery AudioQuery)
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