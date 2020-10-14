#include "DiabloGameMode.h"
#include "DiabloGameInstance.h"
#include "EngineUtils.h"
#include "MonsterSpawnManager.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/DiabloPlayerController.h"
#include "Objs/Actor/DiaDungeon.h"
#include "Objs/Actor/DgMobSpawnPoint.h"

ADiabloGameMode* ADiabloGameMode::Get=nullptr;

ADiabloGameMode::ADiabloGameMode()
{
	Get=this;
	PlayerControllerClass = ADiabloPlayerController::StaticClass();
	DefaultPawnClass = APlayerDiabloCharacter::StaticClass();
}

void ADiabloGameMode::StartPlay()
{
	SetDungeonInstanceToMap();

	InitDungeonInstances();//이방식의 문제점은 메모리 사용량 증가 //생각보다 적을지도 모른다

	//ㄴBeginPlay Before
	
	Super::StartPlay();

	InitSpawnManager();
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



ADiaDungeon* ADiabloGameMode::GetDungeon(FName id)
{
	return m_MapDungeonActors[id]; //use dungeon location to Player Start?
}
