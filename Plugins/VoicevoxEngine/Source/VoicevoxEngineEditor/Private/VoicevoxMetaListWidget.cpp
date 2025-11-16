// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VoicevoxEngineのMeta情報一覧を表示するウィジェットcppファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxMetaListWidget.h"

#include "EditorStyleSet.h"
#include "VoicevoxMetaWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"

/**
 * @brief Construct
 */
void SVoicevoxMetaListWidget::Construct(const FArguments& InArgs)
{
	MetaArray = InArgs._MetaArray;

	const TSharedRef<SVerticalBox> MetaListBox = SNew(SVerticalBox);

	if (MetaArray && MetaArray->Num() > 0)
	{
		for (const FVoicevoxMeta& Meta : *MetaArray)
		{
			MetaListBox->AddSlot()
			.AutoHeight()
			.Padding(5)
			[
				SNew(SBorder)
				.Padding(8)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				[
					SNew(SVoicevoxMetaWidget)
					.MetaData(&Meta)
				]
			];
		}
	}
	else
	{
		// データがない場合の表示
		MetaListBox->AddSlot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("No VOICEVOX CORE Speaker data available.")))
		];
	}

	ChildSlot
	[
		SNew(SScrollBox)
		+ SScrollBox::Slot()
		[
			MetaListBox
		]
	];
}

