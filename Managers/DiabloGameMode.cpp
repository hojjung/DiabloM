#include "DiabloGameMode.h"
#include "DiabloGameInstance.h"
#include "EngineUtils.h"
#include "DungeonMiniMap.h"
#include "GridFlowMiniMap.h"
#include "GridFlowTilemap.h"
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

	m_nDepth=3;

	m_Min = FVector2D(0,0);
	
	m_Max = FVector2D(1000,1000);
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

	InitMinimap();
}

void ADiabloGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	delete m_QuadTree.Release();
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

void ADiabloGameMode::RegisterQuadElement(ITickHideable* actor)
{
	if(!m_QuadTree)
	{
		return;
	}
	 m_QuadTree->AddElement(actor);
}

void ADiabloGameMode::SetQuadTreeCoord(ADiaDungeon* dgActor,UGridFlowTilemap* dgTilemap)
{
	FVector DgCenterPos = dgActor->GetActorLocation();
	
	FVector DgMinPos = DgCenterPos;
	
	FVector DgMaxPos = DgCenterPos;
	//
	int32 Width = dgTilemap->GetWidth() * 400.f;
    
	int32 Height = dgTilemap->GetHeight() * 400.f;
	
	int32 OffsetIdxX = Width / 2;
    
	int32 OffsetIdxY = Height / 2;
	//
	DgMinPos.X-=OffsetIdxX;
	DgMinPos.Y-=OffsetIdxY;
	
	DgMaxPos.X+=OffsetIdxX;
	DgMaxPos.Y+=OffsetIdxY;
	//
	if(m_QuadTree)
	{
		delete m_QuadTree.Release();
	}
	m_QuadTree =  MakeUnique<Quadtree>(m_nDepth,FVector2D(DgMinPos),FVector2D(DgMaxPos));
	PRINTF("Min :%s",*DgMinPos.ToString());
	PRINTF("Max :%s",*DgMaxPos.ToString());
}

void ADiabloGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	m_OnTick.Broadcast(DeltaSeconds);
	//Need Bool
	m_MiniMap->MiniMapTick(DeltaSeconds);
	//m_FOW->MyTick(DeltaSeconds);


	if(m_QuadTree)
	{
		m_QuadTree->DrawBoxes(GetWorld());

		m_QuadTree->TryShow9Cell(ADiabloPlayerController::Get->GetPlayerPawn()->GetActorLocation());
	}
}


ADiaDungeon* ADiabloGameMode::GetDungeon(FName id)
{
	return m_MapDungeonActors[id]; //use dungeon location to Player Start?
}
