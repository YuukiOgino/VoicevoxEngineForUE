// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VoicevoxNativeCoreNemo : ModuleRules
{
	public VoicevoxNativeCoreNemo(ReadOnlyTargetRules Target) : base(Target)
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
				"VoicevoxCoreNemo",
				"VoicevoxUECore",　
				"Json",
				"JsonUtilities"
			});
		
		PrivateDefinitions.Add($"VOICEVOX_NEMO_CORE_NAME=\"NEMO_CORE\"");
	}
}
