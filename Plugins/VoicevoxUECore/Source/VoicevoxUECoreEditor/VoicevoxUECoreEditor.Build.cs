// Copyright Yuuki Ogino. All Rights Reserved.

using UnrealBuildTool;

public class VoicevoxUECoreEditor : ModuleRules
{
    public VoicevoxUECoreEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Latest;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "UMG",
                "UMGEditor",
                "VoicevoxUECore"
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