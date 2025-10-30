// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VoicevoxEngineのMeta情報を表示するウィジェットcppファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxMetaWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "HAL/PlatformApplicationMisc.h"


/**
 * @brief Construct
 */
void SVoicevoxMetaWidget::Construct(const FArguments& InArgs)
{
	MetaData = InArgs._MetaData;
	CopiedMessage = TEXT("");

	const TSharedRef<SVerticalBox> StylesBox = SNew(SVerticalBox);

	for (const FVoicevoxStyle& Style : MetaData->Styles)
	{
		StylesBox->AddSlot()
			.AutoHeight()
			.Padding(10, 2)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromString(FString::Printf(TEXT(" - %s (Id=%d)"), *Style.Name, Style.Id)))
				]
				+ SHorizontalBox::Slot().AutoWidth()
				.Padding(8, 0)
				[
					SNew(SButton)
					.Text(FText::FromString("Copy"))
					.OnClicked_Lambda([Style, this]()
					{
						const FString IdString = FString::FromInt(Style.Id);
						FPlatformApplicationMisc::ClipboardCopy(*IdString);
						UE_LOG(LogTemp, Log, TEXT("Copied Style ID: %s"), *IdString);

						// メッセージ更新
						CopiedMessage = FString::Printf(TEXT("Copied: %d"), Style.Id);
						Invalidate(EInvalidateWidgetReason::Layout);

						if (const UWorld* World = GEditor->GetEditorWorldContext().World())
						{
							// 数秒後にメッセージを消去
							FTimerHandle TimerHandle;
							World->GetTimerManager().SetTimer(TimerHandle, [this]()
							{
								CopiedMessage = TEXT("");
								Invalidate(EInvalidateWidgetReason::Layout);
							}, 2.0f, false);
						}
						
						return FReply::Handled();
					})
					.ToolTipText(FText::FromString("Copy this Style ID to clipboard"))
				]
				// Copied メッセージ
				+ SHorizontalBox::Slot().AutoWidth()
				.Padding(8, 0)
				[
					SNew(STextBlock)
					.Text(FText::FromString(CopiedMessage))
					.ColorAndOpacity(FLinearColor::Green)
				]
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

void SVoicevoxMetaWidget::ClearCopiedMessage()
{
	CopiedMessage = TEXT("");
}
