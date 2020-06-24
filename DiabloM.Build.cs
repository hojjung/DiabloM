// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class DiabloM : ModuleRules
{
    public DiabloM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "AIModule",
            "NavigationSystem",
            "DialoguePlugin",
            "DungeonArchitectRuntime",
            "TechTree"
        });

        

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore",
         "AIModule",
            "NavigationSystem",
            "DialoguePlugin",
            "DungeonArchitectRuntime",
            "TechTree",
            "OnlineSubsystem"
        });

    }

}
