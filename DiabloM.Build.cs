// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class DiabloM : ModuleRules
{
    public DiabloM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
          
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "AIModule",
            "NavigationSystem",
            "DialoguePlugin",
            "DungeonArchitectRuntime",
            "TechTree"
           
        });

        

        PrivateDependencyModuleNames.AddRange(new string[] { 
            "Slate", "SlateCore","UMG",
            "AIModule",
            "NavigationSystem",
            "DialoguePlugin",
            "DungeonArchitectRuntime",
            "TechTree",
            "OnlineSubsystem"
           
        });

    }

}
