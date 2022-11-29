// Copyright Yuuki Ogino. All Rights Reserved.

#include "VoicevoxToolTipTextSpeechCommands.h"
#include "VoicevoxToolTipTextSpeechActions.h"
#include "Interfaces/IMainFrameModule.h"

#if PLATFORM_WINDOWS
#include "VoicevoxCoreUtil.h"
#endif

#define LOCTEXT_NAMESPACE "FVoicevoxToolTipTextSpeechModule"

#if PLATFORM_WINDOWS
DEFINE_LOG_CATEGORY(LogVoicevoxEngine);
#endif

FVoicevoxToolTipTextSpeechCommands::FVoicevoxToolTipTextSpeechCommands()
	: TCommands<FVoicevoxToolTipTextSpeechCommands>
	(
		TEXT("VoicevoxToolTipTextToSpeech"),
		NSLOCTEXT("Contexts", "VOICEVOX", "VOICEVOX ToolTip Text To Speech"),
		NAME_None,
		FEditorStyle::GetStyleSetName()
	)
	, bIsBound(false)
{
}

void FVoicevoxToolTipTextSpeechCommands::RegisterCommands()
{
	UI_COMMAND(
		VoicevoxToolTipTextSpeechAccessor,
		"VOICEVOX Tooltip Text to Speech Accessor",
		"Read the text of the tooltip on the display.",
		EUserInterfaceActionType::None,
		FInputChord(EKeys::P, false, false, false, false)
	);
}

bool FVoicevoxToolTipTextSpeechCommands::IsBound()
{
	return Instance.Pin()->bIsBound;
}

void FVoicevoxToolTipTextSpeechCommands::Bind()
{
	Instance.Pin()->BindCommands();
}

void FVoicevoxToolTipTextSpeechCommands::BindCommands()
{
	if (!IsRegistered())
	{
#if PLATFORM_WINDOWS
		UE_LOG(LogVoicevoxEngine, Fatal, TEXT("Bound before UI Command was registered.\nPlease be sure to bind after registration."));
#endif
	}

	if (IsBound())
	{
#if PLATFORM_WINDOWS
		UE_LOG(LogVoicevoxEngine, Warning, TEXT("The binding process has already been completed."));
#endif
	}
	bIsBound = true;

	IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	const TSharedRef<FUICommandList>& CommandBindings = MainFrame.GetMainFrameCommandBindings();

	CommandBindings->MapAction(
		VoicevoxToolTipTextSpeechAccessor,
		FExecuteAction::CreateStatic(&FVoicevoxToolTipTextSpeechActions::VoicevoxTooltipTextSpeechAccessor)
	);
}

#undef LOCTEXT_NAMESPACE
