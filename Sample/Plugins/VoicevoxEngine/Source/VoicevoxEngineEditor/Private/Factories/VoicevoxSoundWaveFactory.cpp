// Copyright Yuuki Ogino. All Rights Reserved.

#include "Factories/VoicevoxSoundWaveFactory.h"
#include "Sound/SoundWave.h"

UVoicevoxSoundWaveFactory::UVoicevoxSoundWaveFactory(): Super()
{
	bCreateNew = true;
	SupportedClass = USoundWave::StaticClass();
}

UObject* UVoicevoxSoundWaveFactory::FactoryCreateNew(UClass* InClass, UObject* InParent,
                                                     FName InName, EObjectFlags Flags,
                                                     UObject* Context, FFeedbackContext* Warn)
{
	FString ErrorMessage = "";
	if (FWaveModInfo WaveInfo; WaveInfo.ReadWaveInfo(OutputWAV.GetData(), OutputWAV.Num(), &ErrorMessage))
	{
		USoundWave* Sound = NewObject<USoundWave>(InParent, InClass, InName, Flags, Context);
	
		const int32 ChannelCount = *WaveInfo.pChannels;
		const int32 SizeOfSample = *WaveInfo.pBitsPerSample / 8;
		const int32 NumSamples = WaveInfo.SampleDataSize / SizeOfSample;
		const int32 NumFrames = NumSamples / ChannelCount;
	
		Sound->RawPCMDataSize = WaveInfo.SampleDataSize;
		Sound->RawPCMData = static_cast<uint8*>(FMemory::Malloc(WaveInfo.SampleDataSize));
		FMemory::Memmove(Sound->RawPCMData, WaveInfo.SampleDataStart, WaveInfo.SampleDataSize);

		Sound->RawData.Lock(LOCK_READ_WRITE);
		void* LockedData = Sound->RawData.Realloc(OutputWAV.Num());
		FMemory::Memcpy(LockedData, OutputWAV.GetData(), OutputWAV.Num());
		Sound->RawData.Unlock();
	
		Sound->Duration = static_cast<float>(NumFrames) / *WaveInfo.pSamplesPerSec;
		Sound->SetSampleRate(*WaveInfo.pSamplesPerSec);
		Sound->NumChannels = ChannelCount;
		Sound->TotalSamples = *WaveInfo.pSamplesPerSec * Sound->Duration;
		Sound->SoundGroup = SOUNDGROUP_Default;

		return Sound;
	}

	return nullptr;
}
