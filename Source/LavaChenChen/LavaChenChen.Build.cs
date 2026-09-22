// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LavaChenChen : ModuleRules
{
	public LavaChenChen(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"LavaChenChen",
			"LavaChenChen/Variant_Platforming",
			"LavaChenChen/Variant_Platforming/Animation",
			"LavaChenChen/Variant_Combat",
			"LavaChenChen/Variant_Combat/AI",
			"LavaChenChen/Variant_Combat/Animation",
			"LavaChenChen/Variant_Combat/Gameplay",
			"LavaChenChen/Variant_Combat/Interfaces",
			"LavaChenChen/Variant_Combat/UI",
			"LavaChenChen/Variant_SideScrolling",
			"LavaChenChen/Variant_SideScrolling/AI",
			"LavaChenChen/Variant_SideScrolling/Gameplay",
			"LavaChenChen/Variant_SideScrolling/Interfaces",
			"LavaChenChen/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
