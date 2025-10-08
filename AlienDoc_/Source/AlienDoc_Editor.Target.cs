using UnrealBuildTool;
using System.Collections.Generic;

public class AlienDoc__EditorTarget : TargetRules
{
	public AlienDoc__EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		ExtraModuleNames.AddRange(new string[] { "AlienDocGame", "AlienDocGeminiVA" });
	}
}


