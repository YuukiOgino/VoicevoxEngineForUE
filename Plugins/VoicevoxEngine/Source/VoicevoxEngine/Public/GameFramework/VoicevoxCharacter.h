// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxCharacter.h
 * @brief  VOICEVOXのリップシンクコンポーネントを搭載したキャラクターヘッダーファイル
 * @author Yuuki Ogino
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/VoicecoxCharacterLipSyncAudioComponent.h"
#include "VoicevoxCharacter.generated.h"

/**
 * @class AVoicevoxCharacter
 * @brief VOICEVOXのリップシンクコンポーネントを搭載したキャラクタークラス
 */
UCLASS()
class VOICEVOXENGINE_API AVoicevoxCharacter : public ACharacter
{
	GENERATED_BODY()

	//! リップシンクオーディオコンポーネント 
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UVoicecoxCharacterLipSyncAudioComponent> LipSyncAudioComponent;
	
public:
	
	/**
	 * @brief コンストラクタ
	 * @param ObjectInitializer 
	 */
	explicit AVoicevoxCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//! リップシンクコンポーネント名
	static FName LipSyncAudioComponentName;
	

};
