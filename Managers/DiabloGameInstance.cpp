#include "DiabloGameInstance.h"

#include "AbilitySystemGlobals.h"
#include "NavigationSystem.h"
#include "StartMap/PlayerCreateManager.h"
#include "MonsterSpawnManager.h"
#include "DungeonManager.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "GameFramework/GameUserSettings.h"
#include "Item/Inventory.h"


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
    UAbilitySystemGlobals::Get().InitGlobalData();
    
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
    }

    if(!m_DungeonManager)
    {
        m_DungeonManager=NewObject<UDungeonManager>();
        m_DungeonManager->Init();
    }

    //
    UGameUserSettings::GetGameUserSettings()->SetFrameRateLimit(45.f);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
}

void UDiabloGameInstance::Shutdown()
{
    Super::Shutdown();
}

FItemInstance UDiabloGameInstance::CreateItem(FName id)
{
    return m_ItemManager->CreateItemInstance(id);
}

ADroppedItem* UDiabloGameInstance::DropItemActor(APawn* dropCenterActor, float dropRadius, FItemInstance& myItem)
{
    FVector ActorPos = dropCenterActor->GetMovementComponent()->GetActorFeetLocation();
    
    auto* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(dropCenterActor->GetWorld());
    
    return DropItemActor(ActorPos,NavSys,dropRadius,myItem);
}

ADroppedItem* UDiabloGameInstance::DropItemActor(FVector dropCenterPos, UNavigationSystemV1* nav, float dropRadius,
    FItemInstance& myItem)
{

    if(!nav)
    {
        PRINTF("ItemDropFail,NoNav");

        return nullptr;
    }
    
    FNavLocation OutLoc;
    
    if(nav->GetRandomPointInNavigableRadius(dropCenterPos,dropRadius,OutLoc))
    {
        dropCenterPos = OutLoc.Location;
    }

    return m_ItemManager->CreateItemActor(myItem,dropCenterPos);
}

ADroppedGold* UDiabloGameInstance::DropGoldActor(APawn* dropCenterActor, float dropRadius)
{
    FVector ActorPos = dropCenterActor->GetMovementComponent()->GetActorFeetLocation();
    
    auto* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(dropCenterActor->GetWorld());

    if(!NavSys)
    {
        PRINTF("11ItemDropFail,NoNav");

        return nullptr;
    }

    FNavLocation OutLoc;
    
    if(NavSys->GetRandomPointInNavigableRadius(ActorPos,dropRadius,OutLoc))
    {
        ActorPos = OutLoc.Location;
    }
    
    return  Cast<ADroppedGold>( SpawnDropCollInteract(m_DropGoldClass,ActorPos));
}

AHealthSphere* UDiabloGameInstance::DropHpSphereActor(APawn* dropCenterActor, float dropRadius)
{
    FVector ActorPos = dropCenterActor->GetMovementComponent()->GetActorFeetLocation();
    
    auto* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(dropCenterActor->GetWorld());

    if(!NavSys)
    {
        PRINTF("22ItemDropFail,NoNav");

        return nullptr;
    }

    FNavLocation OutLoc;
    
    if(NavSys->GetRandomPointInNavigableRadius(ActorPos,dropRadius,OutLoc))
    {
        ActorPos = OutLoc.Location;
    }
    
    return   Cast<AHealthSphere>(SpawnDropCollInteract(m_DropHpSphereClass,ActorPos));
}

ACollisionInteract* UDiabloGameInstance::SpawnDropCollInteract(TSubclassOf<ACollisionInteract> classWant,
                                                               FVector posWant)
{
    FRotator Rot;
    Rot.Yaw=FMath::RandRange(0.f,360.f);
    FActorSpawnParameters Param;
    Param.bNoFail=true;
    
    ADroppedItem* DroppedActor = Cast<ADroppedItem>(
        GetWorld()->SpawnActor(classWant, &posWant,&Rot,Param));

    return DroppedActor;
}

void UDiabloGameInstance::CreateDungeon()
{
    UDiabloGameInstance::Get->GetDungeonManager()->CreateDefaultInfinityDungeon(1);
    m_bTestIsDungeonOpen=true;
}

void UDiabloGameInstance::BackToDungeon()
{
    if(m_bTestIsDungeonOpen)
    {
        UDiabloGameInstance::Get->GetDungeonManager()->PortalToVillage();
        m_bTestIsDungeonOpen=false;
    }
    else
    {
        UDiabloGameInstance::Get->GetDungeonManager()->PortalToRecentDungeon();
        m_bTestIsDungeonOpen=true;
    }
}

void UDiabloGameInstance::SaveAllPlayerInfo()
{
    TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
    TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
    
    USaveLoadManager::Get->SaveInventory(UPlayerCreateManager::Get->m_CurrentSelectSlot,DiaPC->GetInven()->GetItemAry());

    TArray<FItemInstance> AryEquip;

    for(FEquipSlot* Slot : ADiabloPlayerController::Get->GetEquipment()->GetArySlotPtr())
    {
        AryEquip.Emplace(Slot->m_Item);
    }
	
    USaveLoadManager::Get->SaveEquipment(UPlayerCreateManager::Get->m_CurrentSelectSlot,AryEquip);

  
    FText Name = DiaPl->m_TextUnitName;
    int Lev= DiaPl->GetCharacterLevel();
    int Hair= DiaPl->m_HairIndex;
    int Face= DiaPl->m_FaceIndex;
	
    USaveLoadManager::Get->SaveCharacterStat(UPlayerCreateManager::Get->m_CurrentSelectSlot,Lev,Name,Face,Hair,USaveLoadManager::Get->GetCurrentPlayerClassName(),DiaPl->m_fCurrentExp);

}

bool UDiabloGameInstance::IsDungeonOpened()
{
    return GetDungeonManager()->IsDungeonOpened();
}


