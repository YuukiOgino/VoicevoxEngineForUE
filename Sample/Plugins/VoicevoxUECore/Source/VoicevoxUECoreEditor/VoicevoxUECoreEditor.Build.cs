// Copyright Yuuki Ogino. All Rights Reserved.

using UnrealBuildTool;

public class VoicevoxUECoreEditor : ModuleRules
{
    public VoicevoxUECoreEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "UMG",
                "UMGEditor",
                "VoicevoxUECore",
                "Blutility",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "UnrealEd",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DesktopPlatform",
                "AudioMixer",
                "Json",
                "JsonUtilities"                
            }
        );
    }
}