// Copyright Yuuki Ogino. All Rights Reserved.

using UnrealBuildTool;

public class VoicevoxEngineEditor : ModuleRules
{
    public VoicevoxEngineEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Latest;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Blutility",
                "UMG",
                "UMGEditor"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "VoicevoxUECore",
                "VoicevoxUECoreEditor",
                "VoicevoxEngine",
                "UnrealEd",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DesktopPlatform",
                "Json",
                "JsonUtilities"
            }
        );
    }
}