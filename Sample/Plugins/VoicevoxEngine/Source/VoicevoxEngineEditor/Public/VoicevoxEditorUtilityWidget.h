// Copyright Yuuki Ogino. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "VoicevoxCoreUtil.h"
#include "VoicevoxEditorUtilityWidget.generated.h"

class AActor;
class UEditorPerProjectUserSettings;

DECLARE_DELEGATE_OneParam(FOnAudioQueryPickingChosen, const TArray<FAssetData>&);
DECLARE_DELEGATE(FOnAudioQueryPickingCancelled);

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

	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "LoadAudioQuearyAssets"))
	void LoadAudioQueryAssets();
	
	/**
	* アセットの保存を実行します
	*
	* @param [in]  InTargets           対象アセット
	* @param [in]  bCheckDirty         trueの場合は変更(Dirty)アセットのみが保存されます
	* @param [in]  bPromptToSave       trueの場合はアセットの保存確認を求められます。falseの場合は全て保存されます。
	*/
	UFUNCTION(BlueprintCallable, Category="VOICEVOX Editor", meta=(Keywords="voicevox", DisplayName = "ExecuteSaveAssets"))
	void ExecuteSaveAssets(const TArray<UObject*>& InTargets, bool bCheckDirty = true, bool bPromptToSave = false);
	
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxEditor, Log, All);
