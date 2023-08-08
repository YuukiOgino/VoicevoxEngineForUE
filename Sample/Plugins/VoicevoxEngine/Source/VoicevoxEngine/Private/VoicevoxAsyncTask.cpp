// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOX COREのAPIへ接続するBlueprint公開LatentノードをまとめたCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxAsyncTask.h"
#include "VoicevoxBlueprintLibrary.h"
#include "VoicevoxCoreUtil.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/Task.h"

//------------------------------------------------------------------------
// UVoicevoxInitializeAsyncTask
//------------------------------------------------------------------------

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

//------------------------------------------------------------------------
// UVoicevoxLoadModelAsyncTask
//------------------------------------------------------------------------

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

//------------------------------------------------------------------------
// UVoicevoxSimplePlayTextToSpeechAsyncTask
//------------------------------------------------------------------------

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
	if (const TArray<uint8> OutputWAV = FVoicevoxCoreUtil::RunTextToSpeech(SpeakerId, *Message, bRunKana, true);
		!OutputWAV.IsEmpty())
	{
		USoundWave* Sound = UVoicevoxBlueprintLibrary::CreateSoundWave(OutputWAV);
		if (IsValid(WorldContextObject.Get()))
		{
			UGameplayStatics::PlaySound2D(WorldContextObject->GetWorld(), Sound, 2.0f);
		}
		OnSuccess.Broadcast();
	}
	else
	{
		OnFail.Broadcast();
	}
		
	SetReadyToDestroy();
}

//------------------------------------------------------------------------
// UVoicevoxSimplePlayTextToSpeechAsyncTask
//------------------------------------------------------------------------

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
	const FVoicevoxAudioQuery AudioQuery = FVoicevoxCoreUtil::GetAudioQuery(SpeakerId, Message, bRunKana);

	if (const TArray<uint8> OutputWAV = FVoicevoxCoreUtil::RunSynthesis(AudioQuery, SpeakerId, bRunKana); !OutputWAV.IsEmpty())
	{
		USoundWave* Sound = UVoicevoxBlueprintLibrary::CreateSoundWave(OutputWAV);
		if (IsValid(WorldContextObject.Get()))
		{
			UGameplayStatics::PlaySound2D(WorldContextObject->GetWorld(), Sound);
		}
		OnSuccess.Broadcast();
	}
	else
	{
		OnFail.Broadcast();
	}
	
	SetReadyToDestroy();
}
