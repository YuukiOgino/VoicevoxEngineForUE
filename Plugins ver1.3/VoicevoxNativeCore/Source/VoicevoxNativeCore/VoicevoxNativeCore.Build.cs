// Copyright Yuuki Ogino. All Rights Reserved.

using UnrealBuildTool;

public class VoicevoxNativeCore : ModuleRules
{
	public VoicevoxNativeCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Latest;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"CoreUObject",
				"VoicevoxCore",
				"VoicevoxUECore"
			}
		);
		
		PrivateDefinitions.Add($"VOICEVOX_CORE_NAME=\"CORE\"");
	}
}
