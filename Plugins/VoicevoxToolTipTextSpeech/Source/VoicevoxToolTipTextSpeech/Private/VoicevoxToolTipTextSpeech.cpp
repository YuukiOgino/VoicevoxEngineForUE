// Copyright Yuuki Ogino. All Rights Reserved.

#include "VoicevoxToolTipTextSpeech.h"
#include "VoicevoxToolTipTextSpeechCommands.h"

#if PLATFORM_WINDOWS
#include "VoicevoxCoreUtil.h"
#include "Windows/WindowsHWrapper.h"
#include <playsoundapi.h>
#endif

// Todo Naotsunさんが公開されたサンプルソースコードをインクルード
#include "SlateTextAccessors/DefaultSlateTextAccessor.h"
//↑ここまで

#define LOCTEXT_NAMESPACE "FVoicevoxToolTipTextSpeechModule"

void FVoicevoxToolTipTextSpeechModule::StartupModule()
{
	FVoicevoxToolTipTextSpeechCommands::Register();
	FVoicevoxToolTipTextSpeechCommands::Bind();
	VoicevoxToolTipTextSpeech::FDefaultSlateTextAccessor::Register();
#if PLATFORM_WINDOWS
	FVoicevoxCoreUtil::Initialize(false);
	constexpr int SpeakerId = static_cast<int64>(ESpeakerType::Zundamon);
	int OutputBinarySize = 0;
	uint8* OutputWAV = FVoicevoxCoreUtil::RunTextToSpeech(SpeakerId, TEXT("アンリアルエンジン、ファイブ"), OutputBinarySize);
	PlaySound(reinterpret_cast<LPCTSTR>(OutputWAV), nullptr, SND_MEMORY);
	FVoicevoxCoreUtil::WavFree(OutputWAV);	
#endif
}

void FVoicevoxToolTipTextSpeechModule::ShutdownModule()
{
	VoicevoxToolTipTextSpeech::FDefaultSlateTextAccessor::Unregister();
	FVoicevoxToolTipTextSpeechCommands::Unregister();
#if PLATFORM_WINDOWS
	FVoicevoxCoreUtil::Finalize();
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVoicevoxToolTipTextSpeechModule, VoicevoxToolTipTextSpeech)