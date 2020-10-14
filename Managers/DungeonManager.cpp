#include "DungeonManager.h"
#include "DiabloGameInstance.h"
#include "DiabloGameMode.h"
#include "MonsterSpawnManager.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Datas/DungeonDataTable.h"
#include "Engine/LevelStreaming.h"
#include "Objs/Actor/DgMobSpawnPoint.h"
#include "UObject/UObjectGlobals.h"
#include "Serialization/AsyncPackageLoader.h"

UDungeonManager::UDungeonManager()
{
    m_CurrentDungeon=nullptr;
}

void UDungeonManager::Init()
{
    UDungeonDataTable::GetDungeonTable->GetAllRows("DgManager-NoDungeonData",m_AryDungeonData);
}

void UDungeonManager::CreateDefaultInfinityDungeon(int level)
{
    m_nPointIndex=0;
    
    int MonsterLevel = StageLevelToDungeonLevel(level);

    int DungeonType = StageLevelToDungeonType(level);
    
    FDungeonDataRow* SelectedDungeonData =m_AryDungeonData[DungeonType];
    
    LoadDungeonLevel(SelectedDungeonData);

    SpawnMonstersToDungeon(MonsterLevel, SelectedDungeonData);

    PortalToRecentDungeon();
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
}

void UDungeonManager::PortalToRecentDungeon()
{
    PRINTF("Dgm - Portal Dungeon");
    ShowSpawnedMonster();
    
    APlayerDiabloCharacter* PlayerPawn = ADiabloPlayerController::Get->GetPlayerPawn();
    FVector Loc=m_RecentDungeonFeetLoc;
    Loc.Z+=PlayerPawn->GetCapsule()->GetScaledCapsuleHalfHeight();
    PlayerPawn->SetActorLocation(Loc,false,nullptr,ETeleportType::None);
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

