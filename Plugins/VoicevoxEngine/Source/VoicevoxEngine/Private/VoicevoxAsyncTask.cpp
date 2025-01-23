// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOX COREのAPIへ接続するBlueprint公開LatentノードをまとめたCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxAsyncTask.h"

#include "VoicevoxBlueprintLibrary.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"

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
	Task = UE::Tasks::Launch<>(TEXT("VoicevoxCoreTask"), [&]
	{
		if (GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->Initialize(bUseGPU, CPUNumThreads, false))
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
 * @brief BeginDestroy
 */
void UVoicevoxInitializeAsyncTask::BeginDestroy()
{
	Task.Wait();
	Super::BeginDestroy();
}

//------------------------------------------------------------------------
// UVoicevoxLoadModelAsyncTask
//------------------------------------------------------------------------

/**
 * @brief VOICEVOX COREのモデルをロード実行
 */	
UVoicevoxLoadModelAsyncTask* UVoicevoxLoadModelAsyncTask::LoadModel(UObject* WorldContextObject, int SpeakerType)
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
	if (GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->LoadModel(SpeakerId))
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
// UVoicevoxTextToSpeechAsyncTask
//------------------------------------------------------------------------

/**
 * @brief 非同期でVOICEVOX COERで変換した音声データを取得(Blueprint公開ノード)
 */
UVoicevoxTextToSpeechAsyncTask* UVoicevoxTextToSpeechAsyncTask::TextToSpeech(UObject* WorldContextObject, const int SpeakerType, FString Message, const bool bRunKana, bool bEnableInterrogativeUpspeak)
{
	UVoicevoxTextToSpeechAsyncTask* Task = NewObject<UVoicevoxTextToSpeechAsyncTask>();
	Task->SpeakerId = SpeakerType;
	Task->Message = Message;
	Task->bRunKana = bRunKana;
	Task->bEnableInterrogativeUpspeak = bEnableInterrogativeUpspeak;
	Task->bIsUseAudioQuery = false;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

/**
 * @brief 非同期で入力したテキストをVOICEVOX COREでAudioQueryに変換後、SoundWaveを生成(Blueprint公開ノード)
 */
UVoicevoxTextToSpeechAsyncTask* UVoicevoxTextToSpeechAsyncTask::TextToAudioQuery(UObject* WorldContextObject, int SpeakerType, FString Message, bool bRunKana, bool bEnableInterrogativeUpspeak)
{
	UVoicevoxTextToSpeechAsyncTask* Task = NewObject<UVoicevoxTextToSpeechAsyncTask>();
	Task->SpeakerId = SpeakerType;
	Task->Message = Message;
	Task->bRunKana = bRunKana;
	Task->bEnableInterrogativeUpspeak = bEnableInterrogativeUpspeak;
	Task->bIsUseAudioQuery = true;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

/**
 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
 */	
void UVoicevoxTextToSpeechAsyncTask::Activate()
{
	Task = UE::Tasks::Launch<>(TEXT("VoicevoxCoreTextToSpeechTask"), [&]
	{
		if (USoundWave* Sound = bIsUseAudioQuery ?
			UVoicevoxBlueprintLibrary::TextToAudioQueryOutput(SpeakerId, Message, bRunKana, bEnableInterrogativeUpspeak) :
			UVoicevoxBlueprintLibrary::TextToSpeechOutput(SpeakerId, Message, bRunKana, bEnableInterrogativeUpspeak);
			Sound != nullptr)
		{
			OnSuccess.Broadcast(Sound);
		}
		else
		{
			OnFail.Broadcast();
		}
		
		SetReadyToDestroy();
	});

}

/**
 * @brief BeginDestroy
 */
void UVoicevoxTextToSpeechAsyncTask::BeginDestroy()
{
	Task.Wait();
	Super::BeginDestroy();
}

//------------------------------------------------------------------------
// UVoicevoxAudioQueryToSpeechAsyncTask
//------------------------------------------------------------------------

/**
 * @brief 非同期でVOICEVOX COREで取得したAudioQueryを元に音声データを取得(Blueprint公開ノード)
 */
UVoicevoxAudioQueryToSpeechAsyncTask* UVoicevoxAudioQueryToSpeechAsyncTask::AudioQueryOutput(UObject* WorldContextObject, int SpeakerType, FVoicevoxAudioQuery AudioQuery, const bool bEnableInterrogativeUpspeak)
{
	UVoicevoxAudioQueryToSpeechAsyncTask* Task = NewObject<UVoicevoxAudioQueryToSpeechAsyncTask>();
	Task->SpeakerId = SpeakerType;
	Task->AudioQuery = AudioQuery;
	Task->bEnableInterrogativeUpspeak = bEnableInterrogativeUpspeak;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

UVoicevoxAudioQueryToSpeechAsyncTask* UVoicevoxAudioQueryToSpeechAsyncTask::VoicevoxQueryOutput(UObject* WorldContextObject, UVoicevoxQuery* VoicevoxQuery, bool bEnableInterrogativeUpspeak)
{
	UVoicevoxAudioQueryToSpeechAsyncTask* Task = NewObject<UVoicevoxAudioQueryToSpeechAsyncTask>();
	if (VoicevoxQuery == nullptr)
	{
		Task->SpeakerId = 0;
		return nullptr;
	}
	Task->SpeakerId = VoicevoxQuery->SpeakerType;
	Task->AudioQuery = VoicevoxQuery->VoicevoxAudioQuery;
	Task->bEnableInterrogativeUpspeak = bEnableInterrogativeUpspeak;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

/**
 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
 */	
void UVoicevoxAudioQueryToSpeechAsyncTask::Activate()
{
	if (SpeakerId == 0)
	{
		OnFail.Broadcast();
		SetReadyToDestroy();
		return;
	}
	
	Task = UE::Tasks::Launch<>(TEXT("VoicevoxCoreTextToSpeechTask"), [&]
	{
		if (USoundWave* Sound = UVoicevoxBlueprintLibrary::AudioQueryOutput(AudioQuery, SpeakerId, bEnableInterrogativeUpspeak);
			Sound != nullptr)
		{
			OnSuccess.Broadcast(Sound);
		}
		else
		{
			OnFail.Broadcast();
		}
		
		SetReadyToDestroy();
	});

}

/**
 * @brief BeginDestroy
 */
void UVoicevoxAudioQueryToSpeechAsyncTask::BeginDestroy()
{
	Task.Wait();
	Super::BeginDestroy();
}