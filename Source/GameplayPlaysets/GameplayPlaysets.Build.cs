// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameplayPlaysets : ModuleRules
{
	public GameplayPlaysets(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"GameplayTags",
		});
			
		
		PrivateDependencyModuleNames.AddRange(new[]
		{
			"CoreUObject",
			"Engine",
		});
	}
}
