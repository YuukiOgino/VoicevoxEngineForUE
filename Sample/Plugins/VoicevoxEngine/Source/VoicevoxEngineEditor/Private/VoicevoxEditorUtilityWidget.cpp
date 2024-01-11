// Copyright Yuuki Ogino. All Rights Reserved

#include "VoicevoxEditorUtilityWidget.h"

#include "AssetToolsModule.h"
#include "VoicevoxBlueprintLibrary.h"
#include "VoicevoxQueryFactory.h"
#include "VoicevoxSoundWaveFactory.h"
#include "AssetRegistry/AssetRegistryModule.h"

#if WITH_EDITOR
#include "FileHelpers.h"
#endif // WITH_EDITOR

DEFINE_LOG_CATEGORY(LogVoicevoxEditor);

bool UVoicevoxEditorUtilityWidget::IsSetEditorAudioQuery()
{
    // 生成されればカナが空っぽになることは無いので、この値で生成済みかどうかを判定する
    return !EditorAudioQueryPtr.Kana.IsEmpty();
}

void UVoicevoxEditorUtilityWidget::SaveAudioQueryAssets(ESpeakerType SpeakerType)
{
    UVoicevoxQueryFactory* Factory = NewObject<UVoicevoxQueryFactory>();
    Factory->AudioQueryPtr = &EditorAudioQueryPtr;
    Factory->SpeakerType = SpeakerType;
    Factory->AddToRoot();
    
    const FAssetToolsModule& AssetToolsModule = FAssetToolsModule::GetModule();
    if (UObject* CreatedAsset = AssetToolsModule.Get().CreateAssetWithDialog(Factory->GetSupportedClass(), Factory); CreatedAsset != nullptr)
    {
        FAssetRegistryModule::AssetCreated(CreatedAsset);
        if (const bool IsMark = CreatedAsset->MarkPackageDirty(); !IsMark)
        {
            UE_LOG(LogVoicevoxEditor, Log, TEXT("MarkPackageDirty Error:Create AudioQuery Asset"));
        }
    }
    Factory->RemoveFromRoot();
}

void UVoicevoxEditorUtilityWidget::SaveSoundWaveAssets(const ESpeakerType SpeakerType, const bool bEnableInterrogativeUpspeak) const
{
    if (const TArray<uint8> OutputWAV = FVoicevoxCoreUtil::RunSynthesis(EditorAudioQueryPtr, static_cast<int64>(SpeakerType), bEnableInterrogativeUpspeak); !OutputWAV.IsEmpty())
    {
        UVoicevoxSoundWaveFactory* Factory = NewObject<UVoicevoxSoundWaveFactory>();
        Factory->OutputWAV = OutputWAV;
        Factory->AddToRoot();
    
        const FAssetToolsModule& AssetToolsModule = FAssetToolsModule::GetModule();
        if (UObject* CreatedAsset = AssetToolsModule.Get().CreateAssetWithDialog(Factory->GetSupportedClass(), Factory); CreatedAsset != nullptr)
        {
            FAssetRegistryModule::AssetCreated(CreatedAsset);
            if (const bool IsMark = CreatedAsset->MarkPackageDirty(); !IsMark)
            {
                UE_LOG(LogVoicevoxEditor, Log, TEXT("MarkPackageDirty Error:Create SoundWave Asset"));
            }
        }
        Factory->RemoveFromRoot();
    }
}

void UVoicevoxEditorUtilityWidget::ExecuteSaveAssets(const TArray<UObject*>& InTargets, const bool bCheckDirty, const bool bPromptToSave)
{
    TArray<UPackage*> PackagesToSave;
    for (const UObject* Obj : InTargets)
    {
        if (IsValid(Obj))
        {
            if (Obj->HasAnyFlags(RF_Transient))
            {
                continue;
            }

            if (UPackage* Package = Obj->GetOutermost(); IsValid(Package))
            {
                PackagesToSave.Add(Package);
            }
        }
    }

    if (PackagesToSave.Num() > 0)
    {
        FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, bCheckDirty, bPromptToSave);
    }
}
