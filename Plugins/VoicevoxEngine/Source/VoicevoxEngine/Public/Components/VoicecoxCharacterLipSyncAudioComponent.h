// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicecoxCharacterLipSyncAudioComponent.h
 * @brief  VOICEVOXのAudioQueryを解析して音再生とスケルタルメッシュコンポーネントにリップシンクを行うコンポーネントのヘッダーファイル
 * @author Yuuki Ogino
 */


#pragma once

#include "CoreMinimal.h"
#include "Components/AbstractLipSyncAudioComponent.h"
#include "VoicevoxUEDefined.h"
#include "VoicecoxCharacterLipSyncAudioComponent.generated.h"

/**
 * @class UVoicecoxCharacterLipSyncAudioComponent
 * @brief VOICEVOXのAudioQueryを解析して、音再生とスケルタルメッシュコンポーネントにリップシンクを行うコンポーネントクラス
 */
UCLASS(ClassGroup=(Audio, Common), HideCategories=(Object, ActorComponent, Physics, Rendering, Mobility, LOD), ShowCategories=Trigger, meta=(BlueprintSpawnableComponent))
class VOICEVOXENGINE_API UVoicecoxCharacterLipSyncAudioComponent : public UAbstractLipSyncAudioComponent
{
	GENERATED_BODY()

	//! リップシンク対象のスケルタルメッシュコンポーネント
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

protected:
	
	virtual void NotificationMorphNum(TMap<ELipSyncVowelType, float> Map) override;
	
public:
	
	/**
	 * @brief リップシンク対象のスケルタルメッシュをセット
	 * @param [in]SkeletalMesh :  リップシンク対象のスケルタルメッシュ
	 */
	UFUNCTION(BlueprintCallable)
	void SetSkeletalMesh(USkeletalMeshComponent* SkeletalMesh);
};