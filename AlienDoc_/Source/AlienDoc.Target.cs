using UnrealBuildTool;
using System.Collections.Generic;

public class AlienDocTarget : TargetRules
{
	public AlienDocTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.AddRange(new string[] { "AlienDocGame", "AlienDocGeminiVA" });
	}
}


