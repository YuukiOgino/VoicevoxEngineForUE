// Copyright Yuuki Ogino. All Rights Reserved

#include "VoicevoxEditorUtilityWidget.h"

#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "EditorDirectories.h"
#include "IContentBrowserSingleton.h"
#include "VoicevoxBlueprintLibrary.h"
#include "VoicevoxQuery.h"
#include "Factories/VoicevoxQueryFactory.h"
#include "Factories/VoicevoxSoundWaveFactory.h"
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

void UVoicevoxEditorUtilityWidget::SaveAudioQueryAssets(int64 SpeakerType, FString Text)
{
    UVoicevoxQueryFactory* Factory = NewObject<UVoicevoxQueryFactory>();
    Factory->AudioQueryPtr = &EditorAudioQueryPtr;
    Factory->SpeakerType = SpeakerType;
    Factory->Text = Text;
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

void UVoicevoxEditorUtilityWidget::SaveSoundWaveAssets(const int64 SpeakerType, const bool bEnableInterrogativeUpspeak) const
{
    if (const TArray<uint8> OutputWAV = FVoicevoxCoreUtil::RunSynthesis(EditorAudioQueryPtr, SpeakerType, bEnableInterrogativeUpspeak); !OutputWAV.IsEmpty())
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

void UVoicevoxEditorUtilityWidget::LoadAudioQueryAssets()
{
    struct FLocal
    {
        static void OnAudioQuerySelected(const TArray<FAssetData>& SelectedAssets, FOnAudioQueryPickingChosen OnAudioQueryPickingChosenDelegate)
        {
            if (SelectedAssets.Num() > 0)
            {
                const FAssetData& FirstAssetData = SelectedAssets[0];
                FString FilesystemPath = FPackageName::LongPackageNameToFilename(FirstAssetData.PackagePath.ToString() + TEXT("/"));;
                if (FilesystemPath.EndsWith(TEXT("/"), ESearchCase::CaseSensitive))
                {
                    FilesystemPath.LeftChopInline(1, false);
                }

                FEditorDirectories::Get().SetLastDirectory(ELastDirectory::GENERIC_OPEN, FilesystemPath);
                OnAudioQueryPickingChosenDelegate.ExecuteIfBound(SelectedAssets);
            }
        }

        static void OnDialogCancelled(FOnAudioQueryPickingCancelled OnAudioQueryPickingCancelledDelegate)
        {
            OnAudioQueryPickingCancelledDelegate.ExecuteIfBound();
        }
    };
    
    FOpenAssetDialogConfig OpenAssetDialogConfig;
    OpenAssetDialogConfig.DialogTitleOverride = FText::FromString(TEXT("Open Voicevox Audio Query"));
    OpenAssetDialogConfig.DefaultPath = "/Game";
    OpenAssetDialogConfig.AssetClassNames.Add(UVoicevoxQuery::StaticClass()->GetFName());
    OpenAssetDialogConfig.bAllowMultipleSelection = true;

    const FOnAudioQueryPickingChosen AudioQueryChosenDelegate =
        FOnAudioQueryPickingChosen::CreateLambda([&](const TArray<FAssetData>& List)
        {
            
        });
    const FOnAudioQueryPickingCancelled AudioQueryPickingCancelledDelegate =
        FOnAudioQueryPickingCancelled::CreateLambda([&]
        {
            
        });
    
    const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
    ContentBrowserModule.Get().CreateOpenAssetDialog(OpenAssetDialogConfig,
                                                     FOnAssetsChosenForOpen::CreateStatic(&FLocal::OnAudioQuerySelected, AudioQueryChosenDelegate),
                                                     FOnAssetDialogCancelled::CreateStatic(&FLocal::OnDialogCancelled, AudioQueryPickingCancelledDelegate));
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
