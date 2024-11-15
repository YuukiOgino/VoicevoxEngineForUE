# VoicevoxEngine For UE サンプルプロジェクト

> [!NOTE]
> 事前にPluginsフォルダ内の以下のReadMEを参考に、voicevox_coreの動的ライブラリ、及び必要なフォルダを設置してください。
> 
> [Windows VOICEVOX CORE設置場所のReadME](https://github.com/YuukiOgino/VoicevoxEngineForUE/blob/main/Plugins/VoicevoxNativeCore/Source/ThirdParty/VoicevoxCore/x64/README.md)<br/>
> [Windows VOICEVOX NEMO CORE設置場所のReadME](https://github.com/YuukiOgino/VoicevoxEngineForUE/blob/main/Plugins/VoicevoxNativeCoreNemo/Source/ThirdParty/VoicevoxCore/x64/README.md)
> 
> [Mac VOICEVOX CORE設置場所のReadME](https://github.com/YuukiOgino/VoicevoxEngineForUE/blob/main/Plugins/VoicevoxNativeCore/Source/ThirdParty/VoicevoxCore/osx/README.md)<br/>
> [Mac VOICEVOX NEMO CORE設置場所のReadME](https://github.com/YuukiOgino/VoicevoxEngineForUE/blob/main/Plugins/VoicevoxNativeCoreNemo/Source/ThirdParty/VoicevoxCore/osx/README.md)

## Windows

Windowsでのサンプルプロジェクトの起動方法です。

### 1.Pluginsフォルダをコピー、もしくはシンボリックリンク作成

以下の1か2を実行してください。
1. PluginsフォルダをSampleフォルダ内に手動でコピーする
2. Win_Pluginジャンクション設定.batを実行し、Pluginsフォルダのシンボリックリンクを作成

シンボリックリンクを作成した場合、Sample/Pluginフォルダを削除する場合はWin_Pluginジャンクション削除.batを実行してください。

### 2.Generate Project Filesを実行

uprojectを右クリック→「Generate Project Files」を実行して、slnファイルを生成してください。

- 公式ドキュメント
[Project Files for IDEs](https://dev.epicgames.com/documentation/en-us/unreal-engine/how-to-generate-unreal-engine-project-files-for-your-ide)

バッチファイルでの実行方法もあるようです。（※こちらは試してません。）<br/>
[Generate VS Project Files by Command Line](https://forums.unrealengine.com/t/generate-vs-project-files-by-command-line/277707/18)

### 3.slnファイルを起動→ビルド

![image](https://github.com/user-attachments/assets/14a4504a-3844-4e4e-843f-fb5bff08d6ef)

DebugGame Editor、もしくはDevelopment Edotorでビルドが通れば成功です。

## Mac

Macでのサンプルプロジェクトの起動方法です。

### 1.uprojectのEngineAssociationを5.2以上に設定

![image](https://github.com/user-attachments/assets/5ee3ac72-6791-465a-a300-737dad4c791d)

事前にuprojectのEngineAssociationを5.2以上に書き換えてください。

### 2.Pluginsフォルダをコピー、もしくはシンボリックリンク作成

以下の1か2を実行してください。
1. PluginsフォルダをSampleフォルダ内に手動でコピーする
2. ターミナルでMackPluginLink.shを実行し、Pluginsフォルダのシンボリックリンクを作成

シェルスクリプトの実行例
```
sh MacPluginLink.sh 
```

シンボリックリンクを作成した場合、Sample/Pluginフォルダを削除する場合はMacPluginUnlink.shを実行してください。

```
sh MacPluginUnlink.sh 
```

### 3.xcworkspaceをコマンドラインで生成

Macもuprojectを右クリック→「Generate Project Files」を実行できますが、xcworkspaceが生成されません。<br/>
そのため、ターミナルで**GenerateProjectFiles.sh**を実行する必要があります。

以下、UnrealEngine5.2.ｘのインストール場所を初期設定にしている場合のコマンド例です。

```
"/Users/Shared/Epic Games/UE_5.2/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh" -project="/Users/yukiogino/Documents/VoicevoxEngineForUE/Sample/VoicevoxEngineSample.uproject" -game
```

「5.2」の部分をEngineAssociationで指定したバージョン、-projectに『VoicevoxEngineSample.uproject』のパスを設定してください。

UnrealEngine5.2のインストール場所を初期設定にしている場合、以下のシェルスクリプトを実行していただくとxcworkspaceを生成します。

```
sh MacGenerateProjectFile.sh 
```
エンジンバージョンの値を変更すると5.3以降も使用できるので、ぜひ使用してください。

> [!NOTE]
>　5.2ではxcworkspace、5.3以降ではiOSとMacのxcworkspaceが生成されます。<br/>
>　5.3以降ではMacのxcworkspaceを起動してください。

### 4.xcworkspaceファイルを起動→ビルド

5.2ではそのままRun実行、5.3以降ではVoicevoxEngineSmpleとEditorを選択後にRunを実行してください。

- 5.2<br/>
<img width="565" alt="スクリーンショット 2024-11-15 23 44 21" src="https://github.com/user-attachments/assets/e6305a81-5cd5-4c93-a8b8-5471195b67dc"><br/><br/>

> [!NOTE]
> VOICEVO CORE関連のdylib全てがセキュリティに引っかかりビルドに失敗します。<br/>
> 失敗した場合は「システム設定」→「プライバシーとセキュリティ」でdylibのブロックを解除してください。<br/>
> 参考：[[解決]macOS Sequoiaで野良アプリが開けない問題](https://note.com/5mingame2/n/n5912b7ce9474)

> [!NOTE]
> UE5.2～4では、XCode16以上では**ビルドに失敗します**。XCode14.x及びXCode15.xでビルドをしてください。<br/>
> 過去バージョンは[Apple Developer](https://developer.apple.com/jp/xcode/resources/)にアクセスすれば入手できます。<br/>
> ※サンプルプロジェクトはXCode15.4で確認しています
