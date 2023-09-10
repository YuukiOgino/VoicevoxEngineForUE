// Copyright Yuuki Ogino. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "VoicevoxCoreUtil.h"
#include "VoicevoxEditorUtilityWidget.generated.h"

class AActor;
class UEditorPerProjectUserSettings;

/**
 * 
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API UVoicevoxEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(Category=VOICEVOX, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	FVoicevoxAudioQuery EditorAudioQueryPtr;

};
