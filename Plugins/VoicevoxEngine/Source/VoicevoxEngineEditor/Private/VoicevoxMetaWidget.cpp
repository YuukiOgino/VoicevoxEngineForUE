// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VoicevoxEngineのMeta情報を表示するウィジェットcppファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxMetaWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"

/**
 * @brief Construct
 */
void SVoicevoxMetaWidget::Construct(const FArguments& InArgs)
{
	MetaData = InArgs._MetaData;

	const TSharedRef<SVerticalBox> StylesBox = SNew(SVerticalBox);

	for (const FVoicevoxStyle& Style : MetaData->Styles)
	{
		StylesBox->AddSlot()
		.AutoHeight()
		.Padding(5, 2)
		[
			SNew(STextBlock)
			.Text(FText::FromString(FString::Printf(TEXT("- %s (ID: %d)"), *Style.Name, Style.Id)))
		];
	}
	
	ChildSlot
	[
		SNew(SBorder)
		.Padding(10)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString::Printf(TEXT("%s (Version: %s)"), *MetaData->Name, *MetaData->Version)))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString::Printf(TEXT("UUID: %s"), *MetaData->Speaker_uuid)))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("Styles:")))
			]
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SScrollBox)
				+ SScrollBox::Slot()
				[
					StylesBox
				]
			]
		]
	];
}
