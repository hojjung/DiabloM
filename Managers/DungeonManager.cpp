#include "DungeonManager.h"
#include "DiabloGameInstance.h"
#include "DiabloGameMode.h"
#include "DungeonMiniMap.h"
#include "GridFlowMiniMap.h"
#include "MonsterSpawnManager.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Datas/DungeonDataTable.h"
#include "Engine/LevelStreaming.h"
#include "Objs/Actor/DgMobSpawnPoint.h"
#include "UObject/UObjectGlobals.h"
#include "Serialization/AsyncPackageLoader.h"
#include "Village/Portal.h"
#include "GenericOctree.h"
#include "GridFlowModel.h"

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
    m_CurrentDungeon=nullptr;
    m_CurrentDungeonData=nullptr;
    m_nMonsterLevel=-1;
    m_nDungeonType=-1;
    m_nPointIndex=-1;
    
    UDungeonDataTable::GetDungeonTable->GetAllRows("DgManager-NoDungeonData",m_AryDungeonData);
    
}

void UDungeonManager::CreateQuadTreeBound()
{
    UGridFlowModel* GridModel =Cast<UGridFlowModel>(m_CurrentDungeon.Get()->GetModel());
    UGridFlowTilemap* GridTileMap = GridModel->Tilemap;
    ADiabloGameMode::Get->SetQuadTreeCoord(m_CurrentDungeon.Get(),GridTileMap);
}

void UDungeonManager::CreateDefaultInfinityDungeon(int level)
{
    m_nPointIndex=0;
    
    m_nMonsterLevel = StageLevelToDungeonLevel(level);

    m_nDungeonType = StageLevelToDungeonType(level);
    
    m_CurrentDungeonData = m_AryDungeonData[m_nDungeonType];

    LoadDungeonLevel(m_CurrentDungeonData);

    CreateQuadTreeBound();

    SpawnMonstersToDungeon(m_nMonsterLevel, m_CurrentDungeonData);

    PortalToRecentDungeon();

    m_OnPortalCreate.Broadcast(true);

    UGridFlowMiniMap::Get->BuildLayout(m_CurrentDungeon->GetModel(),m_CurrentDungeon->GetConfig());
    m_MatMinimap = UGridFlowMiniMap::Get->CreateMaterialInstance();
    ADiabloPlayerController::Get->UpdateMinimap(m_MatMinimap);//UI Set Brush Tick add

    ADiabloPlayerController::Get->CloseMapSelectMenu();
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

    ADiabloPlayerController::Get->HideMinimap();//UI Set Brush Tick add
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
    ADiabloPlayerController::Get->ShowMinimap();
}

void UDungeonManager::ClearDungeon()
{
    for (AMonsterPawn* Pawn : m_AryMonsterSpawnedCurrently)
    {
        if(Pawn)
        {
            Pawn->Destroy();
        }
    }
    m_AryMonsterSpawnedCurrently.Reset();
    m_nCurrentMonsterCount=0;
    m_nClearableCount=0;
    
    ADiabloPlayerController::Get->ClientForceGarbageCollection();
    m_RecentDungeonFeetLoc=m_CurrentDungeon->GetActorLocation();
    
    m_OnPortalCreate.Broadcast(false);

    if(m_CurrentDgVillagePortal)
    {
        m_CurrentDgVillagePortal->Destroy();
        m_CurrentDgVillagePortal=nullptr;
    }

    ADiabloPlayerController::Get->HideMinimap();//UI Set Brush Tick a
}

void UDungeonManager::RestartDungeon()
{
    ClearDungeon();
    SpawnMonstersToDungeon(m_nMonsterLevel, m_CurrentDungeonData);
    PortalToRecentDungeon();
    m_OnPortalCreate.Broadcast(true);
}

bool UDungeonManager::IsDungeonOpened()
{
    return m_CurrentDungeon.Get() != nullptr;
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

void UDungeonManager::LoadDungeonLevel(FDungeonDataRow* SelectedDungeonData)
{
    FName DungeonID=SelectedDungeonData->m_IDPremadeDungeons.GetRandom();
    
    m_CurrentDungeon = ADiabloGameMode::Get->GetDungeon(DungeonID);

    m_RecentDungeonFeetLoc=m_CurrentDungeon->GetActorLocation();
}

void UDungeonManager::SpawnMonstersToDungeon(int MonsterLevel, FDungeonDataRow* SelectedDungeonData)
{
    m_AryMonsterSpawnedCurrently.Reset();
    m_nCurrentMonsterCount=0;
    m_nClearableCount=0;
    
    if(m_CurrentDungeon->GetArySpawnPoints().Num()<1)
    {
        return;
    }
    
    UMonsterSpawnManager* SpawnManager = UDiabloGameInstance::Get->GetMonsterSpawn();
    
    for(FMonsterHordeHandle& Horde :SelectedDungeonData->m_AryHorde)
    {
        if(!SpawnManager->SpawnIter(m_CurrentDungeon->GetArySpawnPoints()[m_nPointIndex++]->GetActorLocation(),
            *Horde.GetRow<FMonsterHordeRow>(""),m_AryMonsterSpawnedCurrently,MonsterLevel))
        {
            break;
        }
    }

    for(AMonsterPawn* Mob : m_AryMonsterSpawnedCurrently)
    {
        Mob->m_SpawnedManager = this;
    }
    
    m_nCurrentMonsterCount=m_AryMonsterSpawnedCurrently.Num();
    m_nClearableCount = m_nCurrentMonsterCount *0.1f;
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
    
}

