// Shooter Project Game. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ShooterTarget : TargetRules
{
	public ShooterTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Shooter" } );
	}
}
