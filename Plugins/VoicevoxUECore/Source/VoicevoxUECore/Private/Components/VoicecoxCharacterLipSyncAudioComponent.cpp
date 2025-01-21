// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/VoicecoxCharacterLipSyncAudioComponent.h"


// Sets default values for this component's properties
UVoicecoxCharacterLipSyncAudioComponent::UVoicecoxCharacterLipSyncAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	OnAudioPlaybackPercentNative.AddUObject(this, &UVoicecoxCharacterLipSyncAudioComponent::HandlePlaybackPercent);

	LipSyncMorphNameMap.Reserve(5);
	LipSyncMorphNameMap.Add(ELipSyncVowelType::A, NAME_None);
	LipSyncMorphNameMap.Add(ELipSyncVowelType::I, NAME_None);
	LipSyncMorphNameMap.Add(ELipSyncVowelType::U, NAME_None);
	LipSyncMorphNameMap.Add(ELipSyncVowelType::E, NAME_None);
	LipSyncMorphNameMap.Add(ELipSyncVowelType::O, NAME_None);

	LipSyncMorphNumMap.Reserve(5);
	LipSyncMorphNumMap.Add(ELipSyncVowelType::A, 0.0f);
	LipSyncMorphNumMap.Add(ELipSyncVowelType::I, 0.0f);
	LipSyncMorphNumMap.Add(ELipSyncVowelType::U, 0.0f);
	LipSyncMorphNumMap.Add(ELipSyncVowelType::E, 0.0f);
	LipSyncMorphNumMap.Add(ELipSyncVowelType::O, 0.0f);
}


// Called when the game starts
void UVoicecoxCharacterLipSyncAudioComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVoicecoxCharacterLipSyncAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UVoicecoxCharacterLipSyncAudioComponent::HandlePlaybackPercent(const UAudioComponent* InComponent, const USoundWave* InSoundWave, const float InPlaybackPercentage)
{
	// ループ無しかつ最後まで再生しても止まらない場合があるので、明確にストップする
	if (Sound != nullptr && !Sound->IsLooping() && InPlaybackPercentage >= 1.0f)
	{
		Stop();
		InitMorphNumMap();
		for (const auto Result : LipSyncMorphNumMap)
		{
			SkeletalMeshComponent->SetMorphTarget(LipSyncMorphNameMap[Result.Key], Result.Value);
		}
		return;
	}
	if (LipSyncList.IsEmpty()) return;

	const float NowDuration = Sound->Duration * InPlaybackPercentage;
	if (LipSyncTime < NowDuration)
	{
		// 前回のリップシンク情報を元に初期化
		if (NowLipSync.IsConsonant)
		{
			if (NowLipSync.IsLabialOrPlosive)
			{
				for (const auto Result : LipSyncMorphNumMap)
				{
					SkeletalMeshComponent->SetMorphTarget(LipSyncMorphNameMap[Result.Key], Result.Value);
				}
			}
		}
		else
		{
			// 母音の初期化
			// 前回のリップシンク情報から最大値を更新
			switch (NowLipSync.VowelType)
			{
			case ELipSyncVowelType::A:
				InitMorphNumMap();
				LipSyncMorphNumMap[ELipSyncVowelType::A] = MaxMouthScale * 0.8f;
				break;
			case ELipSyncVowelType::I:
				InitMorphNumMap();
				LipSyncMorphNumMap[ELipSyncVowelType::I] = MaxMouthScale * 0.8f;
				break;
			case ELipSyncVowelType::U:
				InitMorphNumMap();
				LipSyncMorphNumMap[ELipSyncVowelType::U] = MaxMouthScale * 0.8f;
				break;
			case ELipSyncVowelType::E:
				InitMorphNumMap();
				LipSyncMorphNumMap[ELipSyncVowelType::E] = MaxMouthScale * 0.8f;
				break;
			case ELipSyncVowelType::O:
				InitMorphNumMap();
				LipSyncMorphNumMap[ELipSyncVowelType::O] = MaxMouthScale * 0.8f;
				break;
			case ELipSyncVowelType::CL:
				LipSyncMorphNumMap[ELipSyncVowelType::A] = LipSyncMorphNumMap[ELipSyncVowelType::A] * 0.8f * 0.8f;
				LipSyncMorphNumMap[ELipSyncVowelType::I] = LipSyncMorphNumMap[ELipSyncVowelType::I] * 0.8f * 0.8f;
				LipSyncMorphNumMap[ELipSyncVowelType::U] = LipSyncMorphNumMap[ELipSyncVowelType::U] * 0.8f * 0.8f;
				LipSyncMorphNumMap[ELipSyncVowelType::E] = LipSyncMorphNumMap[ELipSyncVowelType::E] * 0.8f * 0.8f;
				LipSyncMorphNumMap[ELipSyncVowelType::O] = LipSyncMorphNumMap[ELipSyncVowelType::O] * 0.8f * 0.8f;
				break;
			default:
				InitMorphNumMap();
				break;
			}

			for (const auto Result : LipSyncMorphNumMap)
			{
				SkeletalMeshComponent->SetMorphTarget(LipSyncMorphNameMap[Result.Key], Result.Value);
			}
		}
		
		NowLipSync = LipSyncList.Pop();
		LipSyncTime += NowLipSync.Length;
	}
	
	const float NowLength = (LipSyncTime - NowDuration) / NowLipSync.Length;
	if (NowLipSync.IsConsonant)
	{
		if (NowLipSync.IsLabialOrPlosive)
		{
			for (auto Map = UpdateConsonantMorphNum(NowLength); const auto Result : Map)
			{
				SkeletalMeshComponent->SetMorphTarget(LipSyncMorphNameMap[Result.Key], Result.Value);
			}
		}
	}
	else if (NowLipSync.VowelType == ELipSyncVowelType::Non)
	{
		for (auto Map = UpdatePauseMorphNum(NowLength); const auto Result : Map)
		{
			SkeletalMeshComponent->SetMorphTarget(LipSyncMorphNameMap[Result.Key], Result.Value);
		}
	}
	else
	{
		for (auto Map = UpdateVowelMorphNum(NowLength); const auto Result : Map)
		{
			SkeletalMeshComponent->SetMorphTarget(LipSyncMorphNameMap[Result.Key], Result.Value);
		}
	}
}

void UVoicecoxCharacterLipSyncAudioComponent::SetSkeletalMesh(USkeletalMeshComponent* SkeletalMesh)
{
	SkeletalMeshComponent = SkeletalMesh;
}
