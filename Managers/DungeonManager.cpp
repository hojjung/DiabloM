#include "DungeonManager.h"
#include "DiabloGameInstance.h"
#include "DiabloGameMode.h"
#include "DungeonMiniMap.h"
#include "MonsterSpawnManager.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Datas/DungeonDataTable.h"
#include "Engine/LevelStreaming.h"
#include "UObject/UObjectGlobals.h"
#include "Serialization/AsyncPackageLoader.h"
#include "Village/Portal.h"
#include "GridFlowBuilder.h"
#include "GridFlowConfig.h"
#include "GridFlowMiniMap.h"
#include "GridFlowModel.h"
#include "GridFlowAsset.h"
#include "MoviePlayer.h"
#include "DungeonThemeAsset.h"
#include "GridDungeonBuilder.h"


UDungeonManager::UDungeonManager(const FObjectInitializer& objInit):Super(objInit)
{
    static ConstructorHelpers::FClassFinder<ADgToVillagePortal> FoundPortal(
    TEXT("Blueprint'/Game/Blueprints/VillageActors/BP_DgVillagePortal.BP_DgVillagePortal_C'"));
    //
    //
    m_ClassDgVillagePortal=FoundPortal.Class;
    m_CurrentDgVillagePortal=nullptr;
    m_MatMinimap=nullptr;
    m_NamePortalID="DgPortal";
}

void UDungeonManager::Init()
{
    m_nClearableCount=0;
    m_nCurrentMonsterCount=0;
    m_bIsPlayerInDungeon=false;
    m_CurrentDungeonData=nullptr;
    m_nMonsterLevel=-1;
    m_nDungeonType=-1;
    m_nPointIndex=-1;
    
    UDungeonDataTable::GetDungeonTable->GetAllRows("DgManager-NoDungeonData",m_AryDungeonData);
}

void UDungeonManager::CreateQuadTreeBound()
{
    ADiaDungeon* Dg = ADiabloGameMode::Get->GetDungeon();
    UGridFlowConfig* Config = Cast< UGridFlowConfig>( Dg->GetConfig());
    UGridFlowModel* GridModel = Cast<UGridFlowModel>(Dg->GetModel());
    UGridFlowTilemap* GridTileMap = GridModel->Tilemap;
    ADiabloGameMode::Get->SetQuadTreeCoord(GridTileMap,Config);
}



void UDungeonManager::CreateDefaultInfinityDungeon(int level)
{
    m_nCurrentDgLevel= level;
    
    if(m_CurrentDungeonData)
    {
        ClearDungeon();
    }
    BindOnDgDelegate();
    
    m_nPointIndex=0;
    
    m_nMonsterLevel = StageLevelToDungeonLevel(level);

    m_nDungeonType = StageLevelToDungeonType(level);
    
    m_CurrentDungeonData = m_AryDungeonData[m_nDungeonType];

    BuildDungeonLevel(m_CurrentDungeonData);

    GetMoviePlayer()->PlayMovie();
}


void UDungeonManager::PortalToVillage(bool isDgCleared)
{
    PRINTF("Dgm - Portal Village");

    if(isDgCleared)
    {
        APlayerDiabloCharacter* PlayerPawn = ADiabloPlayerController::Get->GetPlayerPawn();
        PlayerPawn->SetActorLocation(GetCurrentPlayerFeetLoc(),false,nullptr,ETeleportType::None);

        m_bIsPlayerInDungeon=false;

        ClearDungeon();
        

        return;
    }

    if(!m_bIsPlayerInDungeon)
    {
        return;
    }
    
    APlayerDiabloCharacter* PlayerPawn = ADiabloPlayerController::Get->GetPlayerPawn();
    
    PlayerPawn->SetActorLocation(GetCurrentPlayerFeetLoc(),false,nullptr,ETeleportType::None);

    ADiabloPlayerController::Get->ClientForceGarbageCollection();

    m_bIsPlayerInDungeon=false;

    ADiabloPlayerController::Get->GetMainCanvas()->CloseMinimap();//UI Set Brush Tick add
    //
    ADiabloGameMode::Get->HideAllTreeNodes();
}

void UDungeonManager::PortalToRecentDungeon()
{
    if(!IsDungeonOpened())
    {
        return;
    }
    
    PRINTF("Dgm - Portal Dungeon");
    
    APlayerDiabloCharacter* PlayerPawn = ADiabloPlayerController::Get->GetPlayerPawn();
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(PlayerPawn->GetWorld());
    FVector Loc = m_RecentDungeonFeetLoc;
    FNavLocation NavLoc;
    
    if(NavSys->GetRandomPointInNavigableRadius(Loc,300.f,NavLoc))
    {
        Loc=NavLoc.Location;
    }
    
    Loc.Z+=PlayerPawn->GetCapsule()->GetScaledCapsuleHalfHeight();
    
    PlayerPawn->SetActorLocation(Loc,false,nullptr,ETeleportType::None);

    m_bIsPlayerInDungeon=true;
    
    ADiabloPlayerController::Get->GetMainCanvas()->OpenMinimap();
}

