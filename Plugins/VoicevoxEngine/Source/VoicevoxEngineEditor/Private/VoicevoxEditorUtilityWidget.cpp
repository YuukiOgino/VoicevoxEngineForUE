// Copyright Yuuki Ogino. All Rights Reserved

#include "VoicevoxEditorUtilityWidget.h"

#include "AssetToolsModule.h"
#include "ContentBrowserModule.h"
#include "DesktopPlatformModule.h"
#include "IContentBrowserSingleton.h"
#include "IDesktopPlatform.h"
#include "VoicevoxBlueprintLibrary.h"
#include "Factories/VoicevoxQueryFactory.h"
#include "Factories/VoicevoxSoundWaveFactory.h"
#include "AssetRegistry/AssetRegistryModule.h"

DEFINE_LOG_CATEGORY(LogVoicevoxEditor);

bool UVoicevoxEditorUtilityWidget::IsSetEditorAudioQuery()
{
    // 生成されればカナが空っぽになることは無いので、この値で生成済みかどうかを判定する
    return !EditorAudioQueryPtr.Kana.IsEmpty();
}

void UVoicevoxEditorUtilityWidget::SaveAudioQueryAssets(const int64 SpeakerType, FString Text)
{
    UVoicevoxQueryFactory* Factory = NewObject<UVoicevoxQueryFactory>();
    Factory->EditAudioQuery = NewObject<UVoicevoxQuery>();
    Factory->EditAudioQuery->VoicevoxAudioQuery = EditorAudioQueryPtr;
    Factory->EditAudioQuery->SpeakerType = SpeakerType;
    Factory->EditAudioQuery->Text = Text;
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

void UVoicevoxEditorUtilityWidget::SaveWavFile(const int64 SpeakerType, const bool bEnableInterrogativeUpspeak) const
{
    if (const TArray<uint8> OutputWAV = FVoicevoxCoreUtil::RunSynthesis(EditorAudioQueryPtr, SpeakerType, bEnableInterrogativeUpspeak); !OutputWAV.IsEmpty())
    {
        if (IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get())
        {
            TArray<FString> Filenames;
            if(const bool bSaved = DesktopPlatform->SaveFileDialog(
                FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
                TEXT("Save Wave File"),
                TEXT(""),
                TEXT(""),
                TEXT( "Wave file|*.wav" ),
                EFileDialogFlags::None,
                Filenames
            ); bSaved && Filenames.Num() > 0)
            {
                IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
                TUniquePtr<IFileHandle> FileHandle;
                FileHandle.Reset(PlatformFile.OpenWrite(*Filenames[0]));
                if (FileHandle)
                {
                    if (!FileHandle->Write(OutputWAV.GetData(), OutputWAV.Num()))
                    {
                        UE_LOG(LogVoicevoxEditor, Error, TEXT("Error:Can,t Save to Wave File"));
                    }
                }
            }
        }
    }
}

void UVoicevoxEditorUtilityWidget::LoadAudioQueryAssets()
{
    FOpenAssetDialogConfig OpenAssetDialogConfig;
    OpenAssetDialogConfig.DialogTitleOverride = FText::FromString(TEXT("Open Voicevox Audio Query"));
    OpenAssetDialogConfig.DefaultPath = "/Game";
#if (ENGINE_MINOR_VERSION == 0)
    OpenAssetDialogConfig.AssetClassNames.Add(UVoicevoxQuery::StaticClass()->GetFName());
#else
    OpenAssetDialogConfig.AssetClassNames.Add(UVoicevoxQuery::StaticClass()->GetClassPathName());
#endif
    OpenAssetDialogConfig.bAllowMultipleSelection = true;

    const FOnAudioQueryPickingChosen AudioQueryChosenDelegate =
        FOnAudioQueryPickingChosen::CreateWeakLambda(this,[&](const TArray<FAssetData>& SelectedAssets)
        {
            if (SelectedAssets.Num() > 0)
            {
                const FAssetData& FirstAssetData = SelectedAssets[0];
                UObject* Obj = FirstAssetData.GetAsset();
               if(const UVoicevoxQuery* Query = Cast<UVoicevoxQuery>(Obj))
               {
                   EditorAudioQueryPtr = Query->VoicevoxAudioQuery;
                   OnLoadAudioQuery(Query->SpeakerType, Query->Text);
               }
            }
        });
    const FOnAudioQueryPickingCancelled AudioQueryPickingCancelledDelegate =
        FOnAudioQueryPickingCancelled::CreateWeakLambda(this,[&]
        {
            UE_LOG(LogVoicevoxEditor, Log, TEXT("Asset Loader:Load Cancel"));
        });
    
    const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
    ContentBrowserModule.Get().CreateOpenAssetDialog(OpenAssetDialogConfig,
                                                     AudioQueryChosenDelegate,
                                                     AudioQueryPickingCancelledDelegate);
}