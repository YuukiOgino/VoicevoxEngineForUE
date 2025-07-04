// Copyright Yuuki Ogino. All Rights Reserved.

// リップシンクの実装にあたり、以下のソースコードを元にUE版で実装しています。
// https://github.com/TORISOUP/VoicevoxClientSharp/blob/master/VoicevoxClientSharp.Unity/Assets/VoicevoxClientSharp.Unity/Runtime/VRM/VoicevoxVrmLipSyncPlayer.cs
// Copyright © 2024 TORISOUP 
// MITライセンス全文
// https://opensource.org/licenses/mit-license.php

/**
 * @brief  VOICEVOXのAudioQueryを解析して音再生とリップシンクを行う抽象コンポーネントのCPPファイル
 * @author Yuuki Ogino
 */
#include "Components/AbstractLipSyncAudioComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "Subsystems/VoicevoxCoreSubsystem.h"

DEFINE_LOG_CATEGORY(LogVoicevoxLipSync);

/**
 * @brief コンストラクタ
 */
UAbstractLipSyncAudioComponent::UAbstractLipSyncAudioComponent(): AudioQuery(), NowLipSync()
{
	PrimaryComponentTick.bCanEverTick = true;

	LipSyncMorphNameMap.Reserve(6);
	LipSyncMorphNameMap.Add(ELipSyncVowelType::A, NAME_None);
	LipSyncMorphNameMap.Add(ELipSyncVowelType::I, NAME_None);
	LipSyncMorphNameMap.Add(ELipSyncVowelType::U, NAME_None);
	LipSyncMorphNameMap.Add(ELipSyncVowelType::E, NAME_None);
	LipSyncMorphNameMap.Add(ELipSyncVowelType::O, NAME_None);
	LipSyncMorphNameMap.Add(ELipSyncVowelType::Simple, NAME_None);

	LipSyncMorphNumMap.Reserve(6);
	LipSyncMorphNumMap.Add(ELipSyncVowelType::A, 0.0f);
	LipSyncMorphNumMap.Add(ELipSyncVowelType::I, 0.0f);
	LipSyncMorphNumMap.Add(ELipSyncVowelType::U, 0.0f);
	LipSyncMorphNumMap.Add(ELipSyncVowelType::E, 0.0f);
	LipSyncMorphNumMap.Add(ELipSyncVowelType::O, 0.0f);
	LipSyncMorphNumMap.Add(ELipSyncVowelType::Simple, 0.0f);
}

/**
 * @brief BeginPlay
 */
void UAbstractLipSyncAudioComponent::BeginPlay()
{
	Super::BeginPlay();
	OnAudioPlaybackPercentNative.AddUObject(this, &UAbstractLipSyncAudioComponent::HandlePlaybackPercent);
}

/**
 * @brief EndPlay 
 */
void UAbstractLipSyncAudioComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (bIsExecTts)
	{
		bIsExecTts = false;
		TtsTask.Wait();
		Sound = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

/**
 * @brief TickComponent
 */
void UAbstractLipSyncAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsExecTts)
	{
		if (TtsTask.IsValid() && TtsTask.IsCompleted())
		{
			if (Sound != nullptr)
			{
				Play(0.0f);
			}
			
			bIsExecTts = false;
		}
	}
}

/**
 * @brief OnAudioPlaybackPercentのコールバック
 */
