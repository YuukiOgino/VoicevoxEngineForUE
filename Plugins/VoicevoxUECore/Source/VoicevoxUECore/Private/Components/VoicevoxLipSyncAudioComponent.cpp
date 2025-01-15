// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxLipSyncAudioComponent.cpp
 * @brief  VOICEVOXのAudioQueryを解析して音再生とリップシンクを行うコンポーネントのCPPファイル
 * @author Yuuki Ogino
 */


#include "Components/VoicevoxLipSyncAudioComponent.h"

#include "Sound/SoundWaveProcedural.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"

DEFINE_LOG_CATEGORY(LogVoicevoxLipSync);

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
	// ループ無しかつ最後まで再生しても止まらない場合があるので、明確にストップする
	if (Sound != nullptr && !Sound->IsLooping() && InPlaybackPercentage >= 1.0f)
	{
		Stop();
		return;
	}
	if (LipSyncList.IsEmpty()) return;
	
	if (LipSyncTime < Sound->Duration * InPlaybackPercentage)
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
	if (bIsExecTts)
	{
		UE_LOG(LogVoicevoxLipSync, Warning, TEXT("合成音声生成中のため、音声再生をキャンセルしました。Delay等で少し時間を置いてから再度実行してください"));
		const FColor Col = FColor::Yellow;
		const FVector2D Scl = FVector2D(1.0f, 1.0f);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, Col, TEXT("合成音声生成中のため、音声再生をキャンセルしました。Delay等で少し時間を置いてから再度実行してください"), true, Scl);
		return;
	}
	
	if (Sound != nullptr)
	{
		Stop();
		SetSound(nullptr);
	}
	
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

void UVoicevoxLipSyncAudioComponent::PlayToAudioQuery(const FVoicevoxAudioQuery& Query, int64 SpeakerType, bool bEnableInterrogativeUpspeak, float StartTime)
{
	if (bIsExecTts)
	{
		UE_LOG(LogVoicevoxLipSync, Warning, TEXT("合成音声生成中のため、音声再生をキャンセルしました。Delay等で少し時間を置いてから再度実行してください"));
		const FColor Col = FColor::Yellow;
		const FVector2D Scl = FVector2D(1.0f, 1.0f);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, Col, TEXT("合成音声生成中のため、音声再生をキャンセルしました。Delay等で少し時間を置いてから再度実行してください"), true, Scl);
		return;
	}
	
	if (Sound != nullptr)
	{
		Stop();
		SetSound(nullptr);
	}
	
	AudioQuery = Query;
	PlayStartTime = StartTime;
	TtsTask = UE::Tasks::Launch<>(TEXT("LipSyncComponentTextToSpeechTask"), [=]
	{
		bIsExecTts = true;

		// LipSyncに必要なデータを生成する
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

void UVoicevoxLipSyncAudioComponent::ToSoundWave(int64 SpeakerType, bool bEnableInterrogativeUpspeak)
{
	TtsTask = UE::Tasks::Launch<>(TEXT("LipSyncComponentTextToSpeechTask"), [=]
	{
		bIsExecTts = true;

		// LipSyncに必要なデータを生成する
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