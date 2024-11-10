#pragma once
#include "Subsystems/VoicevoxNativeCoreSubsystem.h"

class UVoicevoxNativeObject;

/**
 * @class FVoicevoxSubsystemCollection
 * @brief VOICEVOX COREのネイティブAPIを参照するSubsystem管理クラス
 */
class FVoicevoxSubsystemCollection final : public FSubsystemCollectionBase
{
	friend class UVoicevoxNativeObject;

	/**
	 * @brief 指定したクラスのSubsystemを取得
	 * @param SubsystemClass 
	 * @return 指定したVOICEVOX CORE Subsystemクラス
	 */
	USubsystem* GetSubsystem(UClass* SubsystemClass) const
	{
		return GetSubsystemInternal(SubsystemClass);
	}
	
public:
	/**
	 * @brief コンストラクタ
	 */
	FVoicevoxSubsystemCollection()
	: FSubsystemCollectionBase(UVoicevoxNativeCoreSubsystem::StaticClass())
	{
	}
};
