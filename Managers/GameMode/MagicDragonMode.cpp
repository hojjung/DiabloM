// My First Hack n Slash


#include "MagicDragonMode.h"

#include "DiaGameState.h"
#include "DiaPlayerState.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"

AMagicDragonMode::AMagicDragonMode()
{
	PlayerControllerClass = ADiabloPlayerController::StaticClass();
	DefaultPawnClass = APlayerDiabloCharacter::StaticClass();
	GameStateClass = ADiaGameState::StaticClass();
	PlayerStateClass = ADiaPlayerState::StaticClass();
	//HUDClass = APVP_HUD::StaticClass();

	//m_PlayerActionManager=CreateDefaultSubobject<UActionManagerComponent>("PlayerActionManager");

	PrimaryActorTick.bCanEverTick = true;
}

void AMagicDragonMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UDiabloGameInstance::Get->m_MagicDgManager->Tick(DeltaSeconds);
	
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->Tick(DeltaSeconds);
}
