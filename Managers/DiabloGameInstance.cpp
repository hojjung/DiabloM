#include "DiabloGameInstance.h"
#include "AbilitySystemGlobals.h"
#include "BigInt.h"
#include "NavigationSystem.h"
#include "StartMap/PlayerCreateManager.h"
#include "MonsterSpawnManager.h"
#include "DungeonManager.h"
#include "PlayfabManager.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Datas/ShopItemTable.h"
#include "GameFramework/GameUserSettings.h"
#include "Item/Inventory.h"
#include "PlayfabManager.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"


UDiabloGameInstance* UDiabloGameInstance::Get = nullptr;

UDiabloGameInstance::UDiabloGameInstance()
{
    UDiabloGameInstance::Get = this;

    m_ItemManager = nullptr;

    m_SaveLoadManager = nullptr;

    m_PlCreateManager = nullptr;

    m_MonsterSpawn = nullptr;

    m_DungeonManager = nullptr;
}

void UDiabloGameInstance::Init()
{
    Super::Init();
    UAbilitySystemGlobals::Get().InitGlobalData();

    m_ItemManager = NewObject<UItemManager>();
    m_ItemManager->Init(this);
    m_SaveLoadManager = NewObject<USaveLoadManager>();
    m_PlCreateManager = NewObject<UPlayerCreateManager>();
    m_PlCreateManager->Init(this);
    m_MonsterSpawn = NewObject<UMonsterSpawnManager>();
    m_DungeonManager = NewObject<UDungeonManager>();
    m_DungeonManager->Init();
    m_RewardManager = NewObject<URewardManager>();
    //
    UGameUserSettings::GetGameUserSettings()->SetFrameRateLimit(50.f);
    UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
    
    UKismetSystemLibrary::ControlScreensaver(false);
    //
    m_DungeonManager->GetOnDungeonCreate().AddUObject(m_RewardManager,&URewardManager::EnqueAllActors);
    //

    m_PlayfabManager = NewObject<UPlayfabManager>();

    m_PlayfabManager->Init();

    TBigInt<512,false>  BigIntTest;

    BigIntTest.Set(9999999999999999999);//100경

    PRINTF( "BIGINT BigIntTest:%s",*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(BigIntTest));
    BigIntTest.Multiply(150);
    PRINTF( "BIGINT2 BigIntTest:%s",*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(BigIntTest));
    BigIntTest.Multiply(150);
    PRINTF( "BIGINT3 BigIntTest:%s",*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(BigIntTest));
    BigIntTest.Multiply(150);
    PRINTF( "BIGINT4 BigIntTest:%s",*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(BigIntTest));
    BigIntTest.Multiply(150);
    PRINTF( "BIGINT5 BigIntTest:%s",*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(BigIntTest));
    BigIntTest.Multiply(150);
    PRINTF( "BIGINT6 BigIntTest:%s",*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(BigIntTest));
    BigIntTest.Multiply(150);
    PRINTF( "BIGINT7 BigIntTest:%s",*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(BigIntTest));
    BigIntTest.MultiplyFast(9999);
    PRINTF( "BIGINT8 BigIntTest:%s",*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(BigIntTest));

    uint32* BitBit = BigIntTest.GetBits();

    uint32 SaveBit[2];
    
    SaveBit[0] = BitBit[0];
    SaveBit[1] = BitBit[1];

    TBigInt<512,false> LoadedNewBigInt(BitBit);
    
    PRINTF( "New BigInt:%s",*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(LoadedNewBigInt));
}


FItemInstance UDiabloGameInstance::CreateItem(FName id,int itemLevel)
{
    return m_ItemManager->CreateItemInstance(id,itemLevel);
}

FItemInstance UDiabloGameInstance::CreateItemManual(const FShopItemSell& itemSell)
{
    return m_ItemManager->CreateItemManual(itemSell);
    
}

ADroppedItem* UDiabloGameInstance::DropItemActor(APawn* dropCenterActor, float dropRadius, FItemInstance& myItem)
{
    FVector Loc = dropCenterActor->GetMovementComponent()->GetActorFeetLocation();
    return m_RewardManager->DropItemActor(Loc,dropRadius,myItem);
}

void UDiabloGameInstance::CreateDungeon()
{
    if(UDiabloGameInstance::Get->GetDungeonManager()->IsDungeonOpened())
    {
        UDiabloGameInstance::Get->GetDungeonManager()->ClearDungeon();
    }
    UDiabloGameInstance::Get->GetDungeonManager()->CreateDefaultInfinityDungeon(1);
}

void UDiabloGameInstance::SaveAllPlayerInfo()
{
    TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
    TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();

    USaveLoadManager::Get->SaveInventory(UPlayerCreateManager::Get->m_CurrentSelectSlot,
                                         DiaPC->GetInven()->GetItemAry());

    TArray<FItemInstance> AryEquip;

    for (FEquipSlot* Slot : ADiabloPlayerController::Get->GetEquipment()->GetArySlotPtr())
    {
        AryEquip.Emplace(Slot->m_Item);
    }

    USaveLoadManager::Get->SaveEquipment(UPlayerCreateManager::Get->m_CurrentSelectSlot, AryEquip);

    FText Name = DiaPl->m_TextUnitName;
    int Lev = DiaPl->GetCharacterLevel();
    int SkinIndex = DiaPl->m_SkinIndex;

    USaveLoadManager::Get->SaveCharacterStat(UPlayerCreateManager::Get->m_CurrentSelectSlot, Lev, Name, SkinIndex,
                                             USaveLoadManager::Get->GetCurrentPlayerClassName(), DiaPl->m_fCurrentExp,DiaPl->m_fCurrentGold);

    
    USaveLoadManager::Get->SaveStorage(UPlayerCreateManager::Get->m_CurrentSelectSlot,DiaPC->GetStorageOpenAry(),DiaPC->GetStorageAry());

    USaveLoadManager::Get->SaveShareStorage(DiaPC->GetStorageOpenAry(),DiaPC->GetStorageAry());
}

bool UDiabloGameInstance::IsDungeonOpened()
{
    return GetDungeonManager()->IsDungeonOpened();
}
