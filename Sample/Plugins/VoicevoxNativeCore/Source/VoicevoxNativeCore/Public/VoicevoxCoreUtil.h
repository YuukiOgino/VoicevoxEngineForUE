// Copyright Yuuki Ogino. All Rights Reserved.

/**
 * @headerfile VoicevoxCoreUtil.h
 * @brief  VOICEVOX COREのAPIを呼び出すUtilクラスをまとめたヘッダーファイル
 * @author Yuuki Ogino
 */

#pragma once

#include "VoicevoxUEDefined.h"
#include "VoicevoxCoreUtil.generated.h"

//------------------------------------------------------------------------
// enum
//------------------------------------------------------------------------

/**
 * @enum EVoicevoxSpeakerType
 * @brief Speakerを示す列挙体(Ver.0.14.5時点)
 */
UENUM(BlueprintType)
enum class EVoicevoxSpeakerType : uint8
{
	Metan=2							UMETA(DisplayName = "四国めたん(ノーマル)",			ToolTip = "02:四国めたん(ノーマル)"),
	MetanAmaAma=0					UMETA(DisplayName = "四国めたん(あまあま)",			ToolTip = "00:四国めたん(あまあま)"),
	MetanTsunTsun=6					UMETA(DisplayName = "四国めたん(ツンツン)",			ToolTip = "06:四国めたん(ツンツン)"),
	MetanSexy=4						UMETA(DisplayName = "四国めたん(セクシー)",			ToolTip = "04:四国めたん(セクシー)"),
	MetanSasayaki=36				UMETA(DisplayName = "四国めたん(ささやき)",			ToolTip = "36:四国めたん(ささやき)"),
	MetanHisoHiso=37				UMETA(DisplayName = "四国めたん(ヒソヒソ)",			ToolTip = "37:四国めたん(ヒソヒソ)"),
	Zundamon=3						UMETA(DisplayName = "ずんだもん(ノーマル)",			ToolTip = "03:ずんだもん(ノーマル)"),
	ZundamonAmaAma=1				UMETA(DisplayName = "ずんだもん(あまあま)",			ToolTip = "01:ずんだもん(あまあま)"),
	ZundamonTsunTsun=7				UMETA(DisplayName = "ずんだもん(ツンツン)",			ToolTip = "07:ずんだもん(ツンツン)"),
	ZundamonSexy=5					UMETA(DisplayName = "ずんだもん(セクシー)",			ToolTip = "05:ずんだもん(セクシー)"),
	ZundamonSasayaki=22				UMETA(DisplayName = "ずんだもん(ささやき)",			ToolTip = "22:ずんだもん(ささやき)"),
	ZundamonHisoHiso=38				UMETA(DisplayName = "ずんだもん(ヒソヒソ)",			ToolTip = "38:ずんだもん(ヒソヒソ)"),
	ZundamonHerohero=75				UMETA(DisplayName = "ずんだもん(ヘロヘロ)",			ToolTip = "22:ずんだもん(ヘロヘロ)"),
	ZundamonNamidame=76				UMETA(DisplayName = "ずんだもん(なみだめ)",			ToolTip = "38:ずんだもん(なみだめ)"),
	Tsumugi=8						UMETA(DisplayName = "春日部つむぎ",					ToolTip = "08:春日部つむぎ"),
	Hau=10							UMETA(DisplayName = "雨晴はう",						ToolTip = "10:雨晴はう"),
	Ritsu=9							UMETA(DisplayName = "波音リツ(ノーマル)",				ToolTip = "09:波音リツ(ノーマル)"),
	RitsuQueen=65					UMETA(DisplayName = "波音リツ(クイーン)",				ToolTip = "65:波音リツ(クイーン)"),
	Kurono=11						UMETA(DisplayName = "玄野武宏(ノーマル)",				ToolTip = "11:玄野武宏(ノーマル)"),
	KuronoYorokobi=39				UMETA(DisplayName = "玄野武宏(喜び)",					ToolTip = "39:玄野武宏(喜び)"),
	KuronoTsungire=40				UMETA(DisplayName = "玄野武宏(ツンギレ)",				ToolTip = "40:玄野武宏(ツンギレ)"),
	KuronoKanashimi=41				UMETA(DisplayName = "玄野武宏(悲しみ)",				ToolTip = "41:玄野武宏(悲しみ)"),
	Kotarou=12						UMETA(DisplayName = "白上虎太郎(ふつう)",				ToolTip = "12:白上虎太郎(ふつう)"),
	KotarouWai=32					UMETA(DisplayName = "白上虎太郎(わーい)",				ToolTip = "32:白上虎太郎(わーい)"),
	KotarouBikuBiku=33				UMETA(DisplayName = "白上虎太郎(びくびく)",			ToolTip = "33:白上虎太郎(びくびく)"),
	KotarouOko=34					UMETA(DisplayName = "白上虎太郎(おこ)",				ToolTip = "34:白上虎太郎(おこ)"),
	KotarouBien=35					UMETA(DisplayName = "白上虎太郎(びえーん)",			ToolTip = "35:白上虎太郎(びえーん)"),
	Ryusei=13						UMETA(DisplayName = "青山龍星",						ToolTip = "13:青山龍星"),
	RyuseiNekketsu=81				UMETA(DisplayName = "青山龍星(熱血)",					ToolTip = "13:青山龍星(熱血)"),
	RyuseiHukigen=82				UMETA(DisplayName = "青山龍星(不機嫌)",				ToolTip = "13:青山龍星(不機嫌)"),
	RyuseiYorokobi=83				UMETA(DisplayName = "青山龍星(喜び)",					ToolTip = "13:青山龍星(喜び)"),
	RyuseiShittori=84				UMETA(DisplayName = "青山龍星(しっとり)",				ToolTip = "13:青山龍星(しっとり)"),
	RyuseiKanashimi=85				UMETA(DisplayName = "青山龍星(かなしみ)",				ToolTip = "13:青山龍星(かなしみ)"),
	RyuseiSasayaki=86				UMETA(DisplayName = "青山龍星(囁き)",					ToolTip = "13:青山龍星(囁き)"),
	Himari=14						UMETA(DisplayName = "冥鳴ひまり",						ToolTip = "14:冥鳴ひまり"),
	Sora=16							UMETA(DisplayName = "九州そら(ノーマル)",				ToolTip = "16:九州そら(ノーマル)"),
	SoraAmaAma=15					UMETA(DisplayName = "九州そら(あまあま)",				ToolTip = "15:九州そら(あまあま)"),
	SoraTsunTsun=18					UMETA(DisplayName = "九州そら(ツンツン)",				ToolTip = "18:九州そら(ツンツン)"),
	SoraSexy=17						UMETA(DisplayName = "九州そら(セクシー)",				ToolTip = "17:九州そら(セクシー)"),
	SoraSasayaki=19					UMETA(DisplayName = "九州そら(ささやき)",				ToolTip = "19:九州そら(ささやき)"),
	MochikoSan=20					UMETA(DisplayName = "もち子さん(ノーマル)",			ToolTip = "20:もち子さん(ノーマル)"),
	MochikoSanSexy=66				UMETA(DisplayName = "もち子さん(セクシー／あん子)",		ToolTip = "66:もち子さん(セクシー／あん子)"),
	MochikoSanNaki=77				UMETA(DisplayName = "もち子さん(泣き)",				ToolTip = "66:もち子さん(泣き)"),
	MochikoSanIkari=78				UMETA(DisplayName = "もち子さん(怒り)",				ToolTip = "66:もち子さん(怒り)"),
	MochikoSanYorokobi=79			UMETA(DisplayName = "もち子さん(喜び)",				ToolTip = "66:もち子さん(喜び)"),
	MochikoSanNonbiri=80			UMETA(DisplayName = "もち子さん(のんびり)",			ToolTip = "66:もち子さん(のんびり)"),
	Kenzaki=21						UMETA(DisplayName = "剣崎雌雄",						ToolTip = "21:剣崎雌雄"),
	WhiteCul=23						UMETA(DisplayName = "WhiteCUL(ノーマル)",			ToolTip = "23:WhiteCUL(ノーマル)"),
	WhiteCulTanoshi=24				UMETA(DisplayName = "WhiteCUL(たのしい)",			ToolTip = "24:WhiteCUL(たのしい)"),
	WhiteCulKanashi=25				UMETA(DisplayName = "WhiteCUL(かなしい)",			ToolTip = "25:WhiteCUL(かなしい)"),
	WhiteCulBien=26					UMETA(DisplayName = "WhiteCUL(びえーん)",			ToolTip = "26:WhiteCUL(びえーん)"),
	Goki=27							UMETA(DisplayName = "後鬼(人間ver.)",				ToolTip = "27:後鬼(人間ver.ル)"),
	GokiNuigurumi=28				UMETA(DisplayName = "後鬼(ぬいぐるみver.)",			ToolTip = "28:後鬼(ぬいぐるみver.)"),
	No7=29							UMETA(DisplayName = "No.7(ノーマル)",				ToolTip = "29:No.7(ノーマル)"),
	No7Announcement=30				UMETA(DisplayName = "No.7(アナウンス)",				ToolTip = "30:No.7(アナウンス)"),
	No7Yomikikase=31				UMETA(DisplayName = "No.7(読み聞かせ)",				ToolTip = "31:No.7(読み聞かせ)"),
	Chibishikiji=42					UMETA(DisplayName = "ちび式じい",						ToolTip = "42:ちび式じい"),
	OukaMiko=43						UMETA(DisplayName = "櫻歌ミコ(ノーマル)",				ToolTip = "43:櫻歌ミコ(ノーマル)"),
	OukaMikoSecond=44				UMETA(DisplayName = "櫻歌ミコ(第二形態)",				ToolTip = "44:櫻歌ミコ(第二形態)"),
	OukaMikoLoli=45					UMETA(DisplayName = "櫻歌ミコ(ロリ)",					ToolTip = "45:櫻歌ミコ(ロリ)"),
	Sayo=46							UMETA(DisplayName = "小夜/SAYO",						ToolTip = "46:小夜/SAYO"),
	TypeT=47						UMETA(DisplayName = "ナースロボ＿タイプＴ(ノーマル)",	ToolTip = "47:ナースロボ＿タイプＴ(ノーマル)"),
	TypeTRakuRaku=48				UMETA(DisplayName = "ナースロボ＿タイプＴ(楽々)",		ToolTip = "48:ナースロボ＿タイプＴ(楽々)"),
	TypeTFear=49					UMETA(DisplayName = "ナースロボ＿タイプＴ(恐怖)",		ToolTip = "49:ナースロボ＿タイプＴ(恐怖)"),
	TypeTSecret=50					UMETA(DisplayName = "ナースロボ＿タイプＴ(内緒話)",		ToolTip = "50:ナースロボ＿タイプＴ(内緒話)"),
	BeniZakura=51					UMETA(DisplayName = "†聖騎士 紅桜†",					ToolTip = "51:†聖騎士 紅桜†"),
	WakamatsuAtsushi=52				UMETA(DisplayName = "雀松朱司",						ToolTip = "52:雀松朱司"),
	KigashimaRin=53					UMETA(DisplayName = "麒ヶ島宗麟",						ToolTip = "53:麒ヶ島宗麟"),
	HarukaNana=54					UMETA(DisplayName = "春歌ナナ",						ToolTip = "54:春歌ナナ"),
	NekotukaAru=55					UMETA(DisplayName = "猫使アル(ノーマル)",				ToolTip = "55:猫使アル(ノーマル)"),
	NekotukaAruOchitsuki=56			UMETA(DisplayName = "猫使アル(おちつき)",				ToolTip = "56:猫使アル(おちつき)"),
	NekotukaAruUkiUki=57			UMETA(DisplayName = "猫使アル(うきうき)",				ToolTip = "57:猫使アル(うきうき)"),
	NekotukaBi=58					UMETA(DisplayName = "猫使ビィ(ノーマル)",				ToolTip = "58:猫使ビィ(ノーマル)"),
	NekotukaBiOchitsuki=59			UMETA(DisplayName = "猫使ビィ(おちつき)",				ToolTip = "59:猫使ビィ(おちつき)"),
	NekotukaBiHitomishiri=60		UMETA(DisplayName = "猫使ビィ(人見知り)",				ToolTip = "60:猫使ビィ(人見知り)"),
	ChugokuUsagi=61					UMETA(DisplayName = "中国うさぎ(ノーマル)",			ToolTip = "61:中国うさぎ(ノーマル)"),
	ChugokuUsagiOdoroki=62			UMETA(DisplayName = "中国うさぎ(おどろき)",			ToolTip = "62:中国うさぎ(おどろき)"),
	ChugokuUsagiKowagari=63			UMETA(DisplayName = "中国うさぎ(こわがり)",			ToolTip = "63:中国うさぎ(こわがり)"),
	ChugokuUsagiHeroHero=64			UMETA(DisplayName = "中国うさぎ(へろへろ)",			ToolTip = "64:中国うさぎ(へろへろ)"),
	KuritaMaron=67					UMETA(DisplayName = "栗田まろん",						ToolTip = "67:栗田まろん"),
	AieruTan=68						UMETA(DisplayName = "あいえるたん",					ToolTip = "68:あいえるたん"),
	ManbetsuHanamaru=69				UMETA(DisplayName = "満別花丸(ノーマル)",				ToolTip = "69:満別花丸(ノーマル)"),
	ManbetsuHanamaruGenki=70		UMETA(DisplayName = "満別花丸(元気)",					ToolTip = "70:満別花丸(元気)"),
	ManbetsuHanamaruSasayaki=71		UMETA(DisplayName = "満別花丸(ささやき)",				ToolTip = "71:満別花丸(ささやき)"),
	ManbetsuHanamaruBurikkko=72		UMETA(DisplayName = "満別花丸(ぶりっ子)",				ToolTip = "72:満別花丸(ぶりっ子)"),
	ManbetsuHanamaruBoy=73			UMETA(DisplayName = "満別花丸(ボーイ)",				ToolTip = "73:満別花丸(ボーイ)"),
	KotoyomiNia=74					UMETA(DisplayName = "琴詠ニア",						ToolTip = "74:琴詠ニア"),
};

