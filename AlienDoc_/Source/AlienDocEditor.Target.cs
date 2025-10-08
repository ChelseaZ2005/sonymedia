using UnrealBuildTool;
using System.Collections.Generic;

public class AlienDocEditorTarget : TargetRules
{
	public AlienDocEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.AddRange(new string[] { "AlienDocGame", "AlienDocGeminiVA" });
	}
}