void UDungeonManager::ClearDungeon()
{
    ADiabloGameMode::Get->ClearQuadTree();
    
    UMonsterSpawnManager* SpawnManager = UDiabloGameInstance::Get->GetMonsterSpawn();

    SpawnManager->Reset();
    
    m_nCurrentMonsterCount=0;
    
    m_nClearableCount=0;
    
    ADiabloPlayerController::Get->ClientForceGarbageCollection();
    
    m_RecentDungeonFeetLoc=ADiabloGameMode::Get->GetDungeon()->GetActorLocation();
    
    m_OnPortalCreate.Broadcast(false);

    if(m_CurrentDgVillagePortal)
    {
        m_CurrentDgVillagePortal->Destroy();
        m_CurrentDgVillagePortal=nullptr;
    }

    URewardManager* RewardManager = UDiabloGameInstance::Get->GetRewardManager();

    RewardManager->EnqueAllActors(false);
    
    ADiabloPlayerController::Get->GetMainCanvas()->CloseMinimap();//UI Set Brush Tick a

    ADiaDungeon* Dg = ADiabloGameMode::Get->GetDungeon();

    Dg->DestroyDungeon();
}

void UDungeonManager::RestartDungeon()
{
    CreateDefaultInfinityDungeon(m_nCurrentDgLevel);
    
    //SpawnMonstersToDungeon(m_nMonsterLevel, m_CurrentDungeonData);
    
    //PortalToRecentDungeon();
    
    //m_OnPortalCreate.Broadcast(true);
}

bool UDungeonManager::IsDungeonOpened()
{
    return ADiabloGameMode::Get->GetDungeon() != nullptr;
}

bool UDungeonManager::IsPlayerInDg()
{
    return m_bIsPlayerInDungeon;
}

void UDungeonManager::MonsterDead()
{
    m_nCurrentMonsterCount--;

    //if(m_nCurrentMonsterCount<=m_nClearableCount)
    {
        PRINTF("All MonsterDead");

        DungeonComplete();
    }
}

FVector UDungeonManager::GetCurrentPlayerFeetLoc()
{
    APlayerDiabloCharacter* PlayerPawn = ADiabloPlayerController::Get->GetPlayerPawn();
    
    m_RecentDungeonFeetLoc=PlayerPawn->GetMovementComponent()->GetActorFeetLocation();
    
    FVector Loc= ADiabloGameMode::Get->GetSpawnPoint()->GetActorLocation();
    
    Loc.Z+=PlayerPawn->GetCapsule()->GetScaledCapsuleHalfHeight();

    return Loc;
}

int UDungeonManager::StageLevelToDungeonLevel(int stageLevel)
{
    //FDungeonDataRow
    return stageLevel;
}

int UDungeonManager::StageLevelToDungeonType(int stageLevel)
{
    return 0;
}

void UDungeonManager::BuildDungeonLevel(FDungeonDataRow* SelectedDungeonData)
{
    ADiaDungeon* Dg = ADiabloGameMode::Get->GetDungeon();

    Dg->Themes.Reset();
    Dg->Themes.Add(Dg->GetDgData(SelectedDungeonData->m_IDDgTheme).m_DgTheme);
    
    Dg->SetBuilderClass(UGridFlowBuilder::StaticClass());

     UGridFlowConfig* Config = Cast< UGridFlowConfig>( Dg->GetConfig());
    
     Config->GridFlow = Dg->GetDgData(SelectedDungeonData->m_IDDgTheme).m_DgGridFlow;
    
     Config->Instanced = true;
    
     Config->Seed = FMath::Rand();
    
     Config->GridSize = FVector(200.f,200.f,100.f);
    
    ADiabloPlayerController::Get->bBlockInput=true;
    
    Dg->BuildDungeon();
}

void UDungeonManager::OnDgBuildComplete(ADungeon* Dungeon)
{
    PRINTF("DgBuildCOmplete");
    UNavigationSystemBase* NavSystems = (Dungeon->GetWorld()->GetNavigationSystem());
    check(NavSystems);
    UNavigationSystemV1* NavV1 = Cast<UNavigationSystemV1>(NavSystems);
    NavV1->OnNavigationGenerationFinishedDelegate.Clear();
    NavV1->OnNavigationGenerationFinishedDelegate.AddDynamic(this, &UDungeonManager::OnNavCookComplete);
}

ADgToVillagePortal* UDungeonManager::GetDgCompletePortalOpen()
{
    return m_CurrentDgVillagePortal;
}

