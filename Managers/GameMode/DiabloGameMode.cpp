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
	FRotator Rot = FRotator(0);
	Rot.Yaw = 310.f;
	FVector Loc = FVector(7777.f);
	FActorSpawnParameters Param;
	Param.bNoFail=true;
	m_VisualActor = GetWorld()->SpawnActor<APlayerVisual>(APlayerVisual::StaticClass(),Loc,Rot,Param);
	m_VisualActor->HideMeshWithTick();
}

void ADiabloGameMode::SpawnOtherPVPActor()
{
	FRotator Rot = FRotator(0);
	Rot.Yaw = 310.f;
	FVector Loc = FVector(77777.f);
	FActorSpawnParameters Param;
	Param.bNoFail=true;
	m_OtherPlayer = GetWorld()->SpawnActor<AOtherPlayerPawn>(AOtherPlayerPawn::StaticClass(),Loc,Rot,Param);
	m_OtherPlayer->HideMeshWithTick();
	m_OtherPlayer->SetPetPositionForVisual();
	//
	UDiabloGameInstance::Get->m_PVPManager->m_OnMatchSuccessed.BindUObject(m_OtherPlayer,&AOtherPlayerPawn::SetPVPPlayerPawn);
}

void ADiabloGameMode::StartPlay()
{
	SpawnVisualActor();
	SpawnOtherPVPActor();
	
	Super::StartPlay();

	UDiabloGameInstance::Get->m_DungeonManager->LoadLevelComplete(this->GetWorld());
	m_PlManager = UDiabloGameInstance::Get->m_PlayfabManager;
	m_PlUpgrade = UDiabloGameInstance::Get->m_PlayerUpgradeManager;
	m_ChatManager = UDiabloGameInstance::Get->m_ChatManager;

	m_GoldManager	= UDiabloGameInstance::Get->m_GoldManager;

	AGameLevelHUD* MyHud = Cast<AGameLevelHUD>( UDiabloGameInstance::Get->GetPlCon()->GetHUD());
	MyHud->m_Canvas->m_OnMenuVisibleChanged.AddUObject(this,&ADiabloGameMode::OnMenuOpen);

	//Test
	//UDiabloGameInstance::Get->m_AdverManager->ShowInterstitialAds();
	//UDiabloGameInstance::Get->m_AdverManager->ShowRewardAds();

	
}

void ADiabloGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UDiabloGameInstance::Get->m_PVPManager->m_OnMatchSuccessed.Unbind();
}

void ADiabloGameMode::OnMenuOpen(bool b)
{
	if(b)
	{
		m_VisualActor->ShowMeshWithTick();
		//m_OtherPlayer->ShowMeshWithTick();
	}
	else
	{
		m_VisualActor->HideMeshWithTick();
		m_OtherPlayer->HideMeshWithTick();
	}
}

void ADiabloGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	m_PlManager->TickTryUpdateUserData(DeltaSeconds);
	m_PlUpgrade->Tick(DeltaSeconds);
	m_ChatManager->Tick(DeltaSeconds);

	m_OnTick.Broadcast(DeltaSeconds);
}

