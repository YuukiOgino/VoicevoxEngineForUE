// Copyright Yuuki Ogino. All Rights Reserved.


#include "Subsystems/VoicevoxCoreSubsystem.h"

#include "VoicevoxNativeObject.h"

//--------------------------------
// override
//--------------------------------

/**
 * @brief Initialize
 */
void UVoicevoxCoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UClass* NativeClass = UVoicevoxNativeObject::StaticClass();
	NativeInstance = NewObject<UVoicevoxNativeObject>(this, NativeClass);
}

/**
 * @brief Deinitialize
 */
void UVoicevoxCoreSubsystem::Deinitialize()
{
	Super::Deinitialize();
	OnInitializeComplete.Unbind();

	NativeInstance->Shutdown();
}

void UVoicevoxCoreSubsystem::NativeInitialize() const
{
	NativeInstance->Init();
}

//--------------------------------
// VOICEVOX CORE Initialize関連
//--------------------------------

/**
 * @brief VOICEVOX CORE 初期化
 */
void UVoicevoxCoreSubsystem::Initialize(const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
{
	MetaList.Empty();
	SupportedDevicesMap.Empty();
	VoicevoxCoreVersionMap.Empty();
	CoreNameList.Empty();
	
	NativeInstance->CoreInitialize(bUseGPU, CPUNumThreads, bLoadAllModels);
}

/**
 * @brief 音声合成するための初期化処理の結果をセット
 */
void UVoicevoxCoreSubsystem::SetInitializeResult(const bool bIsSuccess)
{
	bIsInitialized = bIsSuccess;
	
	if (!bIsSuccess)
	{
		if (!OnInitializeComplete.ExecuteIfBound(false))
		{
			UE_LOG(LogTemp, Error, TEXT("OnInitializeComplete Not Execute!!"));
		}

		return;
	}

	if (!OnInitializeComplete.ExecuteIfBound(true))
	{
		UE_LOG(LogTemp, Error, TEXT("OnInitializeComplete Not Execute!!"));
	}
	
}

/**
 * @brief 全てのVOICEVOX CORE 初期化が完了しているか
 */
bool UVoicevoxCoreSubsystem::GetIsInitialize() const
{
	return bIsInitialized;	
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
	NativeInstance->Finalize();
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

	MetaList.Empty();
	SupportedDevicesMap.Empty();
	VoicevoxCoreVersionMap.Empty();
	CoreNameList.Empty();
	
	if (!OnFinalizeComplete.ExecuteIfBound(true))
	{
		UE_LOG(LogTemp, Display, TEXT("OnFinalizeComplete Not Execute!!"));
	}
}

/**
 * @fn
 * VOICEVOX COREのモデルをロード実行
 * @brief モデルをロードする。
 * @param SpeakerId 話者番号
 * @detail
 * 必ずしも話者とモデルが1:1対応しているわけではない。
 *
 * ※モデルによってはメインスレッドが暫く止まるほど重いので、その場合は非同期で処理してください。（UE::Tasks::Launch等）
 */
void UVoicevoxCoreSubsystem::LoadModel(const int64 SpeakerId) const
{
	NativeInstance->LoadModel(SpeakerId);
}

/**
 * @brief VOICEVOX COREモデル読み込み処理完了のデリゲート関数登録
 */
void UVoicevoxCoreSubsystem::SetOnLoadModelCompleteDelegate(const FVoicevoxCoreCompleteDelegate& OpenDelegate)
{
	if (OnLodeModelComplete.IsBound()) OnLodeModelComplete.Unbind();
	OnLodeModelComplete = OpenDelegate;
}

/**
 * @brief VOICEVOX COREモデル読み込みの結果をセット
 * @param[in] bIsSuccess		モデル読み込み処理が成功したか
 * @detail
 * VOICEVOXのモデル読み込み処理のリザルトをセットする。NativeCoreプラグインで使用する。
 */
void UVoicevoxCoreSubsystem::SetLodeModelResult(const bool bIsSuccess) const
{
	if (!bIsSuccess)
	{
		if (!OnLodeModelComplete.ExecuteIfBound(false))
		{
			UE_LOG(LogTemp, Display, TEXT("OnModelLodeComplete Not Execute!!"));
		}

		return;
	}

	if (!OnLodeModelComplete.ExecuteIfBound(true))
	{
		UE_LOG(LogTemp, Display, TEXT("OnModelLodeComplete Not Execute!!"));
	}
}

/**
 * @fn
 * VOICEVOX COREのvoicevox_audio_queryを取得
 * @brief AudioQuery を取得する。
 * @param[in] SpeakerId 話者番号
 * @param[in] Message 音声データに変換するtextデータ
 * @param[in] bKana aquestalk形式のkanaとしてテキストを解釈する
 * @return AudioQueryをjsonでフォーマット後、構造体へ変換したもの。
 * @details
 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
 */
FVoicevoxAudioQuery UVoicevoxCoreSubsystem::GetAudioQuery(int64 SpeakerId, const FString& Message, bool bKana) const
{
	return NativeInstance->GetAudioQuery(SpeakerId, Message, bKana);
}

/**
 * @brief VOICEVOX COREのtext to speechを実行
 */
TArray<uint8> UVoicevoxCoreSubsystem::RunTextToSpeech(const int64 SpeakerId, const FString& Message, const bool bKana, const bool bEnableInterrogativeUpspeak) const
{
	return NativeInstance->RunTextToSpeech(SpeakerId, Message, bKana, bEnableInterrogativeUpspeak);
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UVoicevoxCoreSubsystem::RunSynthesis(const char* AudioQueryJson, const int64 SpeakerId, bool bEnableInterrogativeUpspeak) const
{
	return NativeInstance->RunSynthesis(AudioQueryJson, SpeakerId,  bEnableInterrogativeUpspeak);
}

/**
 * @brief AudioQueryを音声データに変換する。
 */
TArray<uint8> UVoicevoxCoreSubsystem::RunSynthesis(const FVoicevoxAudioQuery& AudioQueryJson, const int64 SpeakerId, const bool bEnableInterrogativeUpspeak) const
{
	return NativeInstance->RunSynthesis(AudioQueryJson, SpeakerId,  bEnableInterrogativeUpspeak);
}

/**
 * @brief 話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> UVoicevoxCoreSubsystem::GetMetaList()
{
	return MetaList;
}

/**
 * @brief 指定したSpeakerIDの名前を取得する
 */
FString UVoicevoxCoreSubsystem::GetMetaName(const int64 SpeakerID)
{
	TArray<FVoicevoxMeta> List = GetMetaList();
	for  (auto [Name, Styles, Speaker_uuid, Version] : List)
	{
		for (const auto Style :Styles)
		{
			if (Style.Id == SpeakerID)
			{
				return FString::Printf(TEXT("%s(%s)"), *Name, *Style.Name);
			}
		}
	}
	
	return TEXT("");
}

/**
 * @brief VOICEVOX COREのバージョンを取得する
 * @return SemVerでフォーマットされたバージョン
 */
FString UVoicevoxCoreSubsystem::GetVoicevoxVersion(const FString& CoreName)
{
	return VoicevoxCoreVersionMap[CoreName];
}

/**
 * @brief ハードウェアアクセラレーションがGPUモードか判定する
 * @return GPUモードならtrue、そうでないならfalse
 */
bool UVoicevoxCoreSubsystem::IsGpuMode(const FString& CoreName)
{
	return IsGpuModeMap[CoreName];
}
	
/**
 * @brief サポートデバイス情報を取得する
 * @return サポートデバイス情報の構造体
 */
FVoicevoxSupportedDevices UVoicevoxCoreSubsystem::GetSupportedDevices(const FString& CoreName)
{
	return SupportedDevicesMap[CoreName];
}

/**
 * @brief 各VOICEVOX COREの話者名や話者IDのリスト、サポートデバイス、バージョン情報を各変数へ追加
 */
void UVoicevoxCoreSubsystem::AddVoicevoxConfigData(const FString& CoreName, TArray<FVoicevoxMeta> List, FVoicevoxSupportedDevices SupportedDevices, const FString& Version, const bool bIsGpuMode)
{
	for (auto Element : List)
	{
		MetaList.Emplace(Element);
	}
	CoreNameList.Add(CoreName);
	SupportedDevicesMap.Add(CoreName, SupportedDevices);
	VoicevoxCoreVersionMap.Add(CoreName, Version);
	IsGpuModeMap.Add(CoreName, bIsGpuMode);
}

/**
 * @brief 初期化済みのネイティブコア名を取得
 * @return 初期化済みのネイティブコア名のリスト
 */
TArray<FString> UVoicevoxCoreSubsystem::GetCoreNameList()
{
	return CoreNameList;
}