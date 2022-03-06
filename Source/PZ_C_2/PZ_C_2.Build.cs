// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PZ_C_2 : ModuleRules
{
	public PZ_C_2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			// Core
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",

			// UI
			"UMG",

			// GAS
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {"Slate", "SlateCore"});
	}
}