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

void UVoicevoxLipSyncAudioComponent::InitMorphNumMap() 
{
	LipSyncMorphNumMap[ELipSyncVowelType::A] = 0.0f;
	LipSyncMorphNumMap[ELipSyncVowelType::I] = 0.0f;
	LipSyncMorphNumMap[ELipSyncVowelType::U] = 0.0f;
	LipSyncMorphNumMap[ELipSyncVowelType::E] = 0.0f;
	LipSyncMorphNumMap[ELipSyncVowelType::O] = 0.0f;
}

void UVoicevoxLipSyncAudioComponent::UpdateVowelMorphNum(ELipSyncVowelType VowelType)
{
	float CloseA = LipSyncMorphNumMap[ELipSyncVowelType::A] * 0.8f;
	float CloseI = LipSyncMorphNumMap[ELipSyncVowelType::I] * 0.8f;
	float CloseU = LipSyncMorphNumMap[ELipSyncVowelType::U] * 0.8f;
	float CloseE = LipSyncMorphNumMap[ELipSyncVowelType::E] * 0.8f;
	float CloseO = LipSyncMorphNumMap[ELipSyncVowelType::O] * 0.8f;

	float UpdateA = 0.0f;
	float UpdateI = 0.0f;
	float UpdateU = 0.0f;
	float UpdateE = 0.0f;
	float UpdateO = 0.0f;

	switch (VowelType)
	{
		case ELipSyncVowelType::A:
			UpdateA = 1.0f;
			break;
		case ELipSyncVowelType::I:
			UpdateI = 1.0f;
			break;
		case ELipSyncVowelType::U:
			UpdateU = 1.0f;
			break;
		case ELipSyncVowelType::E:
			UpdateE = 1.0f;
			break;
		case ELipSyncVowelType::O:
			UpdateO = 1.0f;
			break;
		case ELipSyncVowelType::CL:
			UpdateA = CloseA * 0.8f;
			UpdateI = CloseI * 0.8f;
			UpdateU = CloseU * 0.8f;
			UpdateE = CloseE * 0.8f;
			UpdateO = CloseO * 0.8f;
			break;
		default:
			break;
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
		FName MorphTargetName = NAME_None;
		if (LipSync.VowelType != ELipSyncVowelType::Non && LipSync.VowelType != ELipSyncVowelType::CL)
		{
			MorphTargetName = LipSyncMorphNameMap[LipSync.VowelType];
		}
		if (OnLipSyncUpdate.IsBound())
		{
			OnLipSyncUpdate.Broadcast(LipSync, MorphTargetName);
		}

		if (OnLipSyncUpdateNative.IsBound())
		{
			OnLipSyncUpdateNative.Broadcast(LipSync, MorphTargetName);
		}
	}
}

void UVoicevoxLipSyncAudioComponent::PlayToText(const int SpeakerType, const FString Message, const bool bRunKana, const bool bEnableInterrogativeUpspeak, const float StartTime)
{
	if (CheckExecTts()) return;
	
	if (Sound != nullptr)
	{
		Stop();
		SetSound(nullptr);
	}

	InitMorphNumMap();
	PlayStartTime = StartTime;
	AudioQuery = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAudioQuery(SpeakerType, Message, bRunKana);

	ToSoundWave(SpeakerType, bEnableInterrogativeUpspeak);
}

void UVoicevoxLipSyncAudioComponent::PlayToAudioQuery(const FVoicevoxAudioQuery& Query, const int64 SpeakerType, const bool bEnableInterrogativeUpspeak, const float StartTime)
{
	if (CheckExecTts()) return;
	
	if (Sound != nullptr)
	{
		Stop();
		SetSound(nullptr);
	}

	InitMorphNumMap();
	PlayStartTime = StartTime;
	AudioQuery = Query;
	ToSoundWave(SpeakerType, bEnableInterrogativeUpspeak);
}

void UVoicevoxLipSyncAudioComponent::PlayToAudioQueryAsset(UVoicevoxQuery* VoicevoxQuery, const bool bEnableInterrogativeUpspeak, const float StartTime)
{
	if (CheckExecTts()) return;
	
	if (Sound != nullptr)
	{
		Stop();
		SetSound(nullptr);
	}

	InitMorphNumMap();
	PlayStartTime = StartTime;
	AudioQuery = VoicevoxQuery->VoicevoxAudioQuery;
	ToSoundWave(VoicevoxQuery->SpeakerType, bEnableInterrogativeUpspeak);
}

/**
 * テキストから音声変換を実行中かチェック
 */
bool UVoicevoxLipSyncAudioComponent::CheckExecTts() const
{
	if (bIsExecTts)
	{
		UE_LOG(LogVoicevoxLipSync, Warning, TEXT("合成音声生成中のため、音声再生をキャンセルしました。Delay等で少し時間を置いてから再度実行してください"));
		const FColor Col = FColor::Yellow;
		const FVector2D Scl = FVector2D(1.0f, 1.0f);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, Col, TEXT("合成音声生成中のため、音声再生をキャンセルしました。Delay等で少し時間を置いてから再度実行してください"), true, Scl);
		return true;
	}

	return false;
}

/**
 * @brief AudioQueryからSoundWaveへ変換
 */
void UVoicevoxLipSyncAudioComponent::ToSoundWave(const int64 SpeakerType, const bool bEnableInterrogativeUpspeak)
{
	TtsTask = UE::Tasks::Launch<>(TEXT("LipSyncComponentTextToSpeechTask"), [=]
	{
		bIsExecTts = true;

		// LipSyncに必要なデータを生成する
		LipSyncList = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetLipSyncList(AudioQuery);
		Algo::Reverse(LipSyncList);
		LipSyncTime = 0.0f;
		// USoundWaveを生成する。Launch内でPlayを実行するとクラッシュするため、Play処理はTickComponentで行う
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