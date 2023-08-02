// Copyright Yuuki Ogino. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;
using Console = Internal.Console;

/// <summary>
/// VOICEVOX COREライブラリモジュールクラス
/// </summary>
public class VoicevoxCore : ModuleRules
{
	/// <summary>
	/// Open JTalkのライブラリフォルダ名
	/// </summary>
	private const string OpenJtalkDicName = "open_jtalk_dic_utf_8-1.11";

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="Target"></param>
	public VoicevoxCore(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		CppStandard = CppStandardVersion.Latest;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// core.hはEngine側も同名のヘッダーファイルがあるため、意図的にx64フォルダまでをIncludePathに含める
			PublicSystemIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "x64")));
			
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "VoicevoxCore", "voicevox_core.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("voicevox_core.dll");
			PublicDelayLoadDLLs.Add("onnxruntime.dll");
			PublicDelayLoadDLLs.Add("onnxruntime_providers_shared.dll");

			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add("$(ProjectDir)/Binaries/Win64/voicevox_core.dll", Path.Combine(ModuleDirectory, "x64", "VoicevoxCore", "voicevox_core.dll"));
			RuntimeDependencies.Add("$(ProjectDir)/Binaries/Win64/onnxruntime.dll", Path.Combine(ModuleDirectory, "x64", "VoicevoxCore", "onnxruntime.dll"));
			RuntimeDependencies.Add("$(ProjectDir)/Binaries/Win64/onnxruntime_providers_shared.dll", Path.Combine(ModuleDirectory, "x64", "VoicevoxCore", "onnxruntime_providers_shared.dll"));
			
			// Open JTalkライブラリフォルダもコピーする
			DeleteDirectory($"{ProjectDirectory}/Binaries/Win64/{OpenJtalkDicName}");
			DirectoryCopy(Path.Combine(ModuleDirectory, "x64", "VoicevoxCore", OpenJtalkDicName), $"{ProjectDirectory}/Binaries/Win64/{OpenJtalkDicName}", true);
			
			// modelフォルダもコピーする
			DeleteDirectory($"{ProjectDirectory}/Binaries/Win64/model");
			DirectoryCopy(Path.Combine(ModuleDirectory, "x64", "VoicevoxCore", "model"), $"{ProjectDirectory}/Binaries/Win64/model", true);
        }
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			// core.hはEngine側も同名のヘッダーファイルがあるため、意図的にx64フォルダまでをIncludePathに含める
			PublicSystemIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "osx")));
			
			// Delay-load the DLL, so we can load it from the right place first
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "osx", "VoicevoxCore", "libvoicevox_core.dylib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "osx", "VoicevoxCore", "libonnxruntime.1.13.1.dylib"));
			
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "osx", "VoicevoxCore", "libvoicevox_core.dylib"));
			PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "osx", "VoicevoxCore", "libonnxruntime.1.13.1.dylib"));
			
			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add("$(ProjectDir)/Binaries/Mac/libvoicevox_core.dylib", Path.Combine(ModuleDirectory, "osx", "VoicevoxCore", "libvoicevox_core.dylib"));
			RuntimeDependencies.Add("$(ProjectDir)/Binaries/Mac/libonnxruntime.1.13.1.dylib", Path.Combine(ModuleDirectory, "osx", "VoicevoxCore", "libonnxruntime.1.13.1.dylib"));
			
			// Open JTalkライブラリフォルダもコピーする
			DeleteDirectory($"{ProjectDirectory}/Binaries/Mac/{OpenJtalkDicName}");
			DirectoryCopy(Path.Combine(ModuleDirectory, "osx", "VoicevoxCore", OpenJtalkDicName), $"{ProjectDirectory}/Binaries/Mac/{OpenJtalkDicName}", true);
			
			// modelフォルダもコピーする
			DeleteDirectory($"{ProjectDirectory}/Binaries/Mac/model");
			DirectoryCopy(Path.Combine(ModuleDirectory, "osx", "VoicevoxCore", "model"), $"{ProjectDirectory}/Binaries/Mac/model", true);
		}
		
		PublicDefinitions.Add($"OPEN_JTALK_DIC_NAME=\"{OpenJtalkDicName}\"");
	}
	
	/// <summary>
	/// プロジェクトディレクトリ名を取得
	/// </summary>
	private static string ProjectDirectory
	{
		get
		{
			foreach (var commandLineArg in Environment.GetCommandLineArgs())
			{
				var nameAndValues = new List<string>(commandLineArg.Split('=', StringSplitOptions.RemoveEmptyEntries));
				if (nameAndValues.Count < 2)
				{
					continue;
				}
				if (nameAndValues[0] != "-Project")
				{
					continue;
				}
				nameAndValues.RemoveAt(0);
				var uProjectFilePath = string.Join("", nameAndValues.ToArray());
				return Path.GetDirectoryName(uProjectFilePath);
			}

			return null;
		}
	}

	/// <summary>
	/// 指定したディレクトリのコピー実行
	/// </summary>
	/// <param name="SourceDirName">コピー元のディレクトリ</param>
	/// <param name="DestDirName">コピー先のディレクトリ</param>
	/// <param name="CopySubDirs">サブディレクトリもコピーを行うか</param>
	/// <exception cref="DirectoryNotFoundException"></exception>
	private static void DirectoryCopy(string SourceDirName, string DestDirName, bool CopySubDirs)
	{
		Console.WriteLine(SourceDirName);
		Console.WriteLine(DestDirName);
		
		var dir = new DirectoryInfo(SourceDirName);

		if (!dir.Exists)
		{
			throw new DirectoryNotFoundException($"The specified directory cannot be found: {SourceDirName}");
		}

		var dirs = dir.GetDirectories();
		Directory.CreateDirectory(DestDirName);
		
		var files = dir.GetFiles();
		foreach (var f in files)
		{
			var tempPath = Path.Combine(DestDirName, f.Name);
			f.CopyTo(tempPath, true);
		}
		
		if (!CopySubDirs) return;
		
		foreach (var subDir in dirs)
		{
			var tempPath = Path.Combine(DestDirName, subDir.Name);
			DirectoryCopy(subDir.FullName, tempPath, true);
		}
	}
	
	/// <summary>
	/// 指定したディレクトリを削除
	/// </summary>
	/// <param name="TargetDirectoryPath">削除したいディレクトリ</param>
	private static void DeleteDirectory(string TargetDirectoryPath)
	{
		if (!Directory.Exists(TargetDirectoryPath))
		{
			return;
		}

		//ディレクトリ以外の全ファイルを削除
		var filePaths = Directory.GetFiles(TargetDirectoryPath);
		foreach (var filePath in filePaths)
		{
			File.SetAttributes(filePath, FileAttributes.Normal);
			File.Delete(filePath);
		}

		//ディレクトリの中のディレクトリも再帰的に削除
		var directoryPaths = Directory.GetDirectories(TargetDirectoryPath);
		foreach (var directoryPath in directoryPaths)
		{
			DeleteDirectory(directoryPath);
		}

		//中が空になったらディレクトリ自身も削除
		Directory.Delete(TargetDirectoryPath, false);
	}		
	
}
