// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOX COREのAPIへ接続するBlueprint公開ノードをまとめたCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxAsyncTask.h"
#include "VoicevoxCoreUtil.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/Task.h"

/**
 * @brief VOICEVOX CORE 初期化(Blueprint公開ノード)
 */
UVoicevoxInitializeAsyncTask* UVoicevoxInitializeAsyncTask::Initialize(UObject* WorldContextObject,const bool bUseGPU, const int CPUNumThreads)
{
	UVoicevoxInitializeAsyncTask* Task = NewObject<UVoicevoxInitializeAsyncTask>();
	Task->bUseGPU = bUseGPU;
	Task->CPUNumThreads = CPUNumThreads;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

/**
 * @brief VOICEVOX CORE 終了処理(Blueprint公開ノード)
 */
void UVoicevoxInitializeAsyncTask::Finalize()
{
	FVoicevoxCoreUtil::Finalize();
}

/**
 * @brief VOICEVOX CORE メタ情報を取得する(Blueprint公開ノード)
 */
void UVoicevoxInitializeAsyncTask::GetMetasToList(TArray<FVoicevoxMeta>& Metas)
{
	Metas = FVoicevoxCoreUtil::GetMetaList();
}

/**
 * @brief サポートデバイス情報を取得する(Blueprint公開ノード)
 */
void UVoicevoxInitializeAsyncTask::GetSupportedDevices(FVoicevoxSupportedDevices& SupportedDevices)
{
	SupportedDevices = FVoicevoxCoreUtil::GetSupportedDevices();
}

/**
 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
 */
void UVoicevoxInitializeAsyncTask::Activate()
{
	UE::Tasks::Launch(TEXT("VoicevoxCoreTask"), [&]
	{
		if (FVoicevoxCoreUtil::Initialize(bUseGPU, CPUNumThreads, false))
		{
			OnSuccess.Broadcast();
		}
		else
		{
			OnFail.Broadcast();
		}
		
		SetReadyToDestroy();
	});
}

/**
 * @brief VOICEVOX COREのモデルをロード実行
 */	
UVoicevoxLoadModelAsyncTask* UVoicevoxLoadModelAsyncTask::LoadModel(UObject* WorldContextObject, ESpeakerType SpeakerType)
{
	UVoicevoxLoadModelAsyncTask* Task = NewObject<UVoicevoxLoadModelAsyncTask>();
	Task->SpeakerId = static_cast<int64>(SpeakerType);
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

/**
 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
 */
void UVoicevoxLoadModelAsyncTask::Activate()
{
	if (FVoicevoxCoreUtil::LoadModel(SpeakerId))
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFail.Broadcast();
	}
		
	SetReadyToDestroy();
}

/**
 * @brief VOICEVOX COREで変換した音声データを簡易的に再生させる(Blueprint公開ノード)
 */
UVoicevoxSimplePlayTextToSpeechAsyncTask* UVoicevoxSimplePlayTextToSpeechAsyncTask::SimplePlayTextToSpeech(UObject* WorldContextObject, ESpeakerType SpeakerType, FString Message, const bool bRunKana)
{
	UVoicevoxSimplePlayTextToSpeechAsyncTask* Task = NewObject<UVoicevoxSimplePlayTextToSpeechAsyncTask>();
	Task->SpeakerId = static_cast<int64>(SpeakerType);
	Task->Message = Message;
	Task->bRunKana = bRunKana;
	Task->WorldContextObject = WorldContextObject;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

/**
 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
 */	
void UVoicevoxSimplePlayTextToSpeechAsyncTask::Activate()
{
	if (TArray<uint8> OutputWAV = FVoicevoxCoreUtil::RunTextToSpeech(SpeakerId, *Message, bRunKana, true);
		!OutputWAV.IsEmpty())
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

			if (IsValid(WorldContextObject.Get()))
			{
				UGameplayStatics::PlaySound2D(WorldContextObject->GetWorld(), Sound, 2.0f);
			}
		}
		OnSuccess.Broadcast();
	}
	else
	{
		OnFail.Broadcast();
	}
		
	SetReadyToDestroy();
}

/**
 * @brief VOICEVOX COREで変換したAudioQuery簡易的に再生させる(Blueprint公開ノード)
 */
UVoicevoxSimplePlayTextToAudioQueryAsyncTask* UVoicevoxSimplePlayTextToAudioQueryAsyncTask::SimplePlayTextToAudioQuery(UObject* WorldContextObject, ESpeakerType SpeakerType, FString Message, const bool bRunKana)
{
	UVoicevoxSimplePlayTextToAudioQueryAsyncTask* Task = NewObject<UVoicevoxSimplePlayTextToAudioQueryAsyncTask>();
	Task->SpeakerId = static_cast<int64>(SpeakerType);
	Task->Message = Message;
	Task->bRunKana = bRunKana;
	Task->WorldContextObject = WorldContextObject;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

/**
 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
 */	
void UVoicevoxSimplePlayTextToAudioQueryAsyncTask::Activate()
{
	char* AudioQuery = FVoicevoxCoreUtil::GetAudioQuery(SpeakerId, Message, bRunKana);

	if (TArray<uint8> OutputWAV = FVoicevoxCoreUtil::RunSynthesis(AudioQuery, SpeakerId, bRunKana); !OutputWAV.IsEmpty())
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

			if (IsValid(WorldContextObject.Get()))
			{
				UGameplayStatics::PlaySound2D(WorldContextObject->GetWorld(), Sound);
			}
		}
		OnSuccess.Broadcast();
	}
	else
	{
		OnFail.Broadcast();
	}

	FVoicevoxCoreUtil::AudioQueryFree(AudioQuery);
	SetReadyToDestroy();
}

/**
 * @brief  VOICEVOX COREで変換したAudioQueryを取得する(Blueprint公開ノード)
 */
void UVoicevoxSimplePlayTextToAudioQueryAsyncTask::GetAudioQuery(FVoicevoxAudioQuery& AudioQuery, ESpeakerType SpeakerType, const FString Message, const bool bRunKana)
{
	AudioQuery = FVoicevoxCoreUtil::GetAudioQueryList(static_cast<int64>(SpeakerType), Message, bRunKana);
}

void UVoicevoxSimplePlayTextToAudioQueryAsyncTask::SimplePlayTextToAudioQueryStruct(UObject* WorldContextObject, FVoicevoxAudioQuery AudioQuery, ESpeakerType SpeakerType, bool bRunKana)
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

