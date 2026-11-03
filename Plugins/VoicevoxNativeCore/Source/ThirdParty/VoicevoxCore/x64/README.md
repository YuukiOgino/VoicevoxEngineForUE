# VOICEVOX CORE、Open JTalk、ONNX Runtimeをこのフォルダに配置してください。

- 格納例<br/>
<img width="598" height="330" alt="image" src="https://github.com/user-attachments/assets/ff658d0b-241c-46db-9bf0-01169ddc2438" />


## CPUモードの場合

以下のDLL及びフォルダを格納してください。

* open_jtalk_dic_utf_8-1.11フォルダ
* modelフォルダ
* voicevox_core.dll
* voicevox_core.lib
* voicevox_onnxruntime.dll
* voicevox_onnxruntime.lib

## GPUモードの場合

> [!NOTE]
> GPU起動に必要なDLLはダウンローダー以外では入手できません。必ずダウンローダーからVOICEVOX COREを入手してください。

> [!WARNING]
> ダウンローダーの最新版(Latest)はVOICEVOX CORE 0.16.ｘ以上のダウンロードに対応しており、15.9以下のバージョンはフォルダ構造の違いからダウンロードできません。<br/>
> ダウンローダーは必ず16.ｘから使用してください。<br/>
> https://github.com/VOICEVOX/voicevox_core/releases/tag/0.16.4

ダウンローダー　DirectML版コマンド例
```PowerShell
Invoke-WebRequest https://github.com/VOICEVOX/voicevox_core/releases/download/0.16.4/download-windows-x64.exe -OutFile ./download.exe
./download.exe --c-api-version 0.16.0 --exclude c-api --devices directml
```

### DirectML版

以下のDLL及びフォルダを格納してください。

* open_jtalk_dic_utf_8-1.11フォルダ
* modelフォルダ
* voicevox_core.dll
* voicevox_core.lib
* voicevox_onnxruntime.dll
* voicevox_onnxruntime.lib
* DirectML.dll

### CUDA版

以下のDLL及びフォルダを格納してください。

* open_jtalk_dic_utf_8-1.11フォルダ
* modelフォルダ
* voicevox_core.dll
* voicevox_core.lib
* voicevox_onnxruntime.dll
* voicevox_onnxruntime.lib
* voicevox_onnxruntime_providers_cuda.dll
* voicevox_onnxruntime_providers_cuda.lib
* voicevox_onnxruntime_providers_shared.dll
* voicevox_onnxruntime_providers_shared.lib
* cublas64_12.dll
* cublasLt64_12.dll
* cudart64_12.dll
* cudnn_adv_infer64_8.dll
* cudnn_cnn_infer64_8.dll
* cudnn_ops_infer64_8.dll
* cudnn64_8.dll
* cufft64_11.dll
* curand64_10.dll
