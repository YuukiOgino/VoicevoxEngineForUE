// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxUEDefined.h
 * @brief  VOICEVOXをUE5内で共通に使用する構造体、列挙体の定義をまとめたヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "VoicevoxUEDefined.generated.h"


//------------------------------------------------------------------------
// enum
//------------------------------------------------------------------------

/**
 * @enum ELipSyncVowelType
 * @brief リップシンク用の母音を示す列挙体
 */
UENUM(BlueprintType)
enum class ELipSyncVowelType : uint8
{
	A	UMETA(DisplayName = "母音:あ",	ToolTip = "母音:あ（a）"),
	I	UMETA(DisplayName = "母音:い",	ToolTip = "母音:い（i）"),
	U	UMETA(DisplayName = "母音:う",	ToolTip = "母音:う（u）"),
	E	UMETA(DisplayName = "母音:え",	ToolTip = "母音:え（e）"),
	O	UMETA(DisplayName = "母音:お",	ToolTip = "母音:お（o）"),
	Non	UMETA(DisplayName = "無音",		ToolTip = "無音（句読点の待機時間）"),
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

/**
 * @struct FVoicevoxStyle
 * @brief VOICEVOXのモデルスタイル情報構造体
 */
USTRUCT(BlueprintType)
struct FVoicevoxSupportedDevices
{
	GENERATED_USTRUCT_BODY()

	// CPU使用可フラグ
	UPROPERTY(BlueprintReadOnly, Category="VOICEVOX Engine")
	bool Cpu;

	// GPU(cuda)使用可フラグ
	UPROPERTY(BlueprintReadOnly, Category="VOICEVOX Engine")
	bool Cuda;

	// GPU(dml)使用可フラグ
	UPROPERTY(BlueprintReadOnly, Category="VOICEVOX Engine")
	bool Dml;
};

/**
 * @struct FVoicevoxMora
 * @brief VOICEVOXのモーラ情報構造体
 */
USTRUCT(BlueprintType)
struct FVoicevoxMora
{
	GENERATED_USTRUCT_BODY()

	// カナ文字（１～２文字）
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	FString Text;

	// 子音
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	FString Consonant;

	// 子音長さ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	float Consonant_length;

	// 母音
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	FString Vowel;

	// 母音長さ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	float Vowel_length;

	// イントネーション
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	float Pitch;
};

/**
 * @struct FVoicevoxAccentPhrase
 * @brief VOICEVOXのアクセント情報構造体
 */
USTRUCT(BlueprintType)
struct FVoicevoxAccentPhrase
{
	GENERATED_USTRUCT_BODY()

	// モーラ情報リスト
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	TArray<FVoicevoxMora> Moras;

	// アクセント場所
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	int Accent;

	// 句読点モーラ情報（カナで句読点が存在する場合、無音時間が格納）
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	FVoicevoxMora Pause_mora;

	// 疑問文か
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	bool Is_interrogative;
};

/**
 * @struct FVoicevoxAudioQuery
 * @brief VOICEVOXのAudioQuery情報構造体
 */
USTRUCT(BlueprintType)
struct FVoicevoxAudioQuery
{
	GENERATED_USTRUCT_BODY()
	
	// アクセントリスト
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	TArray<FVoicevoxAccentPhrase> Accent_phrases;

	// 話速
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine", meta = (ClampMin = "0.5", ClampMax = "2.0", UIMin = "0.5", UIMax = "2.0"))
	float Speed_scale;
	
	// 音高
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine", meta = (ClampMin = "-0.15", ClampMax = "0.15", UIMin = "-0.15", UIMax = "0.15"))
	float Pitch_scale;

	// 抑揚
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine", meta = (ClampMin = "0", ClampMax = "2.0", UIMin = "0", UIMax = "2.0"))
	float Intonation_scale;

	// 音量
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine", meta = (ClampMin = "0", ClampMax = "2.0", UIMin = "0", UIMax = "2.0"))
	float Volume_scale;

	// 開始無音
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine",  meta = (ClampMin = "0", ClampMax = "1.5", UIMin = "0", UIMax = "1.5"))
	float Pre_phoneme_length;

	// 終了無音
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine", meta = (ClampMin = "0", ClampMax = "1.5", UIMin = "0", UIMax = "1.5"))
	float Post_phoneme_length;

	// 出力サンプリングレート
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	int Output_sampling_rate;

	// 出力時にステレオにするか
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	bool Output_stereo;

	// カナ（AquesTalkライクな記法）
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	FString Kana;
};

/**
 * @struct FVoicevoxAudioQuery
 * @brief VOICEVOXのAudioQueryからリップシンクに必要な情報をまとめた構造体
 */
USTRUCT(BlueprintType)
struct FVoicevoxLipSync
{
	GENERATED_USTRUCT_BODY()

	// 母音
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	ELipSyncVowelType VowelType;
	
	// 長さ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	float Length;	
};

/**
 * @struct FVoicevoxCoreProperty
 * @brief VOICEVOXのプロパティ情報をまとめた構造体
 */
USTRUCT(BlueprintType)
struct FVoicevoxCoreProperty
{
	GENERATED_USTRUCT_BODY()

	// VOICEVOX CORE バージョン
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	FString Version;

	// サポートデバイス情報
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	FVoicevoxSupportedDevices SupportedDevices;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VOICEVOX Engine")
	bool IsGpuMode;
};