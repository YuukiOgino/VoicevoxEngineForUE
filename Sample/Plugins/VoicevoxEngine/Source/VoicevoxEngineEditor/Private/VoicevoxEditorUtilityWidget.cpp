// Copyright Yuuki Ogino. All Rights Reserved

#include "VoicevoxEditorUtilityWidget.h"

#if WITH_EDITOR
#include "FileHelpers.h"
#endif // WITH_EDITOR

void UVoicevoxEditorUtilityWidget::ExecuteSaveAssets(const TArray<UObject*>&  InTargets,
                                                const bool                bCheckDirty,
                                                const bool                bPromptToSave)
{
    TArray<UPackage*> PackagesToSave;
    for (UObject* Obj : InTargets)
    {
        if (IsValid(Obj))
        {
            if (Obj->HasAnyFlags(RF_Transient))
            {
                continue;
            }

            UPackage* Package = Obj->GetOutermost();
            if (IsValid(Package))
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
