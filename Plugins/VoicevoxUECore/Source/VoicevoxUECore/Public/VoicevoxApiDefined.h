// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxApiDefined.h
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
   * サポートされているデバイス情報取得に失敗した
   */
  VOICEVOX_RESULT_GET_SUPPORTED_DEVICES_ERROR = 3,
  /**
   * GPUモードがサポートされていない
   */
  VOICEVOX_RESULT_GPU_SUPPORT_ERROR = 4,
  /**
   * 推論ライブラリのロードまたは初期化ができなかった
   */
  VOICEVOX_RESULT_INIT_INFERENCE_RUNTIME_ERROR = 29,
  /**
   * スタイルIDに対するスタイルが見つからなかった
   */
  VOICEVOX_RESULT_STYLE_NOT_FOUND_ERROR = 6,
  /**
   * 音声モデルIDに対する音声モデルが見つからなかった
   */
  VOICEVOX_RESULT_MODEL_NOT_FOUND_ERROR = 7,
  /**
   * 推論に失敗した
   */
  VOICEVOX_RESULT_RUN_MODEL_ERROR = 8,
  /**
   * 入力テキストの解析に失敗した
   */
  VOICEVOX_RESULT_ANALYZE_TEXT_ERROR = 11,
  /**
   * 無効なutf8文字列が入力された
   */
  VOICEVOX_RESULT_INVALID_UTF8_INPUT_ERROR = 12,
  /**
   * AquesTalk風記法のテキストの解析に失敗した
   */
  VOICEVOX_RESULT_PARSE_KANA_ERROR = 13,
  /**
   * 無効なAudioQuery
   */
  VOICEVOX_RESULT_INVALID_AUDIO_QUERY_ERROR = 14,
  /**
   * 無効なAccentPhrase
   */
  VOICEVOX_RESULT_INVALID_ACCENT_PHRASE_ERROR = 15,
  /**
   * ZIPファイルを開くことに失敗した
   */
  VOICEVOX_RESULT_OPEN_ZIP_FILE_ERROR = 16,
  /**
   * ZIP内のファイルが読めなかった
   */
  VOICEVOX_RESULT_READ_ZIP_ENTRY_ERROR = 17,
  /**
   * モデルの形式が不正
   */
  VOICEVOX_RESULT_INVALID_MODEL_HEADER_ERROR = 28,
  /**
   * すでに読み込まれている音声モデルを読み込もうとした
   */
  VOICEVOX_RESULT_MODEL_ALREADY_LOADED_ERROR = 18,
  /**
   * すでに読み込まれているスタイルを読み込もうとした
   */
  VOICEVOX_RESULT_STYLE_ALREADY_LOADED_ERROR = 26,
  /**
   * 無効なモデルデータ
   */
  VOICEVOX_RESULT_INVALID_MODEL_DATA_ERROR = 27,
  /**
   * ユーザー辞書を読み込めなかった
   */
  VOICEVOX_RESULT_LOAD_USER_DICT_ERROR = 20,
  /**
   * ユーザー辞書を書き込めなかった
   */
  VOICEVOX_RESULT_SAVE_USER_DICT_ERROR = 21,
  /**
   * ユーザー辞書に単語が見つからなかった
   */
  VOICEVOX_RESULT_USER_DICT_WORD_NOT_FOUND_ERROR = 22,
  /**
   * OpenJTalkのユーザー辞書の設定に失敗した
   */
  VOICEVOX_RESULT_USE_USER_DICT_ERROR = 23,
  /**
   * ユーザー辞書の単語のバリデーションに失敗した
   */
  VOICEVOX_RESULT_INVALID_USER_DICT_WORD_ERROR = 24,
  /**
   * UUIDの変換に失敗した
   */
  VOICEVOX_RESULT_INVALID_UUID_ERROR = 25,
};

/**
 * @enum VoicevoxUserDictWordType
 * @brief ユーザー辞書の単語の種類。
 */
