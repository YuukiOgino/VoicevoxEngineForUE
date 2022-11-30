# VoicevoxEngineForUE

![GitHub](https://img.shields.io/github/license/y-chan/node-voicevox-engine)

[VOICEVOX ENGINE](https://github.com/VOICEVOX/voicevox_engine)の**非公式**UnrealEngine5対応プラグインです。<br/>
[ヒホ氏](https://github.com/Hiroshiba)が制作・公開されている、[VOICEVOX CORE](https://github.com/VOICEVOX/voicevox_core)を利用しています。

# 動作要件

* UnrealEngine5.0以上
* Windows10以上
* VisualStudio2022もしくはJetBrains Rider

# プラグイン使用準備

VOICEVOX COREのReadMEに従って、最低限CPUモードの動作に必要なライブラリを取得します。

## VOICEVOX CORE、Open JTalk、ONNX Runtimeの取得

[VOICEVOX COREライブラリ](https://github.com/VOICEVOX/voicevox_core)を取得、適当なフォルダに展開してください。<br/>
※プラグインは[VOICEVOX CORE 0.13.2](https://github.com/VOICEVOX/voicevox_core/releases/tag/0.13.2)を元に開発しました。

次に[Open JTalk から配布されている辞書ファイル](https://sourceforge.net/projects/open-jtalk/files/Dictionary/open_jtalk_dic-1.11/open_jtalk_dic_utf_8-1.11.tar.gz/download?use_mirror=jaist) をダウンロードして、上記コアライブラリを展開したディレクトリに展開してください。

最後に[ONNX Runtime](https://github.com/microsoft/onnxruntime/releases/tag/v1.13.1)をダウンロード、、上記コアライブラリを展開したディレクトリに展開してください。

## プラグインへ展開

展開したCOREライブラリを、以下のフォルダに配置してください。

```
Plugins\VoicevoxEngine\Source\ThirdParty\VoicevoxCore\x64\VoicevoxCore
```

※格納例
![image.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/104377/33be4503-896f-3d38-d27c-0ce3cab73ea4.png)

上記COREライブラリ展開済みのPluginフォルダを、プロジェクトフォルダに配置後、uprojectファイルを右クリックして『Generate Visual Studio project files』を選択し、sln作成をしてください。<br/>
あとはuprojectを起動してビルドが通れば成功です。

# 各クラスについて
Qiitaに記事を公開していますので、そちらを見てください。

# 現在判明している不具合

* COREのAPIを非同期で行った場合、特にRunTextToSpeechからの戻りを待たずにプレイ終了したり、続けてRunTextToSpeechを実行するとクラッシュします。
<br/>→この部分はどうしようか、現在模索中です。

* パッケージングを行うとOpen JTalkフォルダのみコピーを失敗します。
<br/>→お手数ですが、パッケージング済みのフォルダにopen_jtalk_dic_utf_8-1.11フォルダを手動でコピーしてください。
![image.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/104377/23f5f42c-a281-399f-fb57-5b6ad2b1aa74.png)

以下にコピーしてもらえれば動作します。
```
【パッケージング出力フォルダ名】\Windows\【プロジェクト名】\Binaries\Win64
```

* GPUモードに対応したCOREライブラリでGPUに切り替えるとエラー、もしくはクラッシュする
<br/>→こちら原因調査中です。

# 利用規約
VOICEVOX Engine for UE5の利用規約は[VOICEVOXのホームページに記載された利用規約](https://voicevox.hiroshiba.jp/term)を継承するため、利用前に必ず一読してください。
各スピーカーモデルのライセンスは[VOICEVOX公式ホームページのキャラクター一覧](https://voicevox.hiroshiba.jp/)を参照してください。

# 問い合わせに関して
VOICEVOX Engine for UE5プラグインに関して質問があれば、[Twitter](https://twitter.com/YuukiOgino)及びQiitaのコメント欄で聞いてもらうか、[issue](https://github.com/YuukiOgino/VoicevoxEngineForUE/issues)に記載してください。

※VOICEVOX CORE作者のヒホ氏、VOICEVOX公式Twitterアカウント及びVOICEVOX ホームページで、VOICEVOX Engine for UE5プラグインの問い合わせをしないようにお願いいたします。

# ライセンス
VOICEVOX ENGINEのライセンスを継承し、LGPL v3と、ソースコードの公開が不要な別ライセンスのデュアルライセンスとなります。

