#pragma once

#include "Subsystems/VoicevoxNativeCoreSubsystem.h"
#include "CoreSubsystem.generated.h"

UCLASS(MinimalAPI)
class UCoreSubsystem : public UVoicevoxNativeCoreSubsystem
{
	GENERATED_BODY()
public:
	VOICEVOXNATIVECORE_API UCoreSubsystem();
};
