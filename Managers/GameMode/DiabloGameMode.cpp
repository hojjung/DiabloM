#include "DiabloGameMode.h"
#include "EngineUtils.h"
#include "MoviePlayer.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/StartMap/PlayerVisual.h"
#include "GameFramework/PlayerState.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/HUD/GameLevelHUD.h"
#include "Widgets/HUD/MainCanvas.h"
#include "Widgets/HUD/MyHUD.h"

ADiabloGameMode::ADiabloGameMode()
{
	PlayerControllerClass = ADiabloPlayerController::StaticClass();
	DefaultPawnClass = APlayerDiabloCharacter::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
}

void ADiabloGameMode::StartPlay()
{
	Super::StartPlay();

	m_DungeonManager= UDiabloGameInstance::Get->m_DungeonManager;
}

void ADiabloGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ADiabloGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	m_DungeonManager->Tick(DeltaSeconds);
}

