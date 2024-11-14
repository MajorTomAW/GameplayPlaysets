﻿using UnrealBuildTool;

public class PlaysetsEditor : ModuleRules
{
    public PlaysetsEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "GameplayPlaysets",
                "AssetDefinition",
                "EditorStyle",
                "Projects",
                "UnrealEd",
                "NiagaraEditor",
                "ToolWidgets",
                "InputCore",
                "StateTreeEditorModule",
            }
        );
    }
}