// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class VoicevoxNativeNemoCore : ModuleRules
{
	/// <summary>
	/// Open JTalkのライブラリフォルダ名
	/// </summary>
	private const string OpenJtalkDicName = "open_jtalk_dic_utf_8-1.11";
	
	public VoicevoxNativeNemoCore(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		CppStandard = CppStandardVersion.Latest;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			const string platformName = "x64";
			const string binPlatformName = "Win64";
			
			// core.hはEngine側も同名のヘッダーファイルがあるため、意図的にx64フォルダまでをIncludePathに含める
			PublicSystemIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, platformName, "Nemo")));
			
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, platformName, "Nemo", "voicevox_core.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("voicevox_core.dll");
			PublicDelayLoadDLLs.Add("onnxruntime.dll");

			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/Nemo/voicevox_core.dll", Path.Combine(ModuleDirectory, platformName, "Nemo", "voicevox_core.dll"));
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/Nemo/onnxruntime.dll", Path.Combine(ModuleDirectory, platformName,  "Nemo", "onnxruntime.dll"));
			
			// onnxruntimeのCUDE関連DLLが存在する場合は必要なDLL一式すべてコピーする
			var cudaPath = Path.Combine(ModuleDirectory, platformName,  "Nemo", "onnxruntime_providers_cuda.dll");
			if (File.Exists(cudaPath))
			{
				var cudaDllList = new[]
				{
					"onnxruntime_providers_cuda.dll",
					"onnxruntime_providers_shared.dll",
					"onnxruntime_providers_tensorrt.dll",
					"cublas64_11.dll",
					"cublasLt64_11.dll",
					"cudart64_110.dll",
					"cudnn_adv_infer64_8.dll",
					"cudnn_cnn_infer64_8.dll",
					"cudnn_ops_infer64_8.dll",
					"cudnn64_8.dll",
					"cufft64_10.dll",
					"curand64_10.dll"
				};
				foreach (var variable in cudaDllList)
				{
					var path = Path.Combine(ModuleDirectory, platformName, variable);
					if (File.Exists(path))
					{
						PublicDelayLoadDLLs.Add(variable);
						RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/Nemo/{variable}", path);
					}
				}
			}
			
			// DirectML.dllが存在する場合はコピーする
			var directMlPath = Path.Combine(ModuleDirectory, platformName,  "Nemo",  "DirectML.dll");
			if (File.Exists(directMlPath))
			{
				PublicDelayLoadDLLs.Add("DirectML.dll");
				RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/Nemo/DirectML.dll", directMlPath);
			}
			
			// Open JTalkライブラリフォルダもコピーする
			//AddRuntimeDependenciesDirectory(OpenJtalkDicName, platformName, binPlatformName, true);
			// modelフォルダもコピーする
			AddRuntimeDependenciesDirectory("model", $"{platformName}/Nemo", $"{binPlatformName}/Nemo", true);
        }
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			const string platformName = "osx";
			const string binPlatformName = "Mac";
			
			// core.hはEngine側も同名のヘッダーファイルがあるため、意図的にx64フォルダまでをIncludePathに含める
			PublicSystemIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, platformName)));
			
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, platformName, "libvoicevox_core.dylib"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, platformName, "libonnxruntime.1.13.1.dylib"));
			
			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/libvoicevox_core.dylib", Path.Combine(ModuleDirectory, platformName, "libvoicevox_core.dylib"));
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{binPlatformName}/libonnxruntime.1.13.1.dylib", Path.Combine(ModuleDirectory, platformName, "libonnxruntime.1.13.1.dylib"));
			
			// Open JTalkライブラリフォルダもコピーする
			//AddRuntimeDependenciesDirectory(OpenJtalkDicName, platformName, binPlatformName, true);
			// modelフォルダもコピーする
			AddRuntimeDependenciesDirectory("model", platformName, binPlatformName, true);
		}
		
		PublicDefinitions.Add($"OPEN_JTALK_DIC_NAME=\"{OpenJtalkDicName}\"");
	}

	/// <summary>
	/// 指定したディレクトリをRuntimeDependenciesに登録
	/// </summary>
	/// <param name="SourceDirName">コピー元のディレクトリ名</param>
	/// <param name="Platform">プラグイン側のプラットフォームフォルダ名</param>
	/// <param name="BinPlatform">出力バイナリのプラットフォームフォルダ名</param>
	/// <param name="CopySubDirs">サブディレクトリもコピーを行うか</param>
	/// <exception cref="DirectoryNotFoundException"></exception>
	private void AddRuntimeDependenciesDirectory(string SourceDirName, string Platform, string BinPlatform,  bool CopySubDirs)
	{
		var Info = new DirectoryInfo(Path.Combine(ModuleDirectory, Platform, SourceDirName));
		if (!Info.Exists)
		{
			throw new DirectoryNotFoundException($"The specified directory cannot be found: {SourceDirName}");
		}
		
		var Infos = Info.GetDirectories();
		var Files = Info.GetFiles();
		
		foreach (var F in Files)
		{
			RuntimeDependencies.Add($"$(ProjectDir)/Binaries/{BinPlatform}/{SourceDirName}/{F.Name}", Path.Combine(ModuleDirectory, Platform, SourceDirName, F.Name));
		}
		
		if (!CopySubDirs) return;
		
		foreach (var SubDir in Infos)
		{
			var TempPath = Path.Combine(SourceDirName, SubDir.Name);
			AddRuntimeDependenciesDirectory(TempPath, Platform, BinPlatform, true);
		}
	}
}
