// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxCoreUtil.h
 * @brief  VOICEVOX COREのAPIを呼び出すUtilクラスをまとめたヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "VoicevoxCoreUtil.generated.h"

//------------------------------------------------------------------------
// enum
//------------------------------------------------------------------------

/**
 * @enum ESpeakerType
 * @brief Speakerを示す列挙体(Ver.0.13.3時点)
 */
UENUM(BlueprintType)
enum class ESpeakerType : uint8
{
	Metan=2,
	MetanAmaAma=0,
	MetanTsunTsun=6,
	MetanSexy=4,
	MetanSasayaki=36,
	MetanHisoHiso=37,
	Zundamon=3,
	ZundamonAmaAma=1,
	ZundamonTsunTsun=7,
	ZundamonSexy=5,
	ZundamonSasayaki=22,
	ZundamonHisoHiso=38,
	Tsumugi=8,
	Hau=10,
	Ritsu=9,
	Kurono=11,
	Kotarou=12,
	KotarouWai=32,
	KotarouBikuBiku=33,
	KotarouOko=34,
	KotarouBien=35,
	Ryusei=13,
	Himari=14,
	Sora=16,
	SoraAmaAma=15,
	SoraTsunTsun=18,
	SoraSexy=17,
	SoraSasayaki=19,
	MochikoSan=20,
	Kenzaki=21,
	WhiteCul=23,
	WhiteCulTanoshi=24,
	WhiteCulKanashi=25,
	WhiteCulBien=26,
	Goki=27,
	GokiNuigurumi=28,
	No7=29,
	No7Announcement=30,
	No7Yomikikase=31
};

//------------------------------------------------------------------------
// struct
//------------------------------------------------------------------------

/**
 * @struct FVoicevoxStyle
 * @brief VOICEVOXのモデルスタイル情報構造体
 */
USTRUCT(BlueprintType)
struct FVoicevoxStyle
{
	GENERATED_USTRUCT_BODY()

	// スタイル名
	UPROPERTY(BlueprintReadOnly, Category="VOICEVOX Engine")
	FString Name;

	// スピーカーID
	UPROPERTY(BlueprintReadOnly, Category="VOICEVOX Engine")
	int64 Id;
};

/**
 * @struct FVoicevoxMeta
 * @brief VOICEVOXのメタ情報構造体
 */
USTRUCT(BlueprintType)
struct FVoicevoxMeta
{
	GENERATED_USTRUCT_BODY()

	// モデル名
	UPROPERTY(BlueprintReadOnly, Category="VOICEVOX Engine")
	FString Name;

	// スタイル情報リスト
	UPROPERTY(BlueprintReadOnly, Category="VOICEVOX Engine")
	TArray<FVoicevoxStyle> Styles;

	// スピーカーユニークID
	UPROPERTY(BlueprintReadOnly, Category="VOICEVOX Engine")
	FString Speaker_uuid;

	// バージョン情報
	UPROPERTY(BlueprintReadOnly, Category="VOICEVOX Engine")
	FString Version;
};

//------------------------------------------------------------------------
// FVoicevoxCoreUtil
//------------------------------------------------------------------------

/**
 * @class FVoicevoxCoreUtil
 * @brief VOICEVOX COREのAPIを呼び出すUtilクラス
 */
class VOICEVOXENGINE_API FVoicevoxCoreUtil
{
	inline static bool bIsInit = false;

	/**
	 * @brief VOICEVOXから受信したエラーメッセージを表示
	 * @param[in] MessageFormat : エラーメッセージのフォーマット
	 */
	static void ShowVoicevoxErrorMessage(FString& MessageFormat);

public:
	/**
	 * @fn
	 * VOICEVOX COER 初期化
	 * @brief 音声合成するための初期化を行う。VOICEVOXのAPIを正しく実行するには先に初期化が必要
	 * @param[in] bUseGPU			trueならGPU用、falseならCPU用の初期化を行う
	 * @param[in] CPUNumThreads		推論に用いるスレッド数を設定する。0の場合論理コア数の半分か、物理コア数が設定される
	 * @param[in] bLoadAllModels	trueなら全てのモデルをロードする(かなり時間がかかるのでtrueは非推奨です。trueはデバッグ用として使用してください)
	 * @return 成功したらtrue、失敗したらfalse
	 * @detail
	 * VOICEVOXの初期化処理は何度も実行可能。use_gpuを変更して実行しなおすことも可能。
	 * 最後に実行したuse_gpuに従って他の関数が実行される。
	 * 初期化処理はUE::Tasks::Launchで非同期に行われる
	 *
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	static bool Initialize(bool bUseGPU, int CPUNumThreads = 0, bool bLoadAllModels = false);

	/**
	 * @fn
	 * VOICEVOX COER 終了処理
	 * @brief 終了処理を行う。以降VOICEVOXのAPIを利用するためには再度Initializeメソッドを行う必要がある。
	 * @detail
	 * VOICEVOXの終了処理は何度も実行可能。
	 * 実行せずにexitしても大抵の場合問題ないが、CUDAを利用している場合は終了処理を実行しておかないと例外が起こることがある。
	 */
	static void Finalize();

