// Copyright Yuuki Ogino. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FVoicevoxToolTipTextSpeechCommands : public TCommands<FVoicevoxToolTipTextSpeechCommands>
{
	bool bIsBound;
public:
	
	TSharedPtr<FUICommandInfo> VoicevoxToolTipTextSpeechAccessor;
	
	FVoicevoxToolTipTextSpeechCommands();
	
	virtual void RegisterCommands() override;
	
	static bool IsBound();
	
	static void Bind();
protected:
	virtual void BindCommands();
};