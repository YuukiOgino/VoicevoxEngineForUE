// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxNativeDefined.h
 * @brief  VOICEVOX COREで使用している構造体、列挙体の定義をまとめたヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

//------------------------------------------------------------------------
// enum
//------------------------------------------------------------------------

/**
 * @enum VoicevoxAccelerationMode
 * @brief ハードウェアアクセラレーションモードを設定する設定値
 */
enum class VoicevoxAccelerationMode : int32_t
{
    /**
     * 実行環境に合った適切なハードウェアアクセラレーションモードを選択する
     */
    VOICEVOX_ACCELERATION_MODE_AUTO = 0,
    /**
     * ハードウェアアクセラレーションモードを"CPU"に設定する
     */
    VOICEVOX_ACCELERATION_MODE_CPU = 1,
    /**
     * ハードウェアアクセラレーションモードを"GPU"に設定する
     */
    VOICEVOX_ACCELERATION_MODE_GPU = 2,
};

/**
 * @enum VoicevoxResultCode
 * @brief 処理結果を示す結果コード
 */
enum class VoicevoxResultCode : int32_t
{
    /**
     * 成功
     */
    VOICEVOX_RESULT_OK = 0,
    /**
     * open_jtalk辞書ファイルが読み込まれていない
     */
    VOICEVOX_RESULT_NOT_LOADED_OPENJTALK_DICT_ERROR = 1,
    /**
     * modelの読み込みに失敗した
     */
    VOICEVOX_RESULT_LOAD_MODEL_ERROR = 2,
    /**
     * サポートされているデバイス情報取得に失敗した
     */
    VOICEVOX_RESULT_GET_SUPPORTED_DEVICES_ERROR = 3,
    /**
     * GPUモードがサポートされていない
     */
    VOICEVOX_RESULT_GPU_SUPPORT_ERROR = 4,
    /**
     * メタ情報読み込みに失敗した
     */
    VOICEVOX_RESULT_LOAD_METAS_ERROR = 5,
    /**
     * ステータスが初期化されていない
     */
    VOICEVOX_RESULT_UNINITIALIZED_STATUS_ERROR = 6,
    /**
     * 無効なspeaker_idが指定された
     */
    VOICEVOX_RESULT_INVALID_SPEAKER_ID_ERROR = 7,
    /**
     * 無効なmodel_indexが指定された
     */
    VOICEVOX_RESULT_INVALID_MODEL_INDEX_ERROR = 8,
    /**
     * 推論に失敗した
     */
    VOICEVOX_RESULT_INFERENCE_ERROR = 9,
    /**
     * コンテキストラベル出力に失敗した
     */
    VOICEVOX_RESULT_EXTRACT_FULL_CONTEXT_LABEL_ERROR = 10,
    /**
     * 無効なutf8文字列が入力された
     */
    VOICEVOX_RESULT_INVALID_UTF8_INPUT_ERROR = 11,
    /**
     * aquestalk形式のテキストの解析に失敗した
     */
    VOICEVOX_RESULT_PARSE_KANA_ERROR = 12,
    /**
     * 無効なAudioQuery
     */
    VOICEVOX_RESULT_INVALID_AUDIO_QUERY_ERROR = 13,
};

/**
 * @struct VoicevoxInitializeOptions
 * @brief 初期化オプション
 */
struct VoicevoxInitializeOptions
{
    /**
     * ハードウェアアクセラレーションモード
     */
    VoicevoxAccelerationMode acceleration_mode;
    /**
     * CPU利用数を指定
     * 0を指定すると環境に合わせたCPUが利用される
     */
    uint16_t cpu_num_threads;
    /**
     * 全てのモデルを読み込む
     */
    bool load_all_models;
    /**
     * open_jtalkの辞書ディレクトリ
     */
    const char *open_jtalk_dict_dir;
};

/**
 * @struct VoicevoxAudioQueryOptions
 * @brief Audio query のオプション
 */
struct VoicevoxAudioQueryOptions
{
    /**
     * aquestalk形式のkanaとしてテキストを解釈する
     */
    bool kana;
};

/**
 * @struct VoicevoxSynthesisOptions
 * @brief `voicevox_synthesis` のオプション
 */
struct VoicevoxSynthesisOptions
{
    /**
     * 疑問文の調整を有効にする
     */
    bool enable_interrogative_upspeak;
};

/**
 * @struct VoicevoxTtsOptions
 * @brief テキスト音声合成オプション
 */
struct VoicevoxTtsOptions
{
    /**
     * aquestalk形式のkanaとしてテキストを解釈する
     */
    bool kana;
    /**
     * 疑問文の調整を有効にする
     */
    bool enable_interrogative_upspeak;
};