//------------------------------------------------------------------------
// FVoicevoxCoreUtil
//------------------------------------------------------------------------

/**
 * @class FVoicevoxCoreUtil
 * @brief VOICEVOX COREのAPIを呼び出すUtilクラス
 */
class VOICEVOXNATIVECORE_API FVoicevoxCoreUtil
{
	friend class FVoicevoxNativeCoreModule;
	
	//! 初期化処理実施済みか
	inline static bool bIsInit = false;

	/**
	 * @brief VOICEVOXから受信したエラーメッセージを表示
	 * @param[in] MessageFormat : エラーメッセージのフォーマット
	 */
	static void ShowVoicevoxErrorMessage(const FString& MessageFormat);

	/**
	 * @fn
	 * VOICEVOX COREのvoicevox_ttsで生成した音声データを開放
	 * @brief voicevox_tts等で生成した音声データを開放する
	 * @param Wav 開放する音声データのポインタ
	 */
	static void WavFree(uint8* Wav);

	/**
	 * @fn
	 * VOICEVOX CORE 初期化
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
	 * VOICEVOX CORE 終了処理
	 * @brief 終了処理を行う。以降VOICEVOXのAPIを利用するためには再度Initializeメソッドを行う必要がある。
	 * @detail
	 * VOICEVOXの終了処理は何度も実行可能。
	 * 実行せずにexitしても大抵の場合問題ないが、CUDAを利用している場合は終了処理を実行しておかないと例外が起こることがある。
	 */
	static void Finalize();