void UAbstractLipSyncAudioComponent::HandlePlaybackPercent(const UAudioComponent* InComponent, const USoundWave* InSoundWave, const float InPlaybackPercentage)
{
	// ループ無しかつ最後まで再生しても止まらない場合があるので、明確にストップする
	if (Sound != nullptr && !Sound->IsLooping() && InPlaybackPercentage >= 1.0f)
	{
		Stop();
		InitMorphNumMap();
		if (bIsPlayLipSyncSimple)
		{
			TMap<ELipSyncVowelType, float> Map;
			Map.Reserve(1);
			Map.Add(ELipSyncVowelType::Simple, LipSyncMorphNumMap[ELipSyncVowelType::Simple]);
			NotificationMorphNum(Map);
		}
		else
		{
			TMap<ELipSyncVowelType, float> Map;
			Map.Reserve(5);
			Map.Add(ELipSyncVowelType::A, LipSyncMorphNumMap[ELipSyncVowelType::A]);
			Map.Add(ELipSyncVowelType::I, LipSyncMorphNumMap[ELipSyncVowelType::I]);
			Map.Add(ELipSyncVowelType::U, LipSyncMorphNumMap[ELipSyncVowelType::U]);
			Map.Add(ELipSyncVowelType::E, LipSyncMorphNumMap[ELipSyncVowelType::E]);
			Map.Add(ELipSyncVowelType::O, LipSyncMorphNumMap[ELipSyncVowelType::O]);
			NotificationMorphNum(Map);
		}
		return;
	}

	if (!bEnabledLipSync)
	{
		InitMorphNumMap();
		if (bIsPlayLipSyncSimple)
		{
			TMap<ELipSyncVowelType, float> Map;
			Map.Reserve(1);
			Map.Add(ELipSyncVowelType::Simple, LipSyncMorphNumMap[ELipSyncVowelType::Simple]);
			NotificationMorphNum(Map);
		}
		else
		{
			TMap<ELipSyncVowelType, float> Map;
			Map.Reserve(5);
			Map.Add(ELipSyncVowelType::A, LipSyncMorphNumMap[ELipSyncVowelType::A]);
			Map.Add(ELipSyncVowelType::I, LipSyncMorphNumMap[ELipSyncVowelType::I]);
			Map.Add(ELipSyncVowelType::U, LipSyncMorphNumMap[ELipSyncVowelType::U]);
			Map.Add(ELipSyncVowelType::E, LipSyncMorphNumMap[ELipSyncVowelType::E]);
			Map.Add(ELipSyncVowelType::O, LipSyncMorphNumMap[ELipSyncVowelType::O]);
			NotificationMorphNum(Map);
		}
		
		if (Sound != nullptr && LipSyncTime < Sound->Duration * InPlaybackPercentage)
		{
			NowLipSync = LipSyncList.Pop();
			LipSyncTime += NowLipSync.Length;
		}
		return;
	}
	if (LipSyncList.IsEmpty()) return;
	if (Sound == nullptr)
	{
		InitMorphNumMap();
		if (bIsPlayLipSyncSimple)
		{
			TMap<ELipSyncVowelType, float> Map;
			Map.Reserve(1);
			Map.Add(ELipSyncVowelType::Simple, LipSyncMorphNumMap[ELipSyncVowelType::Simple]);
			NotificationMorphNum(Map);
		}
		else
		{
			TMap<ELipSyncVowelType, float> Map;
			Map.Reserve(5);
			Map.Add(ELipSyncVowelType::A, LipSyncMorphNumMap[ELipSyncVowelType::A]);
			Map.Add(ELipSyncVowelType::I, LipSyncMorphNumMap[ELipSyncVowelType::I]);
			Map.Add(ELipSyncVowelType::U, LipSyncMorphNumMap[ELipSyncVowelType::U]);
			Map.Add(ELipSyncVowelType::E, LipSyncMorphNumMap[ELipSyncVowelType::E]);
			Map.Add(ELipSyncVowelType::O, LipSyncMorphNumMap[ELipSyncVowelType::O]);
			NotificationMorphNum(Map);
		}
		return;
	}
	
	const float NowDuration = Sound->Duration * InPlaybackPercentage;
	if (LipSyncTime < NowDuration)
	{
		// 前回のリップシンク情報を元に初期化
		if (NowLipSync.IsConsonant && !bIsPlayLipSyncSimple)
		{
			if (NowLipSync.IsLabialOrPlosive)
			{
				TMap<ELipSyncVowelType, float> Map;
				Map.Reserve(5);
				Map.Add(ELipSyncVowelType::A, LipSyncMorphNumMap[ELipSyncVowelType::A]);
				Map.Add(ELipSyncVowelType::I, LipSyncMorphNumMap[ELipSyncVowelType::I]);
				Map.Add(ELipSyncVowelType::U, LipSyncMorphNumMap[ELipSyncVowelType::U]);
				Map.Add(ELipSyncVowelType::E, LipSyncMorphNumMap[ELipSyncVowelType::E]);
				Map.Add(ELipSyncVowelType::O, LipSyncMorphNumMap[ELipSyncVowelType::O]);
				NotificationMorphNum(Map);
			}
		}
		else
		{
			if (!bIsPlayLipSyncSimple)
			{
				// 母音の初期化
				// 前回のリップシンク情報から最大値を更新
				switch (NowLipSync.VowelType)
				{
				case ELipSyncVowelType::A:
				case ELipSyncVowelType::I:
				case ELipSyncVowelType::U:
				case ELipSyncVowelType::E:
				case ELipSyncVowelType::O:
					if (NowLipSync.VowelType != LipSyncList.Last().VowelType && LipSyncList.Last().VowelType != ELipSyncVowelType::Non)
					{
						InitMorphNumMap();
						LipSyncMorphNumMap[NowLipSync.VowelType] = MaxMouthScale * 0.8f;
					}
					else
					{
						InitMorphNumMap();
						LipSyncMorphNumMap[NowLipSync.VowelType] = MaxMouthScale;
					}
					break;
				case ELipSyncVowelType::CL:
					LipSyncMorphNumMap[ELipSyncVowelType::A] = LipSyncMorphNumMap[ELipSyncVowelType::A] * 0.8f * 0.8f;
					LipSyncMorphNumMap[ELipSyncVowelType::I] = LipSyncMorphNumMap[ELipSyncVowelType::I] * 0.8f * 0.8f;
					LipSyncMorphNumMap[ELipSyncVowelType::U] = LipSyncMorphNumMap[ELipSyncVowelType::U] * 0.8f * 0.8f;
					LipSyncMorphNumMap[ELipSyncVowelType::E] = LipSyncMorphNumMap[ELipSyncVowelType::E] * 0.8f * 0.8f;
					LipSyncMorphNumMap[ELipSyncVowelType::O] = LipSyncMorphNumMap[ELipSyncVowelType::O] * 0.8f * 0.8f;
					break;
				default:
					InitMorphNumMap();
					break;
				}

				TMap<ELipSyncVowelType, float> Map;
				Map.Reserve(5);
				Map.Add(ELipSyncVowelType::A, LipSyncMorphNumMap[ELipSyncVowelType::A]);
				Map.Add(ELipSyncVowelType::I, LipSyncMorphNumMap[ELipSyncVowelType::I]);
				Map.Add(ELipSyncVowelType::U, LipSyncMorphNumMap[ELipSyncVowelType::U]);
				Map.Add(ELipSyncVowelType::E, LipSyncMorphNumMap[ELipSyncVowelType::E]);
				Map.Add(ELipSyncVowelType::O, LipSyncMorphNumMap[ELipSyncVowelType::O]);
				NotificationMorphNum(Map);
			}
			else
			{
				InitMorphNumMap();
				TMap<ELipSyncVowelType, float> Map;
				Map.Reserve(1);
				Map.Add(ELipSyncVowelType::Simple, LipSyncMorphNumMap[ELipSyncVowelType::Simple]);
				NotificationMorphNum(Map);
			}
		}
		
		NowLipSync = LipSyncList.Pop();
		LipSyncTime += NowLipSync.Length;
	}
	
	const float NowLength = (LipSyncTime - NowDuration) / NowLipSync.Length;
	if (NowLipSync.IsConsonant)
	{
		if (NowLipSync.IsLabialOrPlosive)
		{
			NotificationMorphNum(UpdateConsonantMorphNum(NowLength));
		}
	}
	else if (NowLipSync.VowelType == ELipSyncVowelType::Non)
	{
		NotificationMorphNum(UpdatePauseMorphNum(NowLength));
	}
	else
	{
		NotificationMorphNum(UpdateVowelMorphNum(NowLength));
	}
}