enum VoicevoxUserDictWordType : int32_t
 {
    /**
     * 固有名詞。
     */
    VOICEVOX_USER_DICT_WORD_TYPE_PROPER_NOUN = 0,
    /**
     * 一般名詞。
     */
    VOICEVOX_USER_DICT_WORD_TYPE_COMMON_NOUN = 1,
    /**
     * 動詞。
     */
    VOICEVOX_USER_DICT_WORD_TYPE_VERB = 2,
    /**
     * 形容詞。
     */
    VOICEVOX_USER_DICT_WORD_TYPE_ADJECTIVE = 3,
    /**
     * 接尾辞。
     */
    VOICEVOX_USER_DICT_WORD_TYPE_SUFFIX = 4,
  };

//------------------------------------------------------------------------
// struct
//------------------------------------------------------------------------

/**
 * テキスト解析器としてのOpen JTalk。
 *
 * <b>構築</b>(_construction_)は ::voicevox_open_jtalk_rc_new で行い、<b>破棄</b>(_destruction_)は ::voicevox_open_jtalk_rc_delete で行う。
 *
 * 参照カウント方式のスマートポインタ(reference-counted smart pointer)であり、
 * ::voicevox_synthesizer_new に渡されるときには参照カウンタがインクリメントされる形でオブジェクトの共有が行われる。
 *
 * \example{
 * ```c
 * OpenJtalkRc *open_jtalk;
 * voicevox_open_jtalk_rc_new("./open_jtalk_dic_utf_8-1.11", &open_jtalk);
 * // ⋮
 * voicevox_open_jtalk_rc_delete(open_jtalk);
 * ```
 * }
 *
 * \orig-impl{OpenJtalkRc}
 */
typedef struct OpenJtalkRc OpenJtalkRc;

/**
 * ONNX Runtime。
 *
 * シングルトンであり、インスタンスは高々一つ。
 *
 * ```c
 * const VoicevoxOnnxruntime *ort1;
 * voicevox_onnxruntime_load_once(
 *     voicevox_make_default_load_onnxruntime_options(), &ort1);
 * const VoicevoxOnnxruntime *ort2 = voicevox_onnxruntime_get();
 * assert(ort1 == ort2);
 * ```
 *
 * \orig-impl{VoicevoxOnnxruntime}
 */
typedef struct VoicevoxOnnxruntime VoicevoxOnnxruntime;

/**
 * 音声シンセサイザ。
 *
 * <b>構築</b>(_construction_)は ::voicevox_synthesizer_new で行い、<b>破棄</b>(_destruction_)は ::voicevox_synthesizer_delete で行う。
 *
 * \orig-impl{VoicevoxSynthesizer}
 */
typedef struct VoicevoxSynthesizer VoicevoxSynthesizer;

/**
 * ユーザー辞書。
 *
 * \orig-impl{VoicevoxUserDict}
 */
typedef struct VoicevoxUserDict VoicevoxUserDict;

/**
 * 音声モデルファイル。
 *
 * VVMファイルと対応する。
 * <b>構築</b>(_construction_)は ::voicevox_voice_model_file_open で行い、<b>破棄</b>(_destruction_)は ::voicevox_voice_model_file_delete で行う。
 *
 * \orig-impl{VoicevoxVoiceModelFile}
 */
typedef struct VoicevoxVoiceModelFile VoicevoxVoiceModelFile;

/**
 * @struct VoicevoxLoadOnnxruntimeOptions
 * @brief voicevox_onnxruntime_load_once のオプション。
 */
struct VoicevoxLoadOnnxruntimeOptions
{
    /**
     * ONNX Runtimeのファイル名（モジュール名）もしくはファイルパスを指定する。
     *
     * `dlopen`/[`LoadLibraryExW`](https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibraryexw)の引数に使われる。デフォルトは ::voicevox_get_onnxruntime_lib_versioned_filename と同じ。
     */
    const char *filename;
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
     * 疑問文の調整を有効にする
     */
    bool enable_interrogative_upspeak;
};

/**
 * @struct VoicevoxUserDictWord
 * @brief ユーザー辞書の単語。
 */
struct VoicevoxUserDictWord
{
    /**
     * 表記
     */
    const char *surface;
    /**
     * 読み
     */
    const char *pronunciation;
    /**
     * アクセント型
     */
    uintptr_t accent_type;
    /**
     * 単語の種類
     */
    VoicevoxUserDictWordType word_type;
    /**
     * 優先度
     */
    uint32_t priority;
} ;