#include "DiaDungeon.h"
#include "DgMobSpawnPoint.h"
#include "DungeonModelHelper.h"

ADiaDungeon::ADiaDungeon(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
    m_ArySpawnPoints.Reset();
}

bool ADiaDungeon::IsMyActor(AActor* actor)
{
    const FName DungeonTag = UDungeonModelHelper::GetDungeonIdTag(this);

    if (actor->ActorHasTag(DungeonTag) || actor->ActorHasTag(UDungeonModelHelper::GenericDungeonIdTag))
    {
        return true;
    }

    return false;
}

void ADiaDungeon::AddSpawnPoints(ADgMobSpawnPoint* spawnPoint)
{
    m_ArySpawnPoints.Emplace(spawnPoint);
}

void ADiaDungeon::AddMyActors(AActor* actor)
{
    m_AryMyActors.Emplace(actor);
}

void ADiaDungeon::ShowDungeon()
{
    for (auto* ASD : m_AryMyActors)
    {
        ASD->SetActorHiddenInGame(false);
    }
}

void ADiaDungeon::HideDungeon()
{
    for (auto* ASD : m_AryMyActors)
    {
        ASD->SetActorHiddenInGame(true);
    }
}

void ADiaDungeon::ShuffleSpawnPoints(int iter)
{
    int MaxIndex = m_ArySpawnPoints.Num() - 1;
    
    for (int j = 0; j < iter; j++)
    {
        for (int i = 0; i < m_ArySpawnPoints.Num(); i++)
        {
            auto* Temp = m_ArySpawnPoints[i];
            int RandIndex = FMath::RandRange(0, MaxIndex);
            m_ArySpawnPoints[i] = m_ArySpawnPoints[RandIndex];
            m_ArySpawnPoints[RandIndex] = Temp;
        }
    }
}
