// Copyright Yuuki Ogino. All Rights Reserved.


#include "Subsystems/VoicevoxCoreSubsystem.h"

/**
 * @brief VOICEVOX CORE 初期化
 */
bool UVoicevoxCoreSubsystem::Initialize(const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
{
	OnInitialize.Broadcast(bUseGPU, CPUNumThreads, bLoadAllModels);
	return true;
}

void UVoicevoxCoreSubsystem::SetOnInitializeDelegate(const TVoicevoxCoreMulticastDelegate<void(bool, int, bool)>& OpenDelegate)
{
	OnInitialize = OpenDelegate;
}