#include "VoicevoxQueryTypeActions.h"
#include "VoicevoxQuery.h"

FVoicevoxQueryTypeActions::FVoicevoxQueryTypeActions():
	Name(FText::FromString(TEXT("Voicevox AudioQuery"))),
	AssetCategoryBit(EAssetTypeCategories::Misc)
{
}

FVoicevoxQueryTypeActions::FVoicevoxQueryTypeActions(EAssetTypeCategories::Type AssetCategoryBit):
	Name(FText::FromString(TEXT("Voicevox AudioQuery"))),
	AssetCategoryBit(AssetCategoryBit)
{
}

FText FVoicevoxQueryTypeActions::GetName() const
{
	return Name;
}

UClass* FVoicevoxQueryTypeActions::GetSupportedClass() const
{
	return UVoicevoxQuery::StaticClass();
}

FColor FVoicevoxQueryTypeActions::GetTypeColor() const
{
	return FColor::Purple;
}

uint32 FVoicevoxQueryTypeActions::GetCategories()
{
	return AssetCategoryBit;
}

void FVoicevoxQueryTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	for(int i = 0; i < InObjects.Num(); ++i)
	{
		FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects[i]);
	}
};