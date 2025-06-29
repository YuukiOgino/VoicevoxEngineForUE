// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOXのリップシンクコンポーネントを搭載したキャラクターCPPファイル
 * @author Yuuki Ogino
 */

#include "GameFramework/VoicevoxCharacter.h"

FName AVoicevoxCharacter::LipSyncAudioComponentName(TEXT("Audio"));

/**
 * @brief コンストラクタ
 */
AVoicevoxCharacter::AVoicevoxCharacter(const FObjectInitializer& ObjectInitializer) : ACharacter(ObjectInitializer)
{
	LipSyncAudioComponent = CreateDefaultSubobject<UVoicecoxCharacterLipSyncAudioComponent>(LipSyncAudioComponentName);
	if (LipSyncAudioComponent)
	{
		LipSyncAudioComponent->SetSkeletalMesh(GetMesh());
	}
}

