// Copyright Yuuki Ogino. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "VoicevoxCoreUtil.h"
#include "VoicevoxQuery.h"
#include "VoicevoxEditorUtilityWidget.generated.h"

class AActor;
class UEditorPerProjectUserSettings;

DECLARE_DELEGATE_OneParam(FOnAudioQueryPickingChosen, const TArray<FAssetData>&);
DECLARE_DELEGATE(FOnAudioQueryPickingCancelled);

//------------------------------------------------------------------------
// class
//------------------------------------------------------------------------

/**
 * @class UVoicevoxEditorUtilityWidget
 */
UCLASS()
class VOICEVOXENGINEEDITOR_API UVoicevoxEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(Category=VOICEVOX, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	FVoicevoxAudioQuery EditorAudioQueryPtr;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "IsSetEditorAudioQuery"))
	bool IsSetEditorAudioQuery(); 
	
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "SaveAudioQuearyAssets"))
	void SaveAudioQueryAssets(int64 SpeakerType, FString Text);

	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "SaveSoundWaveAssets"))
	void SaveSoundWaveAssets(int64 SpeakerType, bool bEnableInterrogativeUpspeak = true) const;

	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "SaveWavFile"))
	void SaveWavFile(int64 SpeakerType, bool bEnableInterrogativeUpspeak = true) const;
	
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "LoadAudioQuearyAssets"))
	void LoadAudioQueryAssets();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLoadAudioQuery(int64 SpeakerType, const FString& Text);
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxEditor, Log, All);
