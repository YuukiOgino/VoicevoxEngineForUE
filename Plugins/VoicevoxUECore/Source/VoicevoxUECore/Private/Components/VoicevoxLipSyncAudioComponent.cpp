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

	if (bIsExecTts)
	{
		if (TtsTask.IsValid() && TtsTask.IsCompleted())
		{
			if (Sound != nullptr)
			{
				Play(PlayStartTime);
			}
			
			bIsExecTts = false;
			PlayStartTime = 0.0f;
		}
	}

}

void UVoicevoxLipSyncAudioComponent::HandlePlaybackPercent(const UAudioComponent* InComponent, const USoundWave* InSoundWave, const float InPlaybackPercentage)
{
	if (LipSyncList.IsEmpty()) return;

	if (LipSyncTime < InPlaybackPercentage)
	{
		const FVoicevoxLipSync LipSync = LipSyncList.Pop();
		LipSyncTime += LipSync.Length;
		if (OnLipSyncUpdate.IsBound())
		{
			OnLipSyncUpdate.Broadcast(LipSync);
		}

		if (OnLipSyncUpdateNative.IsBound())
		{
			OnLipSyncUpdateNative.Broadcast(LipSync);
		}
	}
}

void UVoicevoxLipSyncAudioComponent::PlayToText(const int SpeakerType, const FString Message, const bool bRunKana, const bool bEnableInterrogativeUpspeak, const float StartTime)
{
	SetSound(nullptr);
	PlayStartTime = StartTime;
	TtsTask = UE::Tasks::Launch<>(TEXT("LipSyncComponentTextToSpeechTask"), [=]
	{
		bIsExecTts = true;

		// LipSyncに必要なデータを生成する
		AudioQuery = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAudioQuery(SpeakerType, Message, bRunKana);
		LipSyncList = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetLipSyncList(AudioQuery);
		Algo::Reverse(LipSyncList);
		LipSyncTime = 0.0f;
		// USoundWaveを生成する。Launch内でPlayを実行するとクラッシュするため、Play処理はTickTickComponentで行う
		if (const TArray<uint8> OutputWAV = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->RunSynthesis(AudioQuery, SpeakerType, bEnableInterrogativeUpspeak);
		!OutputWAV.IsEmpty())
		{
			FString ErrorMessage = "";
			if (FWaveModInfo WaveInfo; WaveInfo.ReadWaveInfo(OutputWAV.GetData(), OutputWAV.Num(), &ErrorMessage))
			{
				USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>(USoundWaveProcedural::StaticClass());
				const int32 ChannelCount = *WaveInfo.pChannels;
				const int32 SizeOfSample = *WaveInfo.pBitsPerSample / 8;
				const int32 NumSamples = WaveInfo.SampleDataSize / SizeOfSample;
				const int32 NumFrames = NumSamples / ChannelCount;
				
				SoundWave->RawPCMDataSize = WaveInfo.SampleDataSize;
				SoundWave->QueueAudio(WaveInfo.SampleDataStart, WaveInfo.SampleDataSize);
				
				SoundWave->Duration = static_cast<float>(NumFrames) / *WaveInfo.pSamplesPerSec;
				SoundWave->SetSampleRate(*WaveInfo.pSamplesPerSec);
				SoundWave->NumChannels = ChannelCount;
				SoundWave->TotalSamples = *WaveInfo.pSamplesPerSec * SoundWave->Duration;
				SoundWave->SoundGroup = SOUNDGROUP_Default;

				SetSound(SoundWave);
			}
		}
	});
}