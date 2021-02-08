#include "DiabloGameMode.h"
#include "DiabloGameInstance.h"
#include "DungeonManager.h"
#include "EngineUtils.h"
#include "DungeonMiniMap.h"
#include "GridFlowMiniMap.h"
#include "GridFlowTilemap.h"
#include "MonsterSpawnManager.h"
#include "MoviePlayer.h"
#include "PlayfabManager.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/StartMap/PlayerVisual.h"
#include "GameFramework/PlayerState.h"
#include "Objs/Actor/DiaDungeon.h"
#include "Village/Portal.h"

ADiabloGameMode* ADiabloGameMode::Get=nullptr;

ADiabloGameMode::ADiabloGameMode()
{
	Get=this;
	
	PlayerControllerClass = ADiabloPlayerController::StaticClass();
	
	DefaultPawnClass = APlayerDiabloCharacter::StaticClass();

	m_ItemDropActionManager=CreateDefaultSubobject<UActionManagerComponent>("ItemActionManager");

	m_PlayerActionManager=CreateDefaultSubobject<UActionManagerComponent>("PlayerActionManager");

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
	//m_PostProcess=CreateDefaultSubobject<UPostProcessComponent>("PostProcess");

	m_nDepth=3;

}

void ADiabloGameMode::SpawnVisualPlayer()
{
	FVector Faraway(-20000,-20000,-20000);

	FRotator Rot =FRotator(-20,-10,0.f);

	FActorSpawnParameters Param;
	
	m_PlayerVisual = GetWorld()->SpawnActor<APlayerVisual>(m_ClassVisualActor,Faraway,Rot,Param);

	UPlayerCreateManager::Get->SetCurrentDataFromSaveFile();

	ADiabloPlayerController::Get->GetMainCanvas()->m_OnWidgetOpenClose.AddUObject(this,&ADiabloGameMode::SetVisibleVisualActor);

	m_PlayerVisual->HideMeshWithTick();
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
	SetDungeonInstanceFromMap();

	FindSpawnPoint();

	InitRewardManager();
	
	//ㄴBeginPlay Before
	Super::StartPlay();

	InitSpawnManager();

	InitMinimap();

	GetMoviePlayer()->StopMovie();

	SpawnVisualPlayer();
}

void ADiabloGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);


	m_QuadTree.Reset();

	//Login(ADiabloPlayerController::Get,ENetRole::ROLE_Authority,);
	//Logout(ADiabloPlayerController::Get);
	PRINTF("LogoutNeed");
}

void ADiabloGameMode::SetDungeonInstanceFromMap()
{
	for (ADiaDungeon* DungeonInst : TActorRange<ADiaDungeon>(GetWorld()))
	{
		if(DungeonInst)
		{
			m_MapDungeonActor = DungeonInst;
			m_MapDungeonActor->Init();
			break;
		}
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

void ADiabloGameMode::SetQuadTreeCoord(UGridFlowTilemap* dgTilemap,UGridFlowConfig* config)
{
	FVector DgCenterPos = m_MapDungeonActor->GetActorLocation();
	
	FVector DgMinPos = DgCenterPos;
	
	FVector DgMaxPos = DgCenterPos;
	//
	int32 Width = dgTilemap->GetWidth() * config->GridSize.X;
    
	int32 Height = dgTilemap->GetHeight() * config->GridSize.Y;
	
	int32 OffsetIdxX = Width / 2;
    
	int32 OffsetIdxY = Height / 2;
	//
	DgMinPos.X-=OffsetIdxX;
	DgMinPos.Y-=OffsetIdxY;
	
	DgMaxPos.X+=OffsetIdxX;
	DgMaxPos.Y+=OffsetIdxY;
	//
	if(m_QuadTree.Get())
	{
		m_QuadTree.Reset();
	}
	m_QuadTree =  MakeUnique<Quadtree>(m_nDepth,FVector2D(DgMinPos),FVector2D(DgMaxPos));
	PRINTF("Min :%s",*DgMinPos.ToString());
	PRINTF("Max :%s",*DgMaxPos.ToString());
}

void ADiabloGameMode::ClearQuadTree()
{
	if(m_QuadTree.Get())
	{
		m_QuadTree.Reset();
	}
}

void ADiabloGameMode::HideAllTreeNodes()
{
	if(m_QuadTree)
	{
		m_QuadTree->HideAllNode();
	}
}

bool ADiabloGameMode::CheckActorInVisibleNode(ITickHideable* actor)
{
	return m_QuadTree->CheckActorInVisibleNode(actor);
}

ITickHideable* ADiabloGameMode::GetNearestActor(FVector2D& position)
{
	return m_QuadTree->GetNearestActor(position);
}

ITickHideable* ADiabloGameMode::GetNearestActor(FVector2D&& position)
{
	return m_QuadTree->GetNearestActor(position);
}

void ADiabloGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(UDiabloGameInstance::Get->GetDungeonManager()->IsDungeonOpened())
	{
		m_MiniMap->MiniMapTick(DeltaSeconds);
	}

	if(m_QuadTree)
	{
		m_QuadTree->TryShow9Cell(ADiabloPlayerController::Get->GetPlayerPawn());
	}
}


ADiaDungeon* ADiabloGameMode::GetDungeon()
{
	return m_MapDungeonActor;
}

void ADiabloGameMode::SetVisibleVisualActor(bool able)
{
	if(able)
	{
		m_PlayerVisual->ShowMeshWithTick();
	}
	else
	{
	 	m_PlayerVisual->HideMeshWithTick();
	}
}