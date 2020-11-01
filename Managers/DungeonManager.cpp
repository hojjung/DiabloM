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

void UDungeonManager::Init()
{
    m_CurrentDungeon=nullptr;
    m_CurrentDungeonData=nullptr;
    m_nMonsterLevel=-1;
    m_nDungeonType=-1;
    m_nPointIndex=-1;
    
    UDungeonDataTable::GetDungeonTable->GetAllRows("DgManager-NoDungeonData",m_AryDungeonData);
}

void UDungeonManager::CreateDefaultInfinityDungeon(int level)
{
    m_nPointIndex=0;
    
    m_nMonsterLevel = StageLevelToDungeonLevel(level);

    m_nDungeonType = StageLevelToDungeonType(level);
    
    m_CurrentDungeonData = m_AryDungeonData[m_nDungeonType];
    
    LoadDungeonLevel(m_CurrentDungeonData);

    SpawnMonstersToDungeon(m_nMonsterLevel, m_CurrentDungeonData);

    PortalToRecentDungeon();

    m_OnPortalCreate.Broadcast(true);

    ADiabloGameMode::Get->GetMinimapManager()->BuildLayout(m_CurrentDungeon->GetModel(),m_CurrentDungeon->GetConfig());
    m_MatMinimap = ADiabloGameMode::Get->GetMinimapManager()->CreateMaterialInstance();
    ADiabloPlayerController::Get->UpdateMinimap(m_MatMinimap);//UI Set Brush Tick add
    
}

void UDungeonManager::ShowSpawnedMonster()
{
    for (auto* Pawn : m_AryMonsterSpawnedCurrently)
    {
        if(Pawn)
        {
            Pawn->SetHidden(false);
        }
    }
}

void UDungeonManager::HideSpawnedMonster()
{
    for (AMonsterPawn* Pawn : m_AryMonsterSpawnedCurrently)
    {
        if(Pawn)
        {
            Pawn->FocusTarget(nullptr);
            Pawn->SetHidden(true);
        }
    }
}

void UDungeonManager::PortalToVillage()
{
    PRINTF("Dgm - Portal Village");
    
    APlayerDiabloCharacter* PlayerPawn = ADiabloPlayerController::Get->GetPlayerPawn();
    m_RecentDungeonFeetLoc=PlayerPawn->GetMovementComponent()->GetActorFeetLocation();
    FVector Loc= ADiabloGameMode::Get->GetSpawnPoint()->GetActorLocation();
    Loc.Z+=PlayerPawn->GetCapsule()->GetScaledCapsuleHalfHeight();
    PlayerPawn->SetActorLocation(Loc,false,nullptr,ETeleportType::None);
    
    HideSpawnedMonster();
    m_CurrentDungeon->HideDungeon();

    ADiabloPlayerController::Get->ClientForceGarbageCollection();
}

void UDungeonManager::PortalToRecentDungeon()
{
    PRINTF("Dgm - Portal Dungeon");
    m_CurrentDungeon->ShowDungeon();
    ShowSpawnedMonster();
    APlayerDiabloCharacter* PlayerPawn = ADiabloPlayerController::Get->GetPlayerPawn();
    FVector Loc=m_RecentDungeonFeetLoc;
    Loc.Z+=PlayerPawn->GetCapsule()->GetScaledCapsuleHalfHeight();
    PlayerPawn->SetActorLocation(Loc,false,nullptr,ETeleportType::None);
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
    m_CurrentDungeon->HideDungeon();
    ADiabloPlayerController::Get->ClientForceGarbageCollection();
    m_RecentDungeonFeetLoc=m_CurrentDungeon->GetActorLocation();
    
    m_OnPortalCreate.Broadcast(false);
}

void UDungeonManager::RestartDungeon()
{
    ClearDungeon();
    SpawnMonstersToDungeon(m_nMonsterLevel, m_CurrentDungeonData);
    PortalToRecentDungeon();
}

bool UDungeonManager::IsDungeonOpened()
{
    return m_CurrentDungeon.Get() != nullptr;
}

int UDungeonManager::StageLevelToDungeonLevel(int stageLevel)
{
    //FDungeonDataRow
    return 1;
}

int UDungeonManager::StageLevelToDungeonType(int stageLevel)
{
    return 0;
}

void UDungeonManager::LoadDungeonLevel(FDungeonDataRow* SelectedDungeonData)
{
    FName DungeonID=SelectedDungeonData->m_IDPremadeDungeons.GetRandom();
    
    m_CurrentDungeon = ADiabloGameMode::Get->GetDungeon(DungeonID);

    m_CurrentDungeon->ShowDungeon();

    m_RecentDungeonFeetLoc=m_CurrentDungeon->GetActorLocation();
}

void UDungeonManager::SpawnMonstersToDungeon(int MonsterLevel, FDungeonDataRow* SelectedDungeonData)
{
    m_AryMonsterSpawnedCurrently.Reset();
    
    UMonsterSpawnManager* SpawnManager = UDiabloGameInstance::Get->GetMonsterSpawn();
    
    for(FMonsterHordeHandle& Horde :SelectedDungeonData->m_AryHorde)
    {
        if(!SpawnManager->SpawnIter(m_CurrentDungeon->GetArySpawnPoints()[m_nPointIndex++]->GetActorLocation(),
            *Horde.GetRow<FMonsterHordeRow>(""),m_AryMonsterSpawnedCurrently,MonsterLevel))
        {
            break;
        }
    }
}

