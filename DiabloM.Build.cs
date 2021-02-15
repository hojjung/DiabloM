// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
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
            "OnlineSubsystem", "OnlineSubsystemUtils",
            "MoviePlayer"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { 
            "Slate", "SlateCore","UMG",
            "Core", "CoreUObject", "Engine",
            "AIModule",
            "NavigationSystem",
            "DialoguePlugin",
            "DungeonArchitectRuntime",
            "TechTree",
            "OnlineSubsystem","OnlineSubsystemUtils",
            "EasyActionPlugin",
            "PlayFab",
            "PlayFabCpp",
            "PlayFabCommon",
            "MobileUtils"
        });

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.Add("OnlineSubsystemGooglePlay");
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "Android_UPL.xml"));
            PrivateDependencyModuleNames.Add("AndroidAdvertising");
        }
        
    }

}
