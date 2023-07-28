// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOX COREのAPIへ接続するBlueprint公開ノードをまとめたCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxAsyncTask.h"
#include "VoicevoxCoreUtil.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#include <playsoundapi.h>
#endif

/**
 * @brief VOICEVOX COER 初期化(Blueprint公開ノード)
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
 * @brief VOICEVOX COER 終了処理(Blueprint公開ノード)
 */
void UVoicevoxInitializeAsyncTask::Finalize()
{
	FVoicevoxCoreUtil::Finalize();
}

/**
 * @brief VOICEVOX COER メタ情報を取得する(Blueprint公開ノード)
 */
void UVoicevoxInitializeAsyncTask::GetMetasToString(FString& Metas)
{
	Metas = FVoicevoxCoreUtil::Metas();
}



/**
 * @brief VOICEVOX COER メタ情報を取得する(Blueprint公開ノード)
 */
void UVoicevoxInitializeAsyncTask::GetMetasToList(TArray<FVoicevoxMeta>& Metas)
{
	Metas = FVoicevoxCoreUtil::MetaList();
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
 * @brief VOICEVOX COERで変換した音声データを簡易的に再生させる(Blueprint公開ノード)
 */
UVoicevoxSimplePlayTextToSpeechAsyncTask* UVoicevoxSimplePlayTextToSpeechAsyncTask::SimplePlayTextToSpeech(UObject* WorldContextObject, ESpeakerType SpeakerType, FString Message, bool bRunKana)
{
	UVoicevoxSimplePlayTextToSpeechAsyncTask* Task = NewObject<UVoicevoxSimplePlayTextToSpeechAsyncTask>();
	Task->SpeakerId = static_cast<int64>(SpeakerType);
	Task->Message = Message;
	Task->bRunKana = bRunKana;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

/**
 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
 */	
void UVoicevoxSimplePlayTextToSpeechAsyncTask::Activate()
{
	long OutputBinarySize = 0;
	if (uint8* OutputWAV = FVoicevoxCoreUtil::RunTextToSpeech(SpeakerId, *Message, bRunKana, true, OutputBinarySize);
		OutputWAV != nullptr)
	{
#if PLATFORM_WINDOWS
		PlaySound(reinterpret_cast<LPCTSTR>(OutputWAV), nullptr, SND_MEMORY);
#endif
		FVoicevoxCoreUtil::WavFree(OutputWAV);
		OnSuccess.Broadcast();
	}
	else
	{
		OnFail.Broadcast();
	}
		
	SetReadyToDestroy();
}

/**
 * @brief VOICEVOX COERで変換したAudioQuery簡易的に再生させる(Blueprint公開ノード)
 */
UVoicevoxSimplePlayTextToAudioQueryAsyncTask* UVoicevoxSimplePlayTextToAudioQueryAsyncTask::SimplePlayTextToAudioQuery(UObject* WorldContextObject, ESpeakerType SpeakerType, FString Message, bool bRunKana)
{
	UVoicevoxSimplePlayTextToAudioQueryAsyncTask* Task = NewObject<UVoicevoxSimplePlayTextToAudioQueryAsyncTask>();
	Task->SpeakerId = static_cast<int64>(SpeakerType);
	Task->Message = Message;
	Task->bRunKana = bRunKana;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

/**
 * @brief デリゲートがバインドされた後、アクションをトリガーするために呼び出される
 */	
void UVoicevoxSimplePlayTextToAudioQueryAsyncTask::Activate()
{
	char* q = FVoicevoxCoreUtil::RunAudioQuery(SpeakerId, Message, bRunKana);
	long OutputBinarySize = 0;

	if (uint8* OutputWAV = FVoicevoxCoreUtil::RunSynthesis(q, SpeakerId, bRunKana, OutputBinarySize); OutputWAV != nullptr)
	{
#if PLATFORM_WINDOWS
		PlaySound(reinterpret_cast<LPCTSTR>(OutputWAV), nullptr, SND_MEMORY);
#endif
		FVoicevoxCoreUtil::WavFree(OutputWAV);
		OnSuccess.Broadcast();
	}
	else
	{
		OnFail.Broadcast();
	}

	FVoicevoxCoreUtil::AudioQueryFree(q);
	SetReadyToDestroy();
}

/**
 * @brief VOICEVOX COERで変換したAudioQueryを取得する(Blueprint公開ノード)
 */
void UVoicevoxSimplePlayTextToAudioQueryAsyncTask::GetAudioQuery(FString& Query, ESpeakerType SpeakerType, FString Message, bool bRunKana)
{
	Query = UTF8_TO_TCHAR(FVoicevoxCoreUtil::RunAudioQuery(static_cast<int64>(SpeakerType), Message, bRunKana));
}