	/**
	 * @fn
	 * メタ情報を取得する
	 * @brief 話者名や話者IDのリストを取得する
	 * @return メタ情報が格納されたjson形式の構造体
	 */
	static TArray<FVoicevoxMeta> GetMetaList();

	/**
	 * @fn
	 * メタ情報から指定したSpeakerIDの名前を取得する
	 * @brief 指定したSpeakerIDの名前を取得する
	 * @return 指定したSpeakerIDの名前
	 */
	static FString GetMetaName(int64 SpeakerID);
	
	/**
	 * @brief VOICEVOX COREのバージョンを取得する
	 * @return SemVerでフォーマットされたバージョン
	 */
	static FString GetVoicevoxVersion();

	/**
	 * @brief ハードウェアアクセラレーションがGPUモードか判定する
	 * @return GPUモードならtrue、そうでないならfalse
	 */
	static bool IsGpuMode();
	
	/**
	 * @brief サポートデバイス情報を取得する
	 * @return サポートデバイス情報の構造体
	 */
	static FVoicevoxSupportedDevices GetSupportedDevices();
	
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
	 * VOICEVOX COREのvoicevox_audio_queryを取得
	 * @brief AudioQuery を取得する。
	 * @param[in] SpeakerId 話者番号
	 * @param[in] Message 音声データに変換するtextデータ
	 * @param[in] bKana aquestalk形式のkanaとしてテキストを解釈する
	 * @return AudioQueryをjsonでフォーマット後、構造体へ変換したもの。
	 * @details
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	static FVoicevoxAudioQuery GetAudioQuery(int64 SpeakerId, const FString& Message, bool bKana);

