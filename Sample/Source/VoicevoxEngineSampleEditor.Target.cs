// Copyright Yuuki Ogino. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class VoicevoxEngineSampleEditorTarget : TargetRules
{
	public VoicevoxEngineSampleEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
#if UE_5_4_OR_LATER
		bOverrideBuildEnvironment = true;
#endif
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "VoicevoxEngineSample" } );
	}
}