	/**
	 * @fn
	 * VOICEVOX COREのモデルをロード実行
	 * @brief モデルをロードする。
	 * @param SpeakerId 話者番号
	 * @return 成功したらtrue、失敗したらfalse
	 * @detail
	 * 必ずしも話者とモデルが1:1対応しているわけではない。
	 *
	 * ※モデルによってはメインスレッドが暫く止まるほど重いので、その場合は非同期で処理してください。（UE::Tasks::Launch等）
	 */
	static bool LoadModel(int64 SpeakerId);
	
	/**
	 * @fn
	 * VOICEVOX COERのtext to speechを実行
	 * @brief Textデータを音声データに変換する。
	 * @param[in] SpeakerId 話者番号
	 * @param[in] Message 音声データに変換するtextデータ
	 * @param[out] OutputBinarySize 音声データのサイズを出力する先のポインタ
	 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
	 * @details
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	static uint8* RunTextToSpeech(int64 SpeakerId, const FString& Message, int& OutputBinarySize);

	/**
	 * @fn
	 * VOICEVOX COERのtext to speech（AquesTalkライクな記法版）を実行
	 * @brief Textデータを音声データに変換する。MessageはAquesTalkライクな記法をする必要がある。
	 * @param[in] SpeakerId 話者番号
	 * @param[in] Message 音声データに変換するtextデータ（要AquesTalkライクな記法）
	 * @param[out] OutputBinarySize 音声データのサイズを出力する先のポインタ
	 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
	 * @details
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	static uint8* RunTextToSpeechFromKana(int64 SpeakerId, const FString& Message, int& OutputBinarySize);

	/**
	 * @fn
	 * VOICEVOX COERのvoicevox_ttsで生成した音声データを開放
	 * @brief voicevox_ttsで生成した音声データを開放する
	 * @param Wav 開放する音声データのポインタ
	 */
	static void WavFree(uint8* Wav);

	/**
	 * @fn
	 * メタ情報を取得する
	 * @brief 話者名や話者IDのリストを取得する
	 * @return メタ情報が格納されたjson形式の文字列
	 */
	static FString Metas();

	/**
	 * @fn
	 * メタ情報を取得する
	 * @brief 話者名や話者IDのリストを取得する
	 * @return メタ情報が格納されたjson形式の構造体
	 */
	static TArray<FVoicevoxMeta> MetaList();

	/**
	 * @fn
	 * 音素ごとの長さを求める
	 * @brief 音素列から、音素ごとの長さを求める
	 * @param[in] Length 音素列の長さ
	 * @param[in] PhonemeList 音素列
	 * @param[in] SpeakerID 話者番号
	 * @return 音素ごとの長さ
	 *
	 * @warning 動作確認が取れていないため、クラッシュ、もしくは予期せぬ動作をする可能性が高いです。
	 */
	static TArray<float> GetPhonemeLength(int64 Length, TArray<int64> PhonemeList, int64 SpeakerID);

	/**
	 * @fn
	 * モーラごとの音高を求める
	 * @brief モーラごとの音素列とアクセント情報から、モーラごとの音高を求める
	 * @param[in] Length モーラ列の長さ
	 * @param[in] VowelPhonemeList 母音の音素列
	 * @param[in] ConsonantPhonemeList 子音の音素列
	 * @param[in] StartAccentList アクセントの開始位置
	 * @param[in] EndAccentList アクセントの終了位置
	 * @param[in] StartAccentPhraseList アクセント句の開始位置
	 * @param[in] EndAccentPhraseList アクセント句の終了位置
	 * @param[in] SpeakerID 話者番号
	 * @return モーラごとの音高
	 *
	 * @warning 動作確認が取れていないため、クラッシュ、もしくは予期せぬ動作をする可能性が高いです。
	 */
	static TArray<float> FindPitchEachMora(int64 Length, TArray<int64> VowelPhonemeList, TArray<int64> ConsonantPhonemeList,
											  TArray<int64> StartAccentList, TArray<int64> EndAccentList,
											  TArray<int64> StartAccentPhraseList, TArray<int64> EndAccentPhraseList,
											  int64 SpeakerID);


	/**
	 * @fn
	 * 波形を求める
	 * @brief フレームごとの音素と音高から、波形を求める
	 * @param[in] Length フレームの長さ
	 * @param[in] PhonemeSize 音素の種類数
	 * @param[in] F0 フレームごとの音高
	 * @param[in] Phoneme フレームごとの音素
	 * @param[in] SpeakerID 話者番号
	 * @return 音声波形
	 *
	 * @warning 動作確認が取れていないため、クラッシュ、もしくは予期せぬ動作をする可能性が高いです。
	 */
	static TArray<float> DecodeForward(int64 Length, int64 PhonemeSize, TArray<float> F0, TArray<float> Phoneme,
									  int64 SpeakerID);
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxEngine, Log, All);