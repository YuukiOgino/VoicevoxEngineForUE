// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOX COREのAPIへ接続するBlueprint公開LatentノードをまとめたCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxAsyncTask.h"

#include "VoicevoxBlueprintLibrary.h"
#include "Engine/Engine.h"
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
UVoicevoxLoadModelAsyncTask* UVoicevoxLoadModelAsyncTask::LoadModel(UObject* WorldContextObject, const int32 StyleId)
{
	UVoicevoxLoadModelAsyncTask* Task = NewObject<UVoicevoxLoadModelAsyncTask>();
	Task->StyleId = static_cast<int64>(StyleId);
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

/**
 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
 */
void UVoicevoxLoadModelAsyncTask::Activate()
{
	if (GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->LoadModel(StyleId))
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
// UVoicevoxLoadVoiceModelAsyncTask
//------------------------------------------------------------------------

/**
 * @brief VOICEVOX COREのVVMをロード実行
 */	
UVoicevoxLoadVoiceModelAsyncTask* UVoicevoxLoadVoiceModelAsyncTask::LoadVoiceModel(UObject* WorldContextObject, FString VvmFileName)
{
	UVoicevoxLoadVoiceModelAsyncTask* Task = NewObject<UVoicevoxLoadVoiceModelAsyncTask>();
	Task->VvmFileName = VvmFileName;
	Task->bAllLoad = false;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

/**
 * @brief VOICEVOX COREの全てのVVMをロード
 */	
UVoicevoxLoadVoiceModelAsyncTask* UVoicevoxLoadVoiceModelAsyncTask::AllLoadVoiceModel(UObject* WorldContextObject)
{
	UVoicevoxLoadVoiceModelAsyncTask* Task = NewObject<UVoicevoxLoadVoiceModelAsyncTask>();
	Task->bAllLoad = true;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

/**
 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
 */
void UVoicevoxLoadVoiceModelAsyncTask::Activate()
{
	Task = UE::Tasks::Launch<>(TEXT("VoicevoxCoreTask"), [&]
	{
		if (bAllLoad)
		{
			if (GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->AllLoadVoiceModel())
			{
				OnSuccess.Broadcast();
			}
			else
			{
				OnFail.Broadcast();
			}
		}
		else
		{
			if (GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->LoadVoiceModel(VvmFileName))
			{
				OnSuccess.Broadcast();
			}
			else
			{
				OnFail.Broadcast();
			}
		}
		SetReadyToDestroy();
	});
}

/**
 * @brief BeginDestroy
 */
void UVoicevoxLoadVoiceModelAsyncTask::BeginDestroy()
{
	Task.Wait();
	Super::BeginDestroy();
}

//------------------------------------------------------------------------
// UVoicevoxTextToSpeechAsyncTask
//------------------------------------------------------------------------

/**
 * @brief 非同期でVOICEVOX COERで変換した音声データを取得(Blueprint公開ノード)
 */
UVoicevoxTextToSpeechAsyncTask* UVoicevoxTextToSpeechAsyncTask::TextToSpeech(UObject* WorldContextObject, const int32 StyleId, FString Message, const bool bRunKana, const bool bEnableInterrogativeUpspeak)
{
	UVoicevoxTextToSpeechAsyncTask* Task = NewObject<UVoicevoxTextToSpeechAsyncTask>();
	Task->StyleId = StyleId;
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
UVoicevoxTextToSpeechAsyncTask* UVoicevoxTextToSpeechAsyncTask::TextToAudioQuery(UObject* WorldContextObject, const int32 StyleId, FString Message, const bool bRunKana, const bool bEnableInterrogativeUpspeak)
{
	UVoicevoxTextToSpeechAsyncTask* Task = NewObject<UVoicevoxTextToSpeechAsyncTask>();
	Task->StyleId = StyleId;
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
			UVoicevoxBlueprintLibrary::TextToAudioQueryOutput(StyleId, Message, bRunKana, bEnableInterrogativeUpspeak) :
			UVoicevoxBlueprintLibrary::TextToSpeechOutput(StyleId, Message, bRunKana, bEnableInterrogativeUpspeak);
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
UVoicevoxAudioQueryToSpeechAsyncTask* UVoicevoxAudioQueryToSpeechAsyncTask::AudioQueryOutput(UObject* WorldContextObject, const int32 StyleId, const FVoicevoxAudioQuery AudioQuery, const bool bEnableInterrogativeUpspeak)
{
	UVoicevoxAudioQueryToSpeechAsyncTask* Task = NewObject<UVoicevoxAudioQueryToSpeechAsyncTask>();
	Task->StyleId = StyleId;
	Task->AudioQuery = AudioQuery;
	Task->bEnableInterrogativeUpspeak = bEnableInterrogativeUpspeak;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

UVoicevoxAudioQueryToSpeechAsyncTask* UVoicevoxAudioQueryToSpeechAsyncTask::VoicevoxQueryOutput(UObject* WorldContextObject, UVoicevoxQuery* VoicevoxQuery, const bool bEnableInterrogativeUpspeak)
{
	UVoicevoxAudioQueryToSpeechAsyncTask* Task = NewObject<UVoicevoxAudioQueryToSpeechAsyncTask>();
	if (VoicevoxQuery == nullptr)
	{
		Task->StyleId = 0;
		return nullptr;
	}
	Task->StyleId = VoicevoxQuery->StyleId;
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
	if (StyleId == 0)
	{
		OnFail.Broadcast();
		SetReadyToDestroy();
		return;
	}
	
	Task = UE::Tasks::Launch<>(TEXT("VoicevoxCoreTextToSpeechTask"), [&]
	{
		if (USoundWave* Sound = UVoicevoxBlueprintLibrary::AudioQueryOutput(AudioQuery, StyleId, bEnableInterrogativeUpspeak);
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