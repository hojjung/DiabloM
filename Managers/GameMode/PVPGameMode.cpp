#include "PVPGameMode.h"
#include "Characters/DiabloPlayerController.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/HUD/GameLevelHUD.h"
#include "Widgets/HUD/PVP_HUD.h"

APVPGameMode::APVPGameMode()
{
	HUDClass = APVP_HUD::StaticClass();
}

