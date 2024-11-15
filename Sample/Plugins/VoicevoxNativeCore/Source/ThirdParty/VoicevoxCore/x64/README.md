# VOICEVOX CORE、Open JTalk、ONNX Runtimeをこのフォルダに配置してください。

- 格納例<br/>
![image](https://github.com/user-attachments/assets/ed65d70c-95dc-464f-af12-f1bec9537fef)

## CPUモードの場合

以下のDLL及びフォルダを格納してください。

* open_jtalk_dic_utf_8-1.11フォルダ
* modelフォルダ
* voicevox_core.dll
* voicevox_core.lib
* onnxruntime.dll
* onnxruntime_providers_shared.dll

## GPUモードの場合

> [!NOTE]
> GPU起動に必要なDLLはダウンローダー以外では入手できません。必ずダウンローダーからVOICEVOX COREを入手してください。

ダウンローダー　DirectML版コマンド例
```PowerShell
Invoke-WebRequest https://github.com/VOICEVOX/voicevox_core/releases/latest/download/download-windows-x64.exe -OutFile ./download.exe
./download.exe -v 0.14.6 --device directml
```

### DirectML版

以下のDLL及びフォルダを格納してください。

* open_jtalk_dic_utf_8-1.11フォルダ
* modelフォルダ
* voicevox_core.dll
* voicevox_core.lib
* onnxruntime.dll
* onnxruntime_providers_shared.dll
* DirectML.dll

### CUDA版

以下のDLL及びフォルダを格納してください。

* open_jtalk_dic_utf_8-1.11フォルダ
* modelフォルダ
* voicevox_core.dll
* voicevox_core.lib
* onnxruntime.dll
* onnxruntime_providers_shared.dll
* onnxruntime_providers_cuda.dll
* onnxruntime_providers_shared.dll
* onnxruntime_providers_tensorrt.dll
* cublas64_11.dll
* cublasLt64_11.dll
* cudart64_110.dll
* cudnn_adv_infer64_8.dll
* cudnn_cnn_infer64_8.dll
* cudnn_ops_infer64_8.dll
* cudnn64_8.dll
* cufft64_10.dll
* curand64_10.dll
