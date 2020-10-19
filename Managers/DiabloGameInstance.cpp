#include "DiabloGameInstance.h"
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


