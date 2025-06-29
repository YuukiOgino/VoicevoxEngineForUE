// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOXのAudioQueryを解析して音再生とスケルタルメッシュコンポーネントにリップシンクを行うコンポーネントのCPPファイル
 * @author Yuuki Ogino
 */

#include "Components/VoicecoxCharacterLipSyncAudioComponent.h"

DEFINE_LOG_CATEGORY(LogVoicevoxLipSync);

/**
 * @brief リップシンク対象のスケルタルメッシュをセット
 */
void UVoicecoxCharacterLipSyncAudioComponent::SetSkeletalMesh(USkeletalMeshComponent* SkeletalMesh)
{
	SkeletalMeshComponent = SkeletalMesh;
}

/**
 * @brief モーフターゲット値の通知実行
 */
void UVoicecoxCharacterLipSyncAudioComponent::NotificationMorphNum(TMap<ELipSyncVowelType, float> Map)
{
	for (const auto Result : Map)
	{
		if (FName MorphName = LipSyncMorphNameMap[Result.Key]; MorphName == NAME_None)
		{
			FString KeyName = StaticEnum<ELipSyncVowelType>()->GetValueAsString(Result.Key);
			FString Message = FString::Format(TEXT("LipSyncMorphNameMap {0}にモーフターゲットの名前を設定してください。"), { KeyName });
			UE_LOG(LogVoicevoxLipSync, Error, TEXT("%s"), *Message);
			
			const FColor Col = FColor::Yellow;
			const FVector2D Scl = FVector2D(1.0f, 1.0f);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, Col, Message, true, Scl);
		}
		else
		{
			SkeletalMeshComponent->SetMorphTarget(MorphName, Result.Value);
		}
	}
}