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
	CopyId = -1;

	const TSharedRef<SVerticalBox> StylesBox = SNew(SVerticalBox);

	for (const FVoicevoxStyle& Style : MetaData->Styles)
	{
		StylesBox->AddSlot()
			.AutoHeight()
			.Padding(10, 2)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.MaxWidth(160.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(FString::Printf(TEXT(" - %s (ID=%d)"), *Style.Name, Style.Id)))
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(8, 0)
				[
					SNew(SButton)
					.Text(FText::FromString("Copy"))
					.OnClicked_Lambda([Style, this]
					{
						const FString IdString = FString::FromInt(Style.Id);
						FPlatformApplicationMisc::ClipboardCopy(*IdString);
						UE_LOG(LogTemp, Log, TEXT("Copied Style ID: %s"), *IdString);

						CopyId = Style.Id;

						if (const UWorld* World = GEditor->GetEditorWorldContext().World())
						{
							// 数秒後にメッセージを消去
							FTimerHandle TimerHandle;
							World->GetTimerManager().SetTimer(TimerHandle, [this]
							{
								CopyId = -1;
							}, 2.0f, false);
						}
						
						return FReply::Handled();
					})
					.ToolTipText(FText::FromString("Copy this Style ID to clipboard"))
				]
				+ SHorizontalBox::Slot().AutoWidth()
				.Padding(8, 0)
				[
					SNew(STextBlock)
					.Text_Lambda([Style, this]()
					{
						if (Style.Id == CopyId)
						{
							return FText::FromString(FString::Printf(TEXT("Copied: %d"), Style.Id));
						}
						return FText::FromString("");
					})
					.ColorAndOpacity(FLinearColor::Green)
				]
			];
	}
	auto NameTextStyle = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>( "NormalText" );
	NameTextStyle.SetFontSize(20.f);

	auto StyleTextStyle = FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>( "NormalText" );
	StyleTextStyle.SetFontSize(18.f);
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
				.TextStyle(&NameTextStyle)
				.Text(FText::FromString(FString::Printf(TEXT("%s"), *MetaData->Name)))
			]
			+ SVerticalBox::Slot()
			.Padding(5, 3)
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString::Printf(TEXT("UUID: %s  Version: %s"), *MetaData->Speaker_uuid, *MetaData->Version)))
			]
			+ SVerticalBox::Slot()
			.Padding(0, 3)
			.AutoHeight()
			[
				SNew(STextBlock)
				.TextStyle(&StyleTextStyle)
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