# VoicevoxEngineForUE

![GitHub](https://img.shields.io/github/license/y-chan/node-voicevox-engine)

[VOICEVOX ENGINE](https://github.com/VOICEVOX/voicevox_engine)の**非公式**UnrealEngine5対応プラグインです。
[ヒホ氏](https://github.com/Hiroshiba)が制作・公開されている、
[VOICEVOX CORE](https://github.com/VOICEVOX/voicevox_core)を利用しています。

# 動作要件

* UnrealEngine5.0以上
* Windows10以上
* VisualStudio2022もしくはJetBrains Rider

# プラグイン使用準備

VOICEVOX COREのReadMEに従って、最低限CPUモードの動作に必要なライブラリを取得します。

## VOICEVOX CORE、Open JTalk、ONNX Runtimeの取得

以下からVOICEVOX COREライブラリを取得、適当なフォルダに展開してください。
https://github.com/VOICEVOX/voicevox_core

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

上記COREライブラリ展開済みのPluginフォルダを、プロジェクトフォルダに配置後、uprojectファイルを右クリックして『Generate Visual Studio project files』を選択し、sln作成をしてください。
あとはuprojectを起動してビルドが通れば成功です。



