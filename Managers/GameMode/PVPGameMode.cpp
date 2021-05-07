#include "PVPGameMode.h"
#include "Characters/DiabloPlayerController.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/HUD/GameLevelHUD.h"
#include "Widgets/HUD/PVP_HUD.h"

APVPGameMode::APVPGameMode()
{
	PlayerControllerClass = ADiabloPlayerController::StaticClass();
	DefaultPawnClass = APlayerDiabloCharacter::StaticClass();
	HUDClass = APVP_HUD::StaticClass();
	//m_PlayerActionManager=CreateDefaultSubobject<UActionManagerComponent>("PlayerActionManager");
	PrimaryActorTick.bCanEverTick = true;
	//(X=947.000000,Y=1131.000000,Z=102.000000)
	//(Pitch=0.000000,Yaw=-140.000183,Roll=0.000000)
}

void APVPGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UDiabloGameInstance::Get->m_PVPManager->Tick(DeltaSeconds);
	
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->Tick(DeltaSeconds);
}
