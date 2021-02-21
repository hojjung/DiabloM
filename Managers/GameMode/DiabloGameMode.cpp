#include "DiabloGameMode.h"
#include "DiaGameState.h"
#include "DiaPlayerState.h"
#include "EngineUtils.h"
#include "MoviePlayer.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/StartMap/PlayerVisual.h"
#include "GameFramework/PlayerState.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/MyHUD.h"

ADiabloGameMode::ADiabloGameMode()
{
	PlayerControllerClass = ADiabloPlayerController::StaticClass();
	DefaultPawnClass = APlayerDiabloCharacter::StaticClass();
	GameStateClass = ADiaGameState::StaticClass();
	PlayerStateClass = ADiaPlayerState::StaticClass();
	HUDClass = AMyHUD::StaticClass();

	m_PlayerActionManager=CreateDefaultSubobject<UActionManagerComponent>("PlayerActionManager");
}


void ADiabloGameMode::StartPlay()
{
	Super::StartPlay();

	PRINTF("GameModeStartPlay");
}

void ADiabloGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//Login(ADiabloPlayerController::Get,ENetRole::ROLE_Authority,);
	//Logout(ADiabloPlayerController::Get);
	PRINTF("LogoutNeed");
}

void ADiabloGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


