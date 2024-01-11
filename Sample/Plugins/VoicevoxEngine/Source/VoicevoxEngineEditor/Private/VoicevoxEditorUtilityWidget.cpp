// Copyright Yuuki Ogino. All Rights Reserved

#include "VoicevoxEditorUtilityWidget.h"

#include "VoicevoxBlueprintLibrary.h"
#include "VoicevoxQuery.h"
#include "VoicevoxQueryFactory.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Sound/SoundWave.h"

#if WITH_EDITOR
#include "FileHelpers.h"
#endif // WITH_EDITOR

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
    
    // Todo エディタα板はGameフォルダ直下固定、仮のファイル名で保存する（ファイルセーブだとコンテンツフォルダ外に保存が可能なため、別の手段が無いかβ版までに調査）
    const FString AssetName = TEXT("NewAudioQuery");
    const FString AssetPath = TEXT("/Game");
    const FString PackageName = AssetPath + TEXT("/") + AssetName;
    UPackage* AssetPackage = CreatePackage(*PackageName);
    constexpr EObjectFlags Flags = RF_Public | RF_Standalone;

    if (UObject* CreatedAsset = Factory->FactoryCreateNew(UVoicevoxQuery::StaticClass(), AssetPackage, FName(*AssetName), Flags, nullptr, GWarn))
    {
        FAssetRegistryModule::AssetCreated(CreatedAsset);
        AssetPackage->MarkPackageDirty();
    }
}

void UVoicevoxEditorUtilityWidget::SaveSoundWaveAssets(ESpeakerType SpeakerType, const bool bEnableInterrogativeUpspeak)
{
    // Todo エディタα板はGameフォルダ直下固定、仮のファイル名で保存する（ファイルセーブだとコンテンツフォルダ外に保存が可能なため、別の手段が無いかβ版までに調査）
    const FString AssetName = TEXT("NewVoice");
    const FString AssetPath = TEXT("/Game");
    const FString PackageName = AssetPath + TEXT("/") + AssetName;
    UPackage* AssetPackage = CreatePackage(*PackageName);
    if (const TArray<uint8> OutputWAV = FVoicevoxCoreUtil::RunSynthesis(EditorAudioQueryPtr, static_cast<int64>(SpeakerType), bEnableInterrogativeUpspeak); !OutputWAV.IsEmpty())
    {
        UObject* CreatedAsset = UVoicevoxBlueprintLibrary::CreateSoundWave(OutputWAV, AssetPackage, FName(*AssetName));
        FAssetRegistryModule::AssetCreated(CreatedAsset);
        AssetPackage->MarkPackageDirty();
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
