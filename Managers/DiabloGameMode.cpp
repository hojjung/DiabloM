#include "DiabloGameMode.h"
#include "DiabloGameInstance.h"
#include "DungeonMiniMap.h"
#include "EngineUtils.h"
#include "GridFlowMiniMap.h"
#include "MonsterSpawnManager.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/DiabloPlayerController.h"
#include "Objs/Actor/DiaDungeon.h"
#include "Objs/Actor/DgMobSpawnPoint.h"
#include "Village/Portal.h"

ADiabloGameMode* ADiabloGameMode::Get=nullptr;

ADiabloGameMode::ADiabloGameMode()
{
	Get=this;
	PlayerControllerClass = ADiabloPlayerController::StaticClass();
	DefaultPawnClass = APlayerDiabloCharacter::StaticClass();

	m_ActionManager=CreateDefaultSubobject<UActionManagerComponent>("ActionManager");

	
}

void ADiabloGameMode::InitRewardManager()
{
	URewardManager* RewardManager = UDiabloGameInstance::Get->GetRewardManager();

	RewardManager->CreateActorPool();
}

void ADiabloGameMode::FindSpawnPoint()
{
	for (APortal* PlayerSpawnFind : TActorRange<APortal>(GetWorld()))
	{
		m_PlayerVillageSpawn=PlayerSpawnFind;
	}
}

void ADiabloGameMode::InitMinimap()
{
	m_MiniMap = NewObject<UGridFlowMiniMap>();
	m_MiniMap->m_World = ADiabloPlayerController::Get->GetWorld();
	m_MiniMap->OverlayIcons=m_AryOverlayMinimap;
	FDungeonMiniMapOverlayTracking TrackingInfo;
	TrackingInfo.TrackedActor = ADiabloPlayerController::Get->GetPlayerPawn();
	TrackingInfo.Id = "player";
	TrackingInfo.IconName = "player";
	TrackingInfo.bOrientToRotation = true;
	m_MiniMap->DynamicTracking.Add(TrackingInfo);
}

void ADiabloGameMode::StartPlay()
{
	SetDungeonInstanceToMap();

	InitDungeonInstances();//이방식의 문제점은 메모리 사용량 증가 //생각보다 적을지도 모른다

	FindSpawnPoint();

	InitRewardManager();
	
	
	
	//ㄴBeginPlay Before
	Super::StartPlay();

	InitSpawnManager();

	InitMinimap();
	//

	
}

void ADiabloGameMode::SetDungeonInstanceToMap()
{
	m_MapDungeonActors.Reset();
	
	for (ADiaDungeon* DungeonInst : TActorRange<ADiaDungeon>(GetWorld()))
	{
		m_MapDungeonActors.Emplace(DungeonInst->m_LevelName,DungeonInst);
	}
}

void ADiabloGameMode::InitDungeonInstances()
{
	for (ADgMobSpawnPoint* SpawnPointInWorld : TActorRange<ADgMobSpawnPoint>(GetWorld()))
	{
		for(auto& DungeonActor :m_MapDungeonActors)
		{
			if(DungeonActor.Value->IsMyActor(SpawnPointInWorld))
			{
				DungeonActor.Value->AddSpawnPoints(SpawnPointInWorld);
				break; //found the master
			}
		}
	}

	for (AActor* AllActor : TActorRange<ADgMobSpawnPoint>(GetWorld()))
	{
		for(auto& DungeonActor :m_MapDungeonActors)
		{
			if(DungeonActor.Value->IsMyActor(AllActor))
			{
				DungeonActor.Value->AddMyActors(AllActor);
				break; //found the master
			}
		}
	}

	for(auto& DungeonActor :m_MapDungeonActors)
	{
		DungeonActor.Value->ShuffleSpawnPoints();
		DungeonActor.Value->HideDungeon();
	}
}

void ADiabloGameMode::InitSpawnManager()
{
	UMonsterSpawnManager* SpawnManager = UDiabloGameInstance::Get->GetMonsterSpawn();

	SpawnManager->UpdateWorld(GetWorld());
}

APortal* ADiabloGameMode::GetSpawnPoint()
{
	return m_PlayerVillageSpawn;
}

void ADiabloGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	m_OnTick.Broadcast(DeltaSeconds);
	m_MiniMap->CustomTick(DeltaSeconds);
}


ADiaDungeon* ADiabloGameMode::GetDungeon(FName id)
{
	return m_MapDungeonActors[id]; //use dungeon location to Player Start?
}
