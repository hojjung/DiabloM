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
#include "Managers/DungeonManager.h"
#include "Widgets/GameLevelHUD.h"
#include "Widgets/MainCanvas.h"
#include "Widgets/MyHUD.h"

ADiabloGameMode::ADiabloGameMode()
{
	PlayerControllerClass = ADiabloPlayerController::StaticClass();
	DefaultPawnClass = APlayerDiabloCharacter::StaticClass();
	GameStateClass = ADiaGameState::StaticClass();
	PlayerStateClass = ADiaPlayerState::StaticClass();
	HUDClass = AGameLevelHUD::StaticClass();

	m_PlayerActionManager=CreateDefaultSubobject<UActionManagerComponent>("PlayerActionManager");

	PrimaryActorTick.bCanEverTick = true;

	m_bGoldOfflineLock=  false;
}


void ADiabloGameMode::SpawnVisualActor()
{
	FVector Loc = FVector(7777.f);
	FActorSpawnParameters Param;
	Param.bNoFail=true;
	m_VisualActor = GetWorld()->SpawnActor<APlayerVisual>(APlayerVisual::StaticClass(),Loc,FRotator(0.f),Param);
	m_VisualActor->HideMeshWithTick();
}

void ADiabloGameMode::StartPlay()
{
	SpawnVisualActor();
	
	Super::StartPlay();

	UDiabloGameInstance::Get->m_DungeonManager->LoadLevelComplete(this->GetWorld());
	m_PlManager = UDiabloGameInstance::Get->m_PlayfabManager;
	m_PlUpgrade = UDiabloGameInstance::Get->m_PlayerUpgradeManager;
	m_ChatManager = UDiabloGameInstance::Get->m_ChatManager;

	UDiabloGameInstance::Get->GetPlCon()->m_OnTick.AddUObject(m_PlManager, &UPlayfabManager::TickTryUpdateUserData);
	UDiabloGameInstance::Get->GetPlCon()->m_OnTick.AddUObject(m_PlUpgrade, &UPlayerUpgradeManager::Tick);
	UDiabloGameInstance::Get->GetPlCon()->m_OnTick.AddUObject(m_ChatManager, &UChatManager::Tick);

	m_GoldManager	= UDiabloGameInstance::Get->m_GoldManager;

	AGameLevelHUD* MyHud = Cast<AGameLevelHUD>( UDiabloGameInstance::Get->GetPlCon()->GetHUD());
	MyHud->m_Canvas->m_OnMenuVisibleChanged.AddUObject(this,&ADiabloGameMode::OnMenuOpen);

	//UDiabloGameInstance::Get->m_GoldManager->GainOfflineGold();

	
}

void ADiabloGameMode::OnMenuOpen(bool b)
{
	if(b)
	{
		m_VisualActor->ShowMeshWithTick();
	}
	else
	{
		m_VisualActor->HideMeshWithTick();
	}
}

void ADiabloGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!m_bGoldOfflineLock&&m_GoldManager->GetIsServerTimeGained())
	{
		m_bGoldOfflineLock=true;

		if(!m_GoldManager->GainOfflineGold())
		{
			return;
		}
		
		AGameLevelHUD* GameLevelHUD = Cast<AGameLevelHUD>( UDiabloGameInstance::Get->GetPlCon()->GetHUD());

		GameLevelHUD->ShowOfflineGoldWindow(m_GoldManager->GetFinalOfflineGold());
		
		SetActorTickEnabled(false);
	}
}

