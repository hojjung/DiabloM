#include "MagicDragonMode.h"
#include "Characters/DiabloPlayerController.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/HUD/MagicDgHUD.h"

AMagicDragonMode::AMagicDragonMode()
{
	HUDClass = AMagicDgHUD::StaticClass();
}

