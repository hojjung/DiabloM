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
    //던전이란 무엇인가?
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

    FVector PlayerPos = m_CurrentDungeon->GetActorLocation();
    
    PlayerPos.Z+=ADiabloPlayerController::Get->GetPlayerPawn()->GetCapsule()->GetScaledCapsuleHalfHeight();
    
    ADiabloPlayerController::Get->GetPlayerPawn()->SetActorLocation(PlayerPos,false,nullptr,ETeleportType::None);
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
}

void UDungeonManager::SpawnMonstersToDungeon(int MonsterLevel, FDungeonDataRow* SelectedDungeonData)
{
    m_AryMonsterSpawnedCurrently.Reset();
    
    UMonsterSpawnManager* SpawnManager = UDiabloGameInstance::Get->GetMonsterSpawn();
    
    for(FMonsterHordeHandle& Horde :SelectedDungeonData->m_AryHorde)
    {
        SpawnManager->SpawnIter(m_CurrentDungeon->GetArySpawnPoints()[m_nPointIndex++]->GetActorLocation(),*Horde.GetRow<FMonsterHordeRow>(""),m_AryMonsterSpawnedCurrently,MonsterLevel);
    }
}