/**
 * @brief モーフターゲット値を初期化
 */
void UAbstractLipSyncAudioComponent::InitMorphNumMap() 
{
	LipSyncMorphNumMap[ELipSyncVowelType::A] = 0.0f;
	LipSyncMorphNumMap[ELipSyncVowelType::I] = 0.0f;
	LipSyncMorphNumMap[ELipSyncVowelType::U] = 0.0f;
	LipSyncMorphNumMap[ELipSyncVowelType::E] = 0.0f;
	LipSyncMorphNumMap[ELipSyncVowelType::O] = 0.0f;
	LipSyncMorphNumMap[ELipSyncVowelType::Simple] = 0.0f;
}

/**
 * @brief 母音のモーフターゲット値リストを更新
 */
TMap<ELipSyncVowelType, float> UAbstractLipSyncAudioComponent::UpdateVowelMorphNum(const float Alpha)
{
	const float Rate = LipSyncSpeed * Alpha;
	const float A = FMath::Clamp(Rate, 0.0f, 1.0f);
	TMap<ELipSyncVowelType, float> Map;
	if (bIsPlayLipSyncSimple)
	{
		Map.Reserve(1);
		Map.Add(ELipSyncVowelType::Simple, 0.0f);
		float Update = 0.0f;
		switch (NowLipSync.VowelType)
		{
		case ELipSyncVowelType::A:
			Update = MaxMouthScale;
			break;
		case ELipSyncVowelType::I:
			Update = MaxMouthScale * 0.25f;;
			break;
		case ELipSyncVowelType::U:
			Update = MaxMouthScale * 0.5f;;
			break;
		case ELipSyncVowelType::E:
			Update = MaxMouthScale * 0.45f;
			break;
		case ELipSyncVowelType::O:
			Update = MaxMouthScale * 0.8f;
			break;
		case ELipSyncVowelType::CL:
			Update = MaxMouthScale * 0.15f;
			break;
		default:
			break;
		}

		Map[ELipSyncVowelType::Simple] = FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::Simple], Update, A);
	}
	else
	{
		Map.Reserve(5);
		Map.Add(ELipSyncVowelType::A, 0.0f);
		Map.Add(ELipSyncVowelType::I, 0.0f);
		Map.Add(ELipSyncVowelType::U, 0.0f);
		Map.Add(ELipSyncVowelType::E, 0.0f);
		Map.Add(ELipSyncVowelType::O, 0.0f);

		float UpdateA = 0.0f;
		float UpdateI = 0.0f;
		float UpdateU = 0.0f;
		float UpdateE = 0.0f;
		float UpdateO = 0.0f;

		switch (NowLipSync.VowelType)
		{
		case ELipSyncVowelType::A:
			UpdateA = MaxMouthScale;
			break;
		case ELipSyncVowelType::I:
			UpdateI = MaxMouthScale;
			break;
		case ELipSyncVowelType::U:
			UpdateU = MaxMouthScale;
			break;
		case ELipSyncVowelType::E:
			UpdateE = MaxMouthScale;
			break;
		case ELipSyncVowelType::O:
			UpdateO = MaxMouthScale;
			break;
		case ELipSyncVowelType::CL:
			UpdateA = LipSyncMorphNumMap[ELipSyncVowelType::A];
			UpdateI = LipSyncMorphNumMap[ELipSyncVowelType::I];
			UpdateU = LipSyncMorphNumMap[ELipSyncVowelType::U];
			UpdateE = LipSyncMorphNumMap[ELipSyncVowelType::E];
			UpdateO = LipSyncMorphNumMap[ELipSyncVowelType::O];
			break;
		default:
			break;
		}
	
		Map[ELipSyncVowelType::A] = FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::A], UpdateA, A);
		Map[ELipSyncVowelType::I] = FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::I], UpdateI, A);
		Map[ELipSyncVowelType::U] = FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::U], UpdateU, A);
		Map[ELipSyncVowelType::E] = FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::E], UpdateE, A);
		Map[ELipSyncVowelType::O] = FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::O], UpdateO, A);
	}
	
	return Map;
}

