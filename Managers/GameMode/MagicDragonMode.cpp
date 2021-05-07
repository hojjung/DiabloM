#include "MagicDragonMode.h"
#include "Characters/DiabloPlayerController.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/HUD/MagicDgHUD.h"

AMagicDragonMode::AMagicDragonMode()
{
	PlayerControllerClass = ADiabloPlayerController::StaticClass();
	DefaultPawnClass = APlayerDiabloCharacter::StaticClass();
	HUDClass = AMagicDgHUD::StaticClass();
	
	PrimaryActorTick.bCanEverTick = true;
}

void AMagicDragonMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UDiabloGameInstance::Get->m_MagicDgManager->Tick(DeltaSeconds);
	
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->Tick(DeltaSeconds);
}
