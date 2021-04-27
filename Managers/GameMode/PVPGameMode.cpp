// My First Hack n Slash


#include "PVPGameMode.h"

#include "DiaGameState.h"
#include "DiaPlayerState.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/OtherPlayerPawn.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/GameLevelHUD.h"
#include "Widgets/PVP_HUD.h"

APVPGameMode::APVPGameMode()
{
	PlayerControllerClass = ADiabloPlayerController::StaticClass();
	DefaultPawnClass = APlayerDiabloCharacter::StaticClass();
	GameStateClass = ADiaGameState::StaticClass();
	PlayerStateClass = ADiaPlayerState::StaticClass();
	HUDClass = APVP_HUD::StaticClass();

	//m_PlayerActionManager=CreateDefaultSubobject<UActionManagerComponent>("PlayerActionManager");

	PrimaryActorTick.bCanEverTick = true;

	
	//(X=947.000000,Y=1131.000000,Z=102.000000)
	//(Pitch=0.000000,Yaw=-140.000183,Roll=0.000000)
}

void APVPGameMode::SpawnOtherPlayerPawn()
{
	FVector Loc = FVector(947,1131,102);
	
	FRotator Rot = FRotator(0,130,0);
	
	FActorSpawnParameters Param;
	
	Param.bNoFail=true;
	
	m_OtherPlayer = GetWorld()->SpawnActor<AOtherPlayerPawn>(AOtherPlayerPawn::StaticClass(),Loc,Rot,Param);
	//
	m_OtherPlayer->SetPVPPlayerPawn(UDiabloGameInstance::Get->m_PVPManager->m_StatObj,UDiabloGameInstance::Get->m_PVPManager->m_EquipObj);
}

void APVPGameMode::StartPlay()
{
	Super::StartPlay();

	SpawnOtherPlayerPawn();
}

void APVPGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
}
