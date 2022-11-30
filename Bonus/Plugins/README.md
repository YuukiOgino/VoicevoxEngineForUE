# VoicevoxToolTipTextSpeechプラグイン

[VoicevoxEngineForUEプラグイン](https://github.com/YuukiOgino/VoicevoxEngineForUE)を利用して、ツールチップの文言を読み上げるネタプラグインです。<br/>
Naotsun氏の[Slateのテキストに無理やり(？)アクセスする方法](https://qiita.com/Naotsun/items/5fa305cf91c3dec55c16)で[公開されたサンプルコード](https://github.com/Naotsun19B/SlateTextAccessor)を、一部VOICEVOX COREのAPIを実行するように修正を加えています。。

修正内容がわかる必要最低限のソースコードしか配置していません。

# 動作要件

* UnrealEngine5.0以上
* Windows10以上
* VisualStudio2022もしくはJetBrains Rider

# プラグイン使用準備
公開されたソースコードを見て修正、もしくは[公開されたサンプルコード](https://github.com/Naotsun19B/SlateTextAccessor)から必要なソースコードを取得して各自修正を行ってください。

## 使用方法

動作した場合の使用方法です。

![image.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/104377/e4ab9f87-b0d3-6d9b-52fd-eb7eff6c6c6f.png)
ツールチップが表示されているときにキーボードのPボタンを押すと、ずんだもんのボイスでToolTipのテキストを読み上げます。（VoicevoxToolTipTextSpeechプラグインを改修した場合のみ）

![image.png](https://qiita-image-store.s3.ap-northeast-1.amazonaws.com/0/104377/c44c6302-17ed-b6c3-cc69-20a05b778539.png)
コンテンツブラウザ、とずんだもんの声で読み上げてくれます。

# 問い合わせに関して
VoicevoxToolTipTextSpeechプラグインに関してはサポートを行いません。

# ライセンス
VOICEVOX ENGINEのライセンスを継承し、LGPL v3と、ソースコードの公開が不要な別ライセンスのデュアルライセンスとなります。

# 更新履歴

- 2022/12/01
  - 1.0 公開バージョン
