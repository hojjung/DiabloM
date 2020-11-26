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
           "DialoguePlugin",
           "DungeonArchitectRuntime",
           "TechTree",
            "NavigationSystem",
            "EasyActionPlugin",
            "RenderCore",
            "MoviePlayer",
        });

        

        PrivateDependencyModuleNames.AddRange(new string[] { 
            "Slate", "SlateCore","UMG",
            "Core", "CoreUObject", "Engine",
            "AIModule",
            "NavigationSystem",
            "DialoguePlugin",
            "DungeonArchitectRuntime",
            "TechTree",
            "OnlineSubsystem",
            "EasyActionPlugin",
        });
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            //PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "OnlineSubsystem" });
            DynamicallyLoadedModuleNames.Add("OnlineSubsystemGooglePlay");
        }
    }

}