	/**
	 * @fn
	 * VOICEVOX COREのtext to speechを実行
	 * @brief Textデータを音声データに変換する。
	 * @param[in] SpeakerId 話者番号
	 * @param[in] Message 音声データに変換するtextデータ
	 * @param[in] bKana aquestalk形式のkanaとしてテキストを解釈する
	 * @param[in] bEnableInterrogativeUpspeak 疑問文の調整を有効にする
	 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
	 * @details
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	static TArray<uint8> RunTextToSpeech(int64 SpeakerId, const FString& Message, bool bKana, bool bEnableInterrogativeUpspeak);

	/**
	 * @fn
	 * VOICEVOX COREのvoicevox_synthesisを実行
	 * @brief AudioQueryを音声データに変換する。
	 * @param[in] AudioQueryJson jsonフォーマットされた AudioQuery
	 * @param[in] SpeakerId 話者番号
	 * @param[in] bEnableInterrogativeUpspeak 疑問文の調整を有効にする
	 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
	 * @details
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	static TArray<uint8> RunSynthesis(const char* AudioQueryJson, int64 SpeakerId, bool bEnableInterrogativeUpspeak);

	/**
	 * @fn
	 * VOICEVOX COREのvoicevox_synthesisを実行
	 * @brief AudioQueryを音声データに変換する。
	 * @param[in] AudioQueryJson jsonフォーマットされた AudioQuery構造体
	 * @param[in] SpeakerId 話者番号
	 * @param[in] bEnableInterrogativeUpspeak 疑問文の調整を有効にする
	 * @return 音声データを出力する先のポインタ。使用が終わったらvoicevox_wav_freeで開放する必要がある
	 * @details
	 * ※メインスレッドが暫く止まるほど重いので、非同期で処理してください。（UE::Tasks::Launch等）
	 */
	static TArray<uint8> RunSynthesis(const FVoicevoxAudioQuery& AudioQueryJson, int64 SpeakerId, bool bEnableInterrogativeUpspeak);
	
public:
	
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
	static TArray<float> DecodeForward(int64 Length, int64 PhonemeSize, TArray<float> F0, TArray<float> Phoneme, int64 SpeakerID);
};

DECLARE_LOG_CATEGORY_EXTERN(LogVoicevoxEngine, Log, All);