// Copyright Yuuki Ogino. All Rights Reserved.


#include "Subsystems/VoicevoxCoreSubsystem.h"

//--------------------------------
// override
//--------------------------------

/**
 * @brief Initialize
 */
void UVoicevoxCoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

/**
 * @brief Deinitialize
 */
void UVoicevoxCoreSubsystem::Deinitialize()
{
	Super::Deinitialize();
	OnInitialize.RemoveAll(this);
	OnInitializeComplete.Unbind();
}

//--------------------------------
// VOICEVOX CORE Initialize関連
//--------------------------------

/**
 * @brief VOICEVOX CORE 初期化
 */
void UVoicevoxCoreSubsystem::Initialize(const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
{
	InitializeCoreCompleteNum = 0;
	MetaList.Empty();
	OnInitialize.Broadcast(bUseGPU, CPUNumThreads, bLoadAllModels);
}

/**
 * @brief VOICEVOX CORE初期化実行のデリゲート関数登録
 */
void UVoicevoxCoreSubsystem::SetOnInitializeDelegate(const FVoicevoxCoreInitializeDelegate& OpenDelegate)
{
	OnInitialize.Add(OpenDelegate);
	++LoadCoreNum;
}

/**
 * @brief 音声合成するための初期化処理の結果をセット
 */
void UVoicevoxCoreSubsystem::SetInitializeResult(const bool bIsSuccess)
{
	if (!bIsSuccess)
	{
		if (!OnInitializeComplete.ExecuteIfBound(false))
		{
			UE_LOG(LogTemp, Error, TEXT("OnInitializeComplete Not Execute!!"));
		}

		return;
	}

	++InitializeCoreCompleteNum;
	if (LoadCoreNum == InitializeCoreCompleteNum)
	{
		if (!OnInitializeComplete.ExecuteIfBound(true))
		{
			UE_LOG(LogTemp, Error, TEXT("OnInitializeComplete Not Execute!!"));
		}
	}
	
}

/**
 * @brief 全てのVOICEVOX CORE 初期化が完了しているか
 */
bool UVoicevoxCoreSubsystem::GetIsInitialize() const
{
	return LoadCoreNum == InitializeCoreCompleteNum;	
}

/**
 * @brief VOICEVOX CORE初期化完了のデリゲート関数登録
 */
void UVoicevoxCoreSubsystem::SetOnInitializeCompleteDelegate(const FVoicevoxCoreCompleteDelegate& OpenDelegate)
{
	if (OnInitializeComplete.IsBound()) OnInitializeComplete.Unbind();
	OnInitializeComplete = OpenDelegate;
}

//--------------------------------
// VOICEVOX CORE Finalize関連
//--------------------------------

/**
 * @brief VOICEVOX CORE 終了処理
 */
void UVoicevoxCoreSubsystem::Finalize() const
{
	OnFinalize.Broadcast();
}

/**
 * @brief VOICEVOX CORE終了処理実行のデリゲート関数登録
 */
void UVoicevoxCoreSubsystem::SetOnFinalizeDelegate(const TVoicevoxCoreDelegate<void()>& OpenDelegate)
{
	OnFinalize.Add(OpenDelegate);
}
/**
 * @brief VOICEVOX CORE終了処理完了のデリゲート関数登録
 */
void UVoicevoxCoreSubsystem::SetOnFinalizeCompleteDelegate(const FVoicevoxCoreCompleteDelegate& OpenDelegate)
{
	if (OnFinalizeComplete.IsBound()) OnFinalizeComplete.Unbind();
	OnFinalizeComplete = OpenDelegate;
}

/**
 * @brief VOICEVOX CORE終了処理の結果をセット
 */
void UVoicevoxCoreSubsystem::SetFinalizeResult(const bool bIsSuccess)
{
	if (!bIsSuccess)
	{
		if (!OnFinalizeComplete.ExecuteIfBound(false))
		{
			UE_LOG(LogTemp, Display, TEXT("OnFinalizeComplete Not Execute!!"));
		}

		return;
	}

	--InitializeCoreCompleteNum;
	if (InitializeCoreCompleteNum == 0)
	{
		MetaList.Empty();
		if (!OnFinalizeComplete.ExecuteIfBound(true))
		{
			UE_LOG(LogTemp, Display, TEXT("OnFinalizeComplete Not Execute!!"));
		}
	}
}

/**
 * @brief 話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> UVoicevoxCoreSubsystem::GetMetaList()
{
	return MetaList;
}

/**
 * @brief 話者名や話者IDのリストを追加する
 */
void UVoicevoxCoreSubsystem::AddMetaList(TArray<FVoicevoxMeta> List) 
{
	for (auto Element : List)
	{
		MetaList.Emplace(Element);
	}
}