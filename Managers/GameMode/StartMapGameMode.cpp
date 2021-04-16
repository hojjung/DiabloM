#include "StartMapGameMode.h"
#include "PlayFabServerDataModels.h"
#include "Core/PlayFabClientAPI.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"
#include "Widgets/InitLevelHUD.h"

AStartMapGameMode::AStartMapGameMode()
{
	HUDClass = AInitLevelHUD::StaticClass();

}

void AStartMapGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(this,0)->bShowMouseCursor=true;
	
}