void UDungeonManager::OnNavCookComplete(ANavigationData* NavData)
{
    UWorld* World = ADiabloPlayerController::Get->GetWorld();
    UNavigationSystemBase* NavSystems = (World->GetNavigationSystem());
    check(NavSystems);
    UNavigationSystemV1* NavV1 = Cast<UNavigationSystemV1>(NavSystems);
    NavV1->OnNavigationGenerationFinishedDelegate.Clear();
    
    m_RecentDungeonFeetLoc=ADiabloGameMode::Get->GetDungeon()->GetStartPoint();
    
    CreateQuadTreeBound();

    SpawnMonstersToDungeon(m_nMonsterLevel, m_CurrentDungeonData);

    PortalToRecentDungeon();

    m_OnPortalCreate.Broadcast(true);

    UGridFlowMiniMap::Get->BuildLayout(ADiabloGameMode::Get->GetDungeon()->GetModel(),ADiabloGameMode::Get->GetDungeon()->GetConfig());
    
    m_MatMinimap = UGridFlowMiniMap::Get->CreateMaterialInstance();
    
    ADiabloPlayerController::Get->GetMainCanvas()->UpdateMinimap(m_MatMinimap);//UI Set Brush Tick add

    ADiabloPlayerController::Get->GetMainCanvas()->CloseMapMenu();
    
    PRINTF("OnNavCookComplete");

    ADiabloPlayerController::Get->bBlockInput=false;
    
    //ADiabloPlayerController::Get->SetInputMode(FInputModeGameAndUI());
    
    GetMoviePlayer()->StopMovie();
}

void UDungeonManager::SpawnMonstersToDungeon(int MonsterLevel, FDungeonDataRow* SelectedDungeonData)
{
    UMonsterSpawnManager* SpawnManager = UDiabloGameInstance::Get->GetMonsterSpawn();

    SpawnManager->Reset();
    
    m_nCurrentMonsterCount=0;
    
    m_nClearableCount=0;
    
    if(ADiabloGameMode::Get->GetDungeon()->GetArySpawnPoints().Num()<1)
    {
        PRINTF("NoSpawnPint");
        return;
    }
    
    FMonsterHordeHandle& Horde = SelectedDungeonData->m_Horde;

    for(const FTransform& PointTrans : ADiabloGameMode::Get->GetDungeon()->GetArySpawnPoints())
    {
        SpawnManager->SpawnIter(PointTrans.GetLocation(),*Horde.GetRow<FMonsterHordeRow>(""),MonsterLevel,this);    
    }

    m_nCurrentMonsterCount = SpawnManager->GetCurrentMonsters().Num();
    
    m_nClearableCount = m_nCurrentMonsterCount *0.1f;//Test should be 0.85

    PRINTF("Dgmanager-Spawn Point Count: %d",ADiabloGameMode::Get->GetDungeon()->GetArySpawnPoints().Num());
    PRINTF("Dgmanager-Monster Spawned Count: %d",m_nCurrentMonsterCount);
    PRINTF("Dgmanager-Clearable Remain Count: %d",m_nClearableCount);
}

void UDungeonManager::DungeonComplete()
{
    APlayerDiabloCharacter* PlayerPawn = ADiabloPlayerController::Get->GetPlayerPawn();
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(PlayerPawn->GetWorld());
    FVector Loc = PlayerPawn->GetActorLocation();
    FNavLocation NavLoc;
    
    if(NavSys->GetRandomPointInNavigableRadius(Loc,400.f,NavLoc))
    {
        Loc=NavLoc.Location;
    }
    
    Loc.Z += PlayerPawn->GetCapsule()->GetScaledCapsuleHalfHeight();

    //플레이어 체력 전부체워줘야함
    
    //마나 스태미나 캐릭터는 자원 채워줘야함

    if(m_CurrentDgVillagePortal)
    {
        m_CurrentDgVillagePortal->Destroy();
        m_CurrentDgVillagePortal=nullptr;
    }
    
    FActorSpawnParameters Param;
    
    Param.bNoFail = true;
    //88
    FRotator Rot(0.f,0.f,0.f);
    
    m_CurrentDgVillagePortal = PlayerPawn->GetWorld()->SpawnActor<ADgToVillagePortal>(m_ClassDgVillagePortal, Loc, Rot, Param);
    
    UGridFlowMiniMap::Get->AddTrackActor(m_NamePortalID,m_CurrentDgVillagePortal);

    PRINTF("DgStageClear");
}

void UDungeonManager::BindOnDgDelegate()
{
    UWorld* World = ADiabloPlayerController::Get->GetWorld();
    
    ADiabloGameMode::Get->GetDungeon()->OnDungeonBuildComplete.Clear();
    
    ADiabloGameMode::Get->GetDungeon()->OnDungeonBuildComplete.AddDynamic(this,&UDungeonManager::OnDgBuildComplete);

    PRINTF("BindDelegate");
}

