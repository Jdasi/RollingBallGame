// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RollingBallGame : ModuleRules
{
	public RollingBallGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore",
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"RollingBallGame",
			"RollingBallGame/Variant_Platforming",
			"RollingBallGame/Variant_Platforming/Animation",
			"RollingBallGame/Variant_Combat",
			"RollingBallGame/Variant_Combat/AI",
			"RollingBallGame/Variant_Combat/Animation",
			"RollingBallGame/Variant_Combat/Gameplay",
			"RollingBallGame/Variant_Combat/Interfaces",
			"RollingBallGame/Variant_Combat/UI",
			"RollingBallGame/Variant_SideScrolling",
			"RollingBallGame/Variant_SideScrolling/AI",
			"RollingBallGame/Variant_SideScrolling/Gameplay",
			"RollingBallGame/Variant_SideScrolling/Interfaces",
			"RollingBallGame/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