/**
 * @brief 子音のモーフターゲット値リストを更新
 */
TMap<ELipSyncVowelType, float> UAbstractLipSyncAudioComponent::UpdateConsonantMorphNum(const float Alpha)
{
	const float Rate = LipSyncSpeed * Alpha;
	const float A = FMath::Clamp(Rate, 0.0f, 1.0f);
	TMap<ELipSyncVowelType, float> Map;
	if (bIsPlayLipSyncSimple)
	{
		Map.Reserve(1);
		Map.Add(ELipSyncVowelType::Simple, FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::Simple], 0.0f, A));
	}
	else
	{
		Map.Reserve(5);
		Map.Add(ELipSyncVowelType::A, FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::A], 0.0f, A));
		Map.Add(ELipSyncVowelType::I, FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::I], 0.0f, A));
		Map.Add(ELipSyncVowelType::U, FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::U], 0.0f, A));
		Map.Add(ELipSyncVowelType::E, FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::E], 0.0f, A));
		Map.Add(ELipSyncVowelType::O, FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::O], 0.0f, A));
	}
	return Map;
}

/**
 * @brief 無音のモーフターゲット値リストを更新
 */	
TMap<ELipSyncVowelType, float> UAbstractLipSyncAudioComponent::UpdatePauseMorphNum(const float Alpha)
{
	// 最速でデフォルトに戻すためにレートは2.0固定
	const float PauseRate = 2.0f * Alpha;
	const float A = FMath::Clamp(PauseRate, 0.0f, 1.0f);
	TMap<ELipSyncVowelType, float> Map;
	if (bIsPlayLipSyncSimple)
	{
		Map.Reserve(1);
		Map.Add(ELipSyncVowelType::Simple, FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::Simple], 0.0f, A));
	}
	else
	{
		Map.Reserve(5);
		Map.Add(ELipSyncVowelType::A, FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::A], 0.0f, A));
		Map.Add(ELipSyncVowelType::I, FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::I], 0.0f, A));
		Map.Add(ELipSyncVowelType::U, FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::U], 0.0f, A));
		Map.Add(ELipSyncVowelType::E, FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::E], 0.0f, A));
		Map.Add(ELipSyncVowelType::O, FMath::LerpStable(LipSyncMorphNumMap[ELipSyncVowelType::O], 0.0f, A));
	}
	
	return Map;
}

/**
 * @brief オーディオ再生とリップシンク再生を止める
 */
void UAbstractLipSyncAudioComponent::StopAudioAndLipSync()
{
	if (bIsExecTts)
	{
		bIsExecTts = false;
		TtsTask.Wait();
	}
	Super::Stop();
}

/**
 * @brief テキストを解析してSoundWave生成後、音再生とリップシンク再生を行います。
 */
void UAbstractLipSyncAudioComponent::PlayToText(const FString Message, const bool bRunKana, const bool bEnableInterrogativeUpspeak,
	const float SpeedScale, const float PitchScale, const float IntonationScale, const float VolumeScale, const float PrePhonemeLength, const float PostPhonemeLength)
{
	if (CheckExecTts()) return;
	
	if (Sound != nullptr)
	{
		Stop();
		SetSound(nullptr);
	}

	InitMorphNumMap();
	AudioQuery = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetAudioQuery(SpeakerId, Message, bRunKana);
	AudioQuery.Speed_scale = SpeedScale;
	AudioQuery.Pitch_scale = PitchScale;
	AudioQuery.Intonation_scale = IntonationScale;
	AudioQuery.Volume_scale = VolumeScale;
	AudioQuery.Pre_phoneme_length = PrePhonemeLength;
	AudioQuery.Post_phoneme_length = PostPhonemeLength;
	NowLipSync = {ELipSyncVowelType::Non, -1.0f, false, false};
	bIsPlayLipSyncSimple = bEnabledSimpleLipSync;
	ToSoundWave(SpeakerId, bEnableInterrogativeUpspeak);
}

/**
 * @brief VOICEVOX COREで取得したAudioQueryを元にSoundWaveを生成後、音再生とリップシンク再生を行います。
 */
void UAbstractLipSyncAudioComponent::PlayToAudioQuery(const FVoicevoxAudioQuery& Query, const bool bEnableInterrogativeUpspeak)
{
	if (CheckExecTts()) return;
	
	if (Sound != nullptr)
	{
		Stop();
		SetSound(nullptr);
	}

	InitMorphNumMap();
	AudioQuery = Query;
	NowLipSync = {ELipSyncVowelType::Non, -1.0f, false, false};
	bIsPlayLipSyncSimple = bEnabledSimpleLipSync;
	ToSoundWave(SpeakerId, bEnableInterrogativeUpspeak);
}

/**
 * @brief AudioQueryアセットからSoundWaveを生成後、音再生とリップシンク再生を行います。
 */
void UAbstractLipSyncAudioComponent::PlayToAudioQueryAsset(UVoicevoxQuery* VoicevoxQuery, const bool bEnableInterrogativeUpspeak)
{
	if (CheckExecTts()) return;
	
	if (Sound != nullptr)
	{
		Stop();
		SetSound(nullptr);
	}

	InitMorphNumMap();
	AudioQuery = VoicevoxQuery->VoicevoxAudioQuery;
	NowLipSync = {ELipSyncVowelType::Non, -1.0f, false, false};
	bIsPlayLipSyncSimple = bEnabledSimpleLipSync;
	ToSoundWave(VoicevoxQuery->SpeakerType, bEnableInterrogativeUpspeak);
}

/**
 * @brief AudioQueryをセットしてリップシンクデータを事前生成します。
 */
