// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VoicevoxNemoCore : ModuleRules
{
	public VoicevoxNemoCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Latest;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
			});
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"CoreUObject",
				"VoicevoxNativeNemoCore",
				"VoicevoxUECore",　
				"Json",
				"JsonUtilities"
			});
		
		PrivateDefinitions.Add($"VOICEVOX_NEMO_CORE_NAME=\"NEMO_CORE\"");
	}
}
