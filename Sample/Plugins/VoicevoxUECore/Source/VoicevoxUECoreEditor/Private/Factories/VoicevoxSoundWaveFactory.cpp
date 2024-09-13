// Copyright Yuuki Ogino. All Rights Reserved.

#include "Factories/VoicevoxSoundWaveFactory.h"

#include "AudioCompressionSettingsUtils.h"
#include "Sound/SoundWave.h"
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 0)
#include "SoundFileIO/SoundFileIO.h"
#endif

DEFINE_LOG_CATEGORY(LogVoicevoxSoundWaveFactory);

/**
 * @brief コンストラクタ
 */
UVoicevoxSoundWaveFactory::UVoicevoxSoundWaveFactory(): Super()
{
	bCreateNew = true;
	SupportedClass = USoundWave::StaticClass();
}

/**
 * @brief FactoryCreateNew override
 */
UObject* UVoicevoxSoundWaveFactory::FactoryCreateNew(UClass* InClass, UObject* InParent,
                                                     const FName InName, const EObjectFlags Flags,
                                                     UObject* Context, FFeedbackContext* Warn)
{
	FString ErrorMessage = "";
	if (FWaveModInfo WaveInfo; WaveInfo.ReadWaveInfo(OutputWAV.GetData(), OutputWAV.Num(), &ErrorMessage))
	{
		USoundWave* Sound = NewObject<USoundWave>(InParent, InClass, InName, Flags, Context);
		
		const int32 ChannelCount = *WaveInfo.pChannels;
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 0)
		const int32 SizeOfSample = *WaveInfo.pBitsPerSample / 8;
		const int32 NumSamples = WaveInfo.SampleDataSize / SizeOfSample;
#else
		const int32 NumSamples = Audio::SoundFileUtils::GetNumSamples(OutputWAV);
#endif
		const int32 NumFrames = NumSamples / ChannelCount;
		
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 0)
		Sound->RawData.Lock(LOCK_READ_WRITE);
		void* LockedData = Sound->RawData.Realloc(OutputWAV.Num());
		FMemory::Memcpy(LockedData, OutputWAV.GetData(), OutputWAV.Num());
		Sound->RawData.Unlock();
#else
		Sound->InvalidateCompressedData(true, false);
		const FSharedBuffer UpdatedBuffer = FSharedBuffer::Clone(OutputWAV.GetData(), OutputWAV.Num());
		Sound->RawData.UpdatePayload(UpdatedBuffer);
#endif
		
		Sound->RawPCMDataSize = WaveInfo.SampleDataSize;
		Sound->RawPCMData = static_cast<uint8*>(FMemory::Malloc(WaveInfo.SampleDataSize));
		FMemory::Memmove(Sound->RawPCMData, WaveInfo.SampleDataStart, WaveInfo.SampleDataSize);
		
		Sound->Duration = static_cast<float>(NumFrames) / *WaveInfo.pSamplesPerSec;
		Sound->SetImportedSampleRate(*WaveInfo.pSamplesPerSec);
		Sound->SetSampleRate(*WaveInfo.pSamplesPerSec);
		Sound->NumChannels = ChannelCount;
		Sound->TotalSamples = *WaveInfo.pSamplesPerSec * Sound->Duration;
		Sound->SoundGroup = SOUNDGROUP_Default;

#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 0)
		Sound->SetTimecodeInfo(FSoundWaveTimecodeInfo{});
#endif

		const bool bRebuildStreamingChunks = FPlatformCompressionUtilities::IsCurrentPlatformUsingStreamCaching();
		Sound->InvalidateCompressedData(true, bRebuildStreamingChunks);
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 0)
		if (bRebuildStreamingChunks && Sound->IsStreaming(nullptr))
		{
			Sound->LoadZerothChunk();
		}
#endif
		
		GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, Sound);

#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 0)
		Sound->PostImport();
#endif
		Sound->SetRedrawThumbnail(true);

		return Sound;
	}
	
	UE_LOG(LogVoicevoxSoundWaveFactory, Error, TEXT("SoundWave Factory Error: %s"), *ErrorMessage);
	return nullptr;
}
