#pragma once

#include "Subsystems/VoicevoxNativeCoreSubsystem.h"
#include "CoreSubsystem.generated.h"

UCLASS(MinimalAPI)
class UCoreSubsystem : public UVoicevoxNativeCoreSubsystem
{
	GENERATED_BODY()
public:
	VOICEVOXNATIVECORE_API UCoreSubsystem();

	bool CoreInitialize(bool bUseGPU, int CPUNumThreads = 0, bool bLoadAllModels = false) override;
};
