// Shooter Project Game. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ShooterEditorTarget : TargetRules
{
	public ShooterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Shooter" } );
	}
}
