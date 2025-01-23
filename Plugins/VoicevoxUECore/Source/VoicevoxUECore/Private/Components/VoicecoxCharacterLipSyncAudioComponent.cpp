// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOXのAudioQueryを解析して音再生とスケルタルメッシュコンポーネントにリップシンクを行うコンポーネントのCPPファイル
 * @author Yuuki Ogino
 */

#include "Components/VoicecoxCharacterLipSyncAudioComponent.h"

void UVoicecoxCharacterLipSyncAudioComponent::SetSkeletalMesh(USkeletalMeshComponent* SkeletalMesh)
{
	SkeletalMeshComponent = SkeletalMesh;
}

void UVoicecoxCharacterLipSyncAudioComponent::NotificationMorphNum(TMap<ELipSyncVowelType, float> Map)
{
	for (const auto Result : Map)
	{
		SkeletalMeshComponent->SetMorphTarget(LipSyncMorphNameMap[Result.Key], Result.Value);
	}
}