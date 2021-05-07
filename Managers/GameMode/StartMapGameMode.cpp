#include "StartMapGameMode.h"
#include "PlayFabServerDataModels.h"
#include "Core/PlayFabClientAPI.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/HUD/InitLevelHUD.h"

AStartMapGameMode::AStartMapGameMode()
{
	HUDClass = AInitLevelHUD::StaticClass();
}

void AStartMapGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(this,0)->bShowMouseCursor=true;
}
