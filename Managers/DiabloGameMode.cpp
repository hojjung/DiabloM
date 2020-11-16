#include "DiabloGameMode.h"
#include "DiabloGameInstance.h"
#include "EngineUtils.h"
#include "DungeonMiniMap.h"
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

	m_fMinimapTextureSize=1024.f;
	m_fMinimapOutlineThickness=4.0f;
	m_fMinimapDoorThickness=8.0f;
	m_fMinimapBlurRadius =5.0f;
	m_nMinimapBlurIterations = 3;
	m_bMinimapbEnableFogOfWar = false;
	m_fMinimapFogOfWarTextureScale=0.25f;
	m_NameMinimapFogOfWarTrackingItem="player";
	m_fMinimapFogOfWarVisiblityDistance=6000.f;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FoundMapMat(
	TEXT("MaterialInstanceConstant'/Game/03_VisualEffect/Minimap/Materials/M_MMGridFlow_Template_Inst.M_MMGridFlow_Template_Inst'"));
	m_MinimapMaterialTemplate=FoundMapMat.Object;
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> FoundFogTexture(
    TEXT("Texture2D'/Game/03_VisualEffect/Minimap/Textures/FogOfWarMask.FogOfWarMask'"));
	m_MinimapFogOfWarExploreTexture=FoundFogTexture.Object;

	//m_FOWMaterialTemplate
	m_PostProcess=CreateDefaultSubobject<UPostProcessComponent>("PostProcess");
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
	m_MiniMap->InitMap(GetWorld(),m_fMinimapTextureSize,m_fMinimapOutlineThickness,m_fMinimapDoorThickness,
		m_AryOverlayMinimap,m_MinimapMaterialTemplate,m_fMinimapBlurRadius,m_nMinimapBlurIterations,m_bMinimapbEnableFogOfWar
		,m_fMinimapFogOfWarTextureScale,m_NameMinimapFogOfWarTrackingItem,m_MinimapFogOfWarExploreTexture,m_fMinimapFogOfWarVisiblityDistance);

	UGridFlowMiniMap::Get = m_MiniMap;
	m_MiniMap->AddTrackActor(m_NameMinimapFogOfWarTrackingItem,ADiabloPlayerController::Get->GetPlayerPawn(),true);
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

	//InitFOW();
	InitMinimap();
	//
	
}

void ADiabloGameMode::SetDungeonInstanceToMap()
{
	m_MapDungeonActors.Reset();
	
	for (ADiaDungeon* DungeonInst : TActorRange<ADiaDungeon>(GetWorld()))
	{
		m_MapDungeonActors.Emplace(DungeonInst->m_LevelName,DungeonInst);
		DungeonInst->HideDungeon();
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
	//Need Bool
	m_MiniMap->MiniMapTick(DeltaSeconds);
	//m_FOW->MyTick(DeltaSeconds);
}


ADiaDungeon* ADiabloGameMode::GetDungeon(FName id)
{
	return m_MapDungeonActors[id]; //use dungeon location to Player Start?
}
