// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxLipSyncAudioComponent.cpp
 * @brief  VOICEVOXのAudioQueryを解析して音再生とリップシンクを行うコンポーネントのCPPファイル
 * @author Yuuki Ogino
 */


#include "Components/VoicevoxLipSyncAudioComponent.h"

#include "Sound/SoundWaveProcedural.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"


// Sets default values for this component's properties
UVoicevoxLipSyncAudioComponent::UVoicevoxLipSyncAudioComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	OnAudioPlaybackPercentNative.AddUObject(this, &UVoicevoxLipSyncAudioComponent::HandlePlaybackPercent);
	// ...
}


// Called when the game starts
void UVoicevoxLipSyncAudioComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UVoicevoxLipSyncAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UVoicevoxLipSyncAudioComponent::HandlePlaybackPercent(const UAudioComponent* InComponent, const USoundWave* InSoundWave, const float InPlaybackPercentage)
{

}

void UVoicevoxLipSyncAudioComponent::PlayToText(int SpeakerType, FString Message, bool bRunKana, bool bEnableInterrogativeUpspeak, float StartTime)
{
	UE::Tasks::Launch<>(TEXT("VoicevoxCoreTextToSpeechTask"), [&]
	{
		if (const TArray<uint8> OutputWAV = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->RunTextToSpeech(SpeakerType, Message, bRunKana, bEnableInterrogativeUpspeak);
		!OutputWAV.IsEmpty())
		{
			FString ErrorMessage = "";
	
			if (FWaveModInfo WaveInfo; WaveInfo.ReadWaveInfo(OutputWAV.GetData(), OutputWAV.Num(), &ErrorMessage))
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

				SetSound(Sound);
				
				PlayInternalRequestData InternalRequestData;
				InternalRequestData.StartTime = StartTime;
				PlayInternal(InternalRequestData);
				
			}
		}
	});
}