void UAbstractLipSyncAudioComponent::SetLipSyncDataToAudioQuery(const FVoicevoxAudioQuery& Query)
{
	if (GetPlayState() != EAudioComponentPlayState::Stopped) return;
	
	AudioQuery = Query;
	NowLipSync = {ELipSyncVowelType::Non, -1.0f, false, false};
	// LipSyncに必要なデータを生成する
	bIsPlayLipSyncSimple = bEnabledSimpleLipSync;
	LipSyncList = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetLipSyncList(AudioQuery, bIsPlayLipSyncSimple);
	Algo::Reverse(LipSyncList);
	LipSyncTime = 0.0f;
}

/**
 * @brief AudioQueryアセットをセットしてリップシンクデータを事前生成します。
 */
void UAbstractLipSyncAudioComponent::SetLipSyncDataToAudioQueryAsset(UVoicevoxQuery* VoicevoxQuery)
{
	if (GetPlayState() != EAudioComponentPlayState::Stopped) return;
	
	AudioQuery = VoicevoxQuery->VoicevoxAudioQuery;
	NowLipSync = {ELipSyncVowelType::Non, -1.0f, false, false};
	// LipSyncに必要なデータを生成する
	bIsPlayLipSyncSimple = bEnabledSimpleLipSync;
	LipSyncList = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetLipSyncList(AudioQuery, bIsPlayLipSyncSimple);
	Algo::Reverse(LipSyncList);
	LipSyncTime = 0.0f;
}

/**
 * テキストから音声変換を実行中かチェック
 */
bool UAbstractLipSyncAudioComponent::CheckExecTts() const
{
	if (bIsExecTts)
	{
		const FString Message = TEXT("合成音声生成中のため、音声再生をキャンセルしました。Delay等で少し時間を置いてから再度実行してください");
		UE_LOG(LogVoicevoxLipSync, Warning, TEXT("%s"), *Message);
		const FColor Col = FColor::Yellow;
		const FVector2D Scl = FVector2D(1.0f, 1.0f);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, Col, Message, true, Scl);
		return true;
	}

	return false;
}

/**
 * @brief AudioQueryからSoundWaveへ変換
 */
void UAbstractLipSyncAudioComponent::ToSoundWave(const int64 SpeakerType, const bool bEnableInterrogativeUpspeak)
{
	TtsTask = UE::Tasks::Launch<>(TEXT("LipSyncComponentTextToSpeechTask"), [=]
	{
		bIsExecTts = true;

		// LipSyncに必要なデータを生成する
		LipSyncList = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->GetLipSyncList(AudioQuery, bIsPlayLipSyncSimple);
		Algo::Reverse(LipSyncList);
		LipSyncTime = 0.0f;
		// USoundWaveを生成する。Launch内でPlayを実行するとクラッシュするため、Play処理はTickComponentで行う
		if (const TArray<uint8> OutputWAV = GEngine->GetEngineSubsystem<UVoicevoxCoreSubsystem>()->RunSynthesis(AudioQuery, SpeakerType, bEnableInterrogativeUpspeak);
		!OutputWAV.IsEmpty())
		{
			FString ErrorMessage = "";
			if (FWaveModInfo WaveInfo; WaveInfo.ReadWaveInfo(OutputWAV.GetData(), OutputWAV.Num(), &ErrorMessage))
			{
				USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>(USoundWaveProcedural::StaticClass());
				const int32 ChannelCount = *WaveInfo.pChannels;
				const int32 SizeOfSample = *WaveInfo.pBitsPerSample / 8;
				const int32 NumSamples = WaveInfo.SampleDataSize / SizeOfSample;
				const int32 NumFrames = NumSamples / ChannelCount;
					
				SoundWave->RawPCMDataSize = WaveInfo.SampleDataSize;
				SoundWave->QueueAudio(WaveInfo.SampleDataStart, WaveInfo.SampleDataSize);
					
				SoundWave->Duration = static_cast<float>(NumFrames) / *WaveInfo.pSamplesPerSec;
				SoundWave->SetSampleRate(*WaveInfo.pSamplesPerSec);
				SoundWave->NumChannels = ChannelCount;
				SoundWave->TotalSamples = *WaveInfo.pSamplesPerSec * SoundWave->Duration;
				SoundWave->SoundGroup = SOUNDGROUP_Default;

				SetSound(SoundWave);

				if (OnCreateSoundWave.IsBound())
				{
					OnCreateSoundWave.Broadcast();
				}

				if (OnCreateSoundWaveNative.IsBound())
				{
					OnCreateSoundWaveNative.Broadcast();
				}
			}
		}
	});
}