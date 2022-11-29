// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @brief  VOICEVOX COREのAPIを呼び出すUtilクラスをまとめたCPPファイル
 * @author Yuuki Ogino
 */

#include "VoicevoxCoreUtil.h"

#include "JsonObjectConverter.h"
#include "VoicevoxCore/core.h"

DEFINE_LOG_CATEGORY(LogVoicevoxEngine);

/**
 * @brief VOICEVOX COER 初期化
 */
bool FVoicevoxCoreUtil::Initialize(const bool bUseGPU, const int CPUNumThreads, const bool bLoadAllModels)
{
#if PLATFORM_WINDOWS
	const FString PlatformFolderName = TEXT("Win64");
#else
	const FString PlatformFolderName = "";
#endif

	if (PlatformFolderName.IsEmpty())
	{
		FString ErrorMessage = TEXT("VOICEVOX Initialize Error:Not covered Platform");
		ShowVoicevoxErrorMessage(ErrorMessage);
		return false;
	}

	if (!initialize(bUseGPU, CPUNumThreads, bLoadAllModels))
	{
		const FString LastMessage = UTF8_TO_TCHAR(last_error_message());
		FString ErrorMessage = FString::Printf(TEXT("VOICEVOX Initialize Error:%s"), *LastMessage);
		ShowVoicevoxErrorMessage(ErrorMessage);
		return false;
	}

	const FString JtalkPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), TEXT("Binaries"), PlatformFolderName, OPEN_JTALK_DIC_NAME));
	const VoicevoxResultCode Result = voicevox_load_openjtalk_dict(TCHAR_TO_UTF8(*JtalkPath));
	if (Result !=  VOICEVOX_RESULT_SUCCEED)
	{
		const FString ResultMessage = UTF8_TO_TCHAR(voicevox_error_result_to_message(Result));
		FString ErrorMessage = FString::Printf(TEXT("VOICEVOX Initialize Error:%s"), *ResultMessage);
		ShowVoicevoxErrorMessage(ErrorMessage);
	}

	bIsInit = Result == VOICEVOX_RESULT_SUCCEED;
	return Result == VOICEVOX_RESULT_SUCCEED;
}

/**
 * @brief VOICEVOX COER 終了処理
 */
void FVoicevoxCoreUtil::Finalize()
{
	finalize();
	bIsInit = false;
}

/**
 * @brief  VOICEVOX COREのモデルをロード実行
 */
bool FVoicevoxCoreUtil::LoadModel(const int64 SpeakerId)
{
	// 重い処理のため、スピーカーモデルがロードされていない場合のみロードを実行する
	if (!is_model_loaded(SpeakerId))
	{
		return load_model(SpeakerId);
	}
	
	return true;
}

/**
 * @brief VOICEVOX COERのtext to speechを実行
 */
uint8* FVoicevoxCoreUtil::RunTextToSpeech(const int64 SpeakerId, const FString& Message, int& OutputBinarySize)
{
	// スピーカーモデルがロードされていない場合はロードを実行する
	if (LoadModel(SpeakerId))
	{
		uint8* OutputWAV = nullptr;
		if (const VoicevoxResultCode Result = voicevox_tts(TCHAR_TO_UTF8(*Message), SpeakerId, &OutputBinarySize, &OutputWAV);
			Result != VOICEVOX_RESULT_SUCCEED)
		{
			const FString ResultMessage = UTF8_TO_TCHAR(voicevox_error_result_to_message(Result));
			FString ErrorMessage = FString::Printf(TEXT("VOICEVOX TTS Error:%s"), *ResultMessage);
			ShowVoicevoxErrorMessage(ErrorMessage);
			return nullptr;	
		}

		return OutputWAV;
	}

	return nullptr;
}

/**
 * @brief VOICEVOX COERのtext to speech（AquesTalkライクな記法版）を実行
 */
uint8* FVoicevoxCoreUtil::RunTextToSpeechFromKana(const int64 SpeakerId, const FString& Message, int& OutputBinarySize)
{
	// スピーカーモデルがロードされていない場合はロードを実行する
	if (LoadModel(SpeakerId))
	{
		uint8* OutputWAV = nullptr;
		if (const VoicevoxResultCode Result = voicevox_tts_from_kana(TCHAR_TO_UTF8(*Message), SpeakerId, &OutputBinarySize, &OutputWAV);
			Result != VOICEVOX_RESULT_SUCCEED)
		{
			const FString ResultMessage = UTF8_TO_TCHAR(voicevox_error_result_to_message(Result));
			FString ErrorMessage = FString::Printf(TEXT("VOICEVOX TTS(Kana) Error:%s"), *ResultMessage);
			ShowVoicevoxErrorMessage(ErrorMessage);
			return nullptr;	
		}

		return OutputWAV;
	}
	
	return nullptr;
}

/**
 * @brief VOICEVOX COERのvoicevox_ttsで生成した音声データを開放
 */
void FVoicevoxCoreUtil::WavFree(uint8* Wav)
{
	voicevox_wav_free(Wav);
}

/**
 * @brief 話者名や話者IDのリストを取得する
 */
FString FVoicevoxCoreUtil::Metas()
{
	if (!bIsInit) return "";
	return UTF8_TO_TCHAR(metas());
}

/**
 * @brief 話者名や話者IDのリストを取得する
 */
TArray<FVoicevoxMeta> FVoicevoxCoreUtil::MetaList()
{
	TArray<FVoicevoxMeta> List;
	// 初期化が行われていない場合はJSON変換時にクラッシュするため、Empty状態で返却する
	if (!bIsInit) return List;
	FJsonObjectConverter::JsonArrayStringToUStruct(Metas(), &List, 0, 0);
	return List;
}

/** 
 * @brief 音素列から、音素ごとの長さを求める
 */
TArray<float> FVoicevoxCoreUtil::GetPhonemeLength(const int64 Length, TArray<int64> PhonemeList, int64 SpeakerID)
{
	TArray<float> Output;
	Output.Init(0, Length);
	yukarin_s_forward(Length, PhonemeList.GetData(), &SpeakerID, Output.GetData());
	return Output;
}

/**
 * @brief モーラごとの音素列とアクセント情報から、モーラごとの音高を求める
 */
TArray<float> FVoicevoxCoreUtil::FindPitchEachMora(const int64 Length, TArray<int64> VowelPhonemeList, TArray<int64> ConsonantPhonemeList,
                                                   TArray<int64> StartAccentList, TArray<int64> EndAccentList,
                                                   TArray<int64> StartAccentPhraseList, TArray<int64> EndAccentPhraseList,
                                                   int64 SpeakerID)
{
	TArray<float> Output;
	Output.Init(0, Length);

	if (const bool bResult = yukarin_sa_forward(Length, VowelPhonemeList.GetData(), ConsonantPhonemeList.GetData(),
	                                            StartAccentList.GetData(), EndAccentList.GetData(), StartAccentPhraseList.GetData(),
	                                            EndAccentPhraseList.GetData(), &SpeakerID, Output.GetData()); !bResult)
	{
		const FString LastMessage = UTF8_TO_TCHAR(last_error_message());
		FString Message = FString::Printf(TEXT("VOICEVOX yukarin_sa_forward Error:%s"), *LastMessage);
		ShowVoicevoxErrorMessage(Message);
	}
	
	return Output;
}

/**
 * @brief フレームごとの音素と音高から、波形を求める
 */
TArray<float> FVoicevoxCoreUtil::DecodeForward(int64 Length, int64 PhonemeSize, TArray<float> F0, TArray<float> Phoneme,
									  int64 SpeakerID)
{
	TArray<float> Output;
	Output.Init(0, Length);
	if (const bool bResult = decode_forward(Length, PhonemeSize, F0.GetData(), Phoneme.GetData(), &SpeakerID, Output.GetData()); !bResult)
	{
		const FString LastMessage = UTF8_TO_TCHAR(last_error_message());
		FString Message = FString::Printf(TEXT("VOICEVOX decode_forward Error:%s"), *LastMessage);
		ShowVoicevoxErrorMessage(Message);
	}
	return Output;
}

/**
 * @brief VOICEVOXから受信したエラーメッセージを表示
 */
void FVoicevoxCoreUtil::ShowVoicevoxErrorMessage(FString& MessageFormat)
{
	UE_LOG(LogVoicevoxEngine, Error,  TEXT("%s"), *MessageFormat);
	const FColor Col = FColor::Red;
	const FVector2D Scl = FVector2D(1.0f, 1.0f);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, Col, *MessageFormat, true, Scl);
}