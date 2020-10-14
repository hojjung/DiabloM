#include "DiabloGameInstance.h"
#include "NavigationSystem.h"
#include "StartMap/PlayerCreateManager.h"
#include "MonsterSpawnManager.h"
#include "DungeonManager.h"


UDiabloGameInstance* UDiabloGameInstance::Get = nullptr;

UDiabloGameInstance::UDiabloGameInstance()
{
    UDiabloGameInstance::Get = this;

    m_ItemManager=nullptr;

    m_SaveLoadManager=nullptr;
    
    m_PlCreateManager=nullptr;

    m_MonsterSpawn=nullptr;

    m_DungeonManager=nullptr;
}

void UDiabloGameInstance::Init()
{
    Super::Init();
    
    if (!m_ItemManager)
    {
        m_ItemManager=NewObject<UItemManager>();
        m_ItemManager->Init(this);
    }
    if (!m_SaveLoadManager)
    {
        m_SaveLoadManager=NewObject<USaveLoadManager>();
    }

    if(!m_PlCreateManager)
    {
        m_PlCreateManager = NewObject<UPlayerCreateManager>();
        m_PlCreateManager->Init(this);
    }

    if(!m_MonsterSpawn)
    {
        m_MonsterSpawn=NewObject<UMonsterSpawnManager>();
        //m_MonsterSpawn->UpdateWorld(GetWorld());
    }

    if(!m_DungeonManager)
    {
        m_DungeonManager=NewObject<UDungeonManager>();
        m_DungeonManager->Init();
    }
}

void UDiabloGameInstance::Shutdown()
{
    Super::Shutdown();
}

FItemInstance UDiabloGameInstance::CreateItem(FName id)
{
    return m_ItemManager->CreateItemInstance(id);
}

ADroppedItem* UDiabloGameInstance::DropItemActor(FItemInstance& myItem)
{
    FVector ActorPos = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetMovementComponent()->GetActorFeetLocation();
    
    auto* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    
    FNavLocation OutLoc;
    
    if(NavSys->GetRandomPointInNavigableRadius(ActorPos,400.f,OutLoc))
    {
        ActorPos = OutLoc.Location;
    }

    return m_ItemManager->CreateItemActor(myItem,ActorPos);
}


