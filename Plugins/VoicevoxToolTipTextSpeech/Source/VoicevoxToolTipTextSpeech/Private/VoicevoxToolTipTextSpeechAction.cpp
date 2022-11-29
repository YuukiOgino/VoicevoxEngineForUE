// Copyright Yuuki Ogino. All Rights Reserved.

#include "VoicevoxToolTipTextSpeechActions.h"

// Todo Naotsunさんが公開されたサンプルソースコードをインクルード
#include "Utilities/SamplePluginSlateHelpers.h"
#include "SlateTextAccessors/TooltipTextAccessor.h"
//↑ここまで 

#if PLATFORM_WINDOWS
#include "VoicevoxCoreUtil.h"
#include "Windows/WindowsHWrapper.h"
#include <playsoundapi.h>
#endif

void FVoicevoxToolTipTextSpeechActions::VoicevoxTooltipTextSpeechAccessor()
{
	const TSharedPtr<SWidget> TooltipWidget = FSamplePluginSlateHelpers::GetTooltipWidget();
	
	TSharedPtr<VoicevoxToolTipTextSpeech::ITooltipTextAccessor> TooltipText = nullptr;
	{
		TArray<TSharedPtr<SWidget>> ChildWidgets;
		FSamplePluginSlateHelpers::GetAllChildWidgets(TooltipWidget, ChildWidgets);
		for (const auto& ChildWidget : ChildWidgets)
		{
			TooltipText = VoicevoxToolTipTextSpeech::FTooltipTextAccessorFactory::CreateAccessor(ChildWidget);
			if (TooltipText.IsValid())
			{
				break;
			}
		}
	}

	if (TooltipText.IsValid())
	{
		if (const FText& TextTooltip = TooltipText->GetTextTooltip(); !TextTooltip.IsEmpty())
		{
			// ToolTipの文言を読み上げる
#if PLATFORM_WINDOWS
			constexpr int SpeakerId = static_cast<int64>(ESpeakerType::Zundamon);
			int OutputBinarySize = 0;
			uint8* OutputWAV = FVoicevoxCoreUtil::RunTextToSpeech(SpeakerId, *TextTooltip.ToString(), OutputBinarySize);
			PlaySound(reinterpret_cast<LPCTSTR>(OutputWAV), nullptr, SND_MEMORY);
			FVoicevoxCoreUtil::WavFree(OutputWAV);	
#endif			
		}
	}
}
