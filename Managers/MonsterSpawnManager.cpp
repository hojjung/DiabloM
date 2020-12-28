#include "MonsterSpawnManager.h"
#include "DiabloGameInstance.h"
#include "EngineUtils.h"
#include "GridFlowMiniMap.h"
#include "Characters/DiabloPlayerController.h"

UMonsterSpawnManager::UMonsterSpawnManager()
{
    m_CurrentWorld = nullptr;
    m_NavSys = nullptr;
    m_fSpawnRadius = 1200.f;
    m_IdEnemy="enemy";
    m_IdBossEnemy="boss";
    m_IdSpecialEnemy="special";
}

void UMonsterSpawnManager::UpdateWorld(UWorld* world)
{
    m_CurrentWorld = world;
    m_NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(m_CurrentWorld);
}

bool UMonsterSpawnManager::SpawnIter(const FVector& centerSpawnLoc,const FMonsterHordeRow& selectedHorde, TArray<AMonsterPawn*>& outMobAry,
                                      int level,UDungeonManager* dgSpawnedManager)
{
    for (const FMonsterSelect& MobSelected : selectedHorde.m_AryMonsterEntity) 
    {
        for (int MobCount = 0; MobCount < MobSelected.m_nCount; MobCount++)
        {
            FVector PointSpawn = GetRandomPoint(centerSpawnLoc, m_fSpawnRadius);

            AMonsterPawn* SpawnedMob = SpawnMob(PointSpawn);
        
            if (!SpawnedMob)
            {
                continue;
            }

            outMobAry.Add(SpawnedMob);

            SpawnedMob->InitMonster(MobSelected.m_MonsterEntity, level,dgSpawnedManager);
        }
    }

    return true;
}

FVector UMonsterSpawnManager::GetRandomPoint(const FVector& loc, const float& radius)
{
    FNavLocation ResultLoc;

    if (!m_NavSys->GetRandomPointInNavigableRadius(loc, radius, ResultLoc))
    {
        //FAIL
        return loc;
    }

    return ResultLoc;
}

AMonsterPawn* UMonsterSpawnManager::SpawnMob(FVector loc)
{
    check(UCharacterDataTable::ClassMonsterPawn);
    
    FActorSpawnParameters Param;

    Param.bNoFail = true;
    loc.Z += 88.f;
    //88
    FRotator Rot;
    Rot.Yaw=FMath::RandRange(0.f,360.f);
    
    AMonsterPawn* Mob = m_CurrentWorld->SpawnActor<AMonsterPawn>(UCharacterDataTable::ClassMonsterPawn, loc, Rot, Param);

    UGridFlowMiniMap::Get->AddTrackActor(m_IdEnemy,Mob);

    return Mob;
}
