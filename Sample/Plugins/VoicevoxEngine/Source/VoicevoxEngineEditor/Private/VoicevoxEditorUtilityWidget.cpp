// Copyright Yuuki Ogino. All Rights Reserved

#include "VoicevoxEditorUtilityWidget.h"

#include "VoicevoxQuery.h"
#include "VoicevoxQueryFactory.h"
#include "AssetRegistry/AssetRegistryModule.h"

#if WITH_EDITOR
#include "FileHelpers.h"
#endif // WITH_EDITOR

bool UVoicevoxEditorUtilityWidget::IsSetEditorAudioQuery()
{
    // 生成されればカナが空っぽになることは無いので、この値で生成済みかどうかを判定する
    return !EditorAudioQueryPtr.Kana.IsEmpty();
}

void UVoicevoxEditorUtilityWidget::SaveAudioQueryAssets()
{
    UVoicevoxQueryFactory* Factory = NewObject<UVoicevoxQueryFactory>();
    Factory->AudioQueryPtr = &EditorAudioQueryPtr;
    const FString AssetName = TEXT("SampleAudioQuery");
    const FString AssetPath = TEXT("/Game");
    const FString PackageName = AssetPath + TEXT("/") + AssetName;
    UPackage* AssetPackage = CreatePackage(*PackageName);
    constexpr EObjectFlags Flags = RF_Public | RF_Standalone;

    if (UObject* CreatedAsset = Factory->FactoryCreateNew(UVoicevoxQuery::StaticClass(), AssetPackage, FName(*AssetName), Flags, NULL, GWarn))
    {
        // Notify the asset registry
        FAssetRegistryModule::AssetCreated(CreatedAsset);

        // Mark the package dirty...
        AssetPackage->MarkPackageDirty();
    }
}

void UVoicevoxEditorUtilityWidget::ExecuteSaveAssets(const TArray<UObject*>&  InTargets,
                                                const bool                bCheckDirty,
                                                const bool                bPromptToSave)
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

    if (0 < PackagesToSave.Num())
    {
        FEditorFileUtils::PromptForCheckoutAndSave(
            PackagesToSave,
            bCheckDirty,
            bPromptToSave);
    }
}
