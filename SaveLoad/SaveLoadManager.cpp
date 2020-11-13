#include "SaveLoadManager.h"
#include "Characters/DiabloPlayerController.h"
#include "DiabloM.h"
#include "SaveEquipment.h"
#include "SaveShareStorage.h"
#include "Kismet/GameplayStatics.h"
#include "SaveLoad/SaveInventory.h"
#include "SaveLoad/SaveCharacterStatus.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Item/ItemManager.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/StartMap/PlayerCreateManager.h"
#include "SaveStorage.h"

USaveLoadManager* USaveLoadManager::Get = nullptr;


USaveLoadManager::USaveLoadManager():
    m_InvenSlotName("Inventory"),
    m_EquipSlotName("Equipment"),
    m_CharSlotName("Character"),
    m_StorageSlotName("Storage"),
    m_StorageShareSlotName("ShareStorage"),
    m_nMaxSlotCount(7),
    m_nCurrentSlotCount(5),m_LoadShareStorage(nullptr)
{
    USaveLoadManager::Get = this;
    m_AryLoadedCharacters.Init(nullptr, m_nCurrentSlotCount);
    m_AryLoadedEquipments.Init(nullptr, m_nCurrentSlotCount);
    m_AryLoadedInventory.Init(nullptr, m_nCurrentSlotCount);
    m_AryLoadedStorage.Init(nullptr, m_nCurrentSlotCount);
    TryLoadAllCharacter();
    
}

USaveLoadManager::~USaveLoadManager()
{
    USaveLoadManager::Get = nullptr;
    m_AryLoadedCharacters.Empty();
    m_AryLoadedEquipments.Empty();
    m_AryLoadedInventory.Empty();
    m_AryLoadedStorage.Empty();
    m_OnDataCreated.Unbind();
}

void USaveLoadManager::TryLoadAllCharacter()
{
    LoadShareStorage();
    for (int i = 0; i < m_nCurrentSlotCount; i++)
    {
        if (!DoesSaveDataExist(i))
        {
            PRINTF("Fail: %d", i);
        }
        else
        {
            LoadCharStat(i);
            LoadEquipment(i);
            LoadInventory(i);
            LoadStorage(i);

            PRINTF("Success: %d", i);
        }
    }

    
}

void USaveLoadManager::DeleteSlot(int i)
{
    PRINTF("DeleteSlot-SaveMag");
    if (!UGameplayStatics::DeleteGameInSlot(m_EquipSlotName, i))
    {
        //PRINTF("Fail Delete Equipment");
    }
    if (!UGameplayStatics::DeleteGameInSlot(m_CharSlotName, i))
    {
        //PRINTF("Fail Delete Character");
    }
    if (!UGameplayStatics::DeleteGameInSlot(m_InvenSlotName, i))
    {
        //PRINTF("Fail Delete InventoryOld");
    }
    if (!UGameplayStatics::DeleteGameInSlot(m_StorageSlotName, i))
    {
        //PRINTF("Fail Delete InventoryOld");
    }
    if (!UGameplayStatics::DeleteGameInSlot(m_StorageShareSlotName, 0))
    {
        //PRINTF("Fail Delete InventoryOld");
    }

    m_AryLoadedCharacters[i] = nullptr;
    m_AryLoadedEquipments[i] = nullptr;
    m_AryLoadedInventory[i] = nullptr;
    m_AryLoadedStorage[i] = nullptr;
    m_LoadShareStorage=nullptr;
}

void USaveLoadManager::DeleteAllSlot()
{
    PRINTF("DeleteAllSlot");
    for (int i = 0; i < m_nCurrentSlotCount; i++)
    {
        DeleteSlot(i);
    }
}

void USaveLoadManager::SaveInventory(int slotIndex, const TArray<FItemInstance>& aryItem)
{
    USaveInventory* SaveInven = Cast<USaveInventory>(
        UGameplayStatics::CreateSaveGameObject(USaveInventory::StaticClass()));

    SaveInven->m_SaveVersion = m_SaveVersion;

    SaveInven->SetSaveData(aryItem);

    UGameplayStatics::SaveGameToSlot(SaveInven, m_InvenSlotName, slotIndex);

    m_AryLoadedInventory[slotIndex] = SaveInven;

    PRINTF("SaveInventory");
}

void USaveLoadManager::LoadInventory(int slotIndex)
{
    USaveInventory* LoadInven = Cast<USaveInventory>(
        UGameplayStatics::LoadGameFromSlot(m_InvenSlotName, slotIndex));

    LoadItemDataForInstance(LoadInven->m_InvenAry,LoadInven->m_SaveVersion);
    
    m_AryLoadedInventory[slotIndex] = LoadInven;

    PRINTF("LoadInventory");
}

void USaveLoadManager::SaveEquipment(int slotIndex, const TArray<FItemInstance>& aryItem)
{
    USaveEquipment* SaveEquip = Cast<USaveEquipment>(UGameplayStatics::CreateSaveGameObject(USaveEquipment::StaticClass()));

    SaveEquip->m_SaveVersion = m_SaveVersion;

    SaveEquip->SetEquipSaveData(aryItem);

    UGameplayStatics::SaveGameToSlot(SaveEquip, m_EquipSlotName, slotIndex);

    m_AryLoadedEquipments[slotIndex] = SaveEquip;
    
    PRINTF("SaveEquipment");
}

void USaveLoadManager::CreateSetEquipSlotItem(TArray<FItemInstance>& arrayUsing, FName itemId, ESlotsEquipAry slot)
{
    TWeakObjectPtr<UItemManager> ItemManager = UDiabloGameInstance::Get->GetItemManager();
    int GridIndex = static_cast<int>(slot);
    arrayUsing[GridIndex] = ItemManager->CreateItemInstance(itemId, 1);
    arrayUsing[GridIndex].m_nGridIndex = GridIndex;
    arrayUsing[GridIndex].m_Holder = nullptr;
}

void USaveLoadManager::SetEquipSaveDataFromCreation(const FCurrentCharData& charData,TArray<FItemInstance>& newEquipAry)
{
    newEquipAry.Reserve(15);
    newEquipAry.Init(FItemInstance(), static_cast<int>(ESlotsEquipAry::Length));
    //9

    if (charData.m_CurrentHelmet)
    {
        CreateSetEquipSlotItem(newEquipAry, charData.m_CurrentHelmet->m_ItemID, ESlotsEquipAry::Head);
    }
    if (charData.m_CurrentBody)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentBody->m_ItemID, ESlotsEquipAry::Torso);
    }
    if (charData.m_CurrentGlove)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentGlove->m_ItemID, ESlotsEquipAry::Hand);
    }
    if (charData.m_CurrentShoe)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentShoe->m_ItemID, ESlotsEquipAry::Leg);
    }
    if (charData.m_CurrentShoulder)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentShoulder->m_ItemID, ESlotsEquipAry::Shoulder);
    }
    //if(charData.m_CurrentBackpack)
    {
        //TODO BackPack? 
    }
    if (charData.m_CurrentBelt)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentBelt->m_ItemID, ESlotsEquipAry::Waist);
    }
    
    if (charData.m_CurrentRightWeapon)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentRightWeapon->m_ItemID, ESlotsEquipAry::WeaponRight);
    }
    
    if (charData.m_CurrentLeftWeapon)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentLeftWeapon->m_ItemID, ESlotsEquipAry::WeaponLeft);
    }
}

void USaveLoadManager::LoadEquipment(int slotIndex)
{
    USaveEquipment* LoadEquip = Cast<USaveEquipment>(UGameplayStatics::LoadGameFromSlot(m_EquipSlotName, slotIndex));

    LoadItemDataForInstance(LoadEquip->m_EquipAry,LoadEquip->m_SaveVersion);
    
    m_AryLoadedEquipments[slotIndex] = LoadEquip;

    PRINTF("LoadEquipment");
}

void USaveLoadManager::SaveCharacterStat(int slotIndex, int level, FText nameText, int faceIndex, int hairIndex,FName classID,float exp,float gold)
{
    USaveCharacterStatus* SaveCharStat = Cast<USaveCharacterStatus>(
        UGameplayStatics::CreateSaveGameObject(USaveCharacterStatus::StaticClass()));

    SaveCharStat->m_nSlotIndex = slotIndex;
    SaveCharStat->m_nLevel = level;
    SaveCharStat->m_TextName = nameText.ToString();
    SaveCharStat->m_SaveVersion = m_SaveVersion;
    SaveCharStat->m_IndexFace = faceIndex;
    SaveCharStat->m_IndexHair = hairIndex;
    SaveCharStat->m_fExp=exp;
    SaveCharStat->m_ClassName=classID;
    SaveCharStat->m_fGold = gold;
    //m_ClassName

    UGameplayStatics::SaveGameToSlot(SaveCharStat, m_CharSlotName, slotIndex);

    m_AryLoadedCharacters[slotIndex] = SaveCharStat;

    PRINTF("SaveCharacter");
}

void USaveLoadManager::LoadCharStat(int index)
{
    USaveCharacterStatus* LoadCharStat = Cast<USaveCharacterStatus>(UGameplayStatics::LoadGameFromSlot(m_CharSlotName, index));

    switch (LoadCharStat->m_SaveVersion)
    {
    case ESaveVersion::Init:
        break;

    default: ;
    }

    m_AryLoadedCharacters[index] = LoadCharStat;
    
    PRINTF("LoadCharacter");
}

void USaveLoadManager::SaveStorage(int slotIndex, const TArray<bool>& aryOpen,
    const TArray<TArray<FItemInstance>>& aryItems)
{
    USaveStorage* SaveStorage = Cast<USaveStorage>(
     UGameplayStatics::CreateSaveGameObject(USaveStorage::StaticClass()));

    SaveStorage->SetSaveStorage(m_SaveVersion,aryOpen,aryItems);

    UGameplayStatics::SaveGameToSlot(SaveStorage, m_StorageSlotName, slotIndex);

    m_AryLoadedStorage[slotIndex] = SaveStorage;
    //
 
    
    PRINTF("SaveStorage");
}

void USaveLoadManager::SaveShareStorage( const TArray<bool>& aryOpen,
    const TArray<TArray<FItemInstance>>& aryItems)
{
    //
    USaveShareStorage* SaveShareStorage = Cast<USaveShareStorage>(
     UGameplayStatics::CreateSaveGameObject(USaveShareStorage::StaticClass()));
    //
    SaveShareStorage->SetSaveShareStorage(m_SaveVersion,aryOpen,aryItems);
    //
    UGameplayStatics::SaveGameToSlot(SaveShareStorage, m_StorageShareSlotName, 0);
    //
    m_LoadShareStorage=SaveShareStorage;
    //
    PRINTF("SaveShareStorage");
}

void USaveLoadManager::LoadStorage(int slotIndex)
{
    USaveStorage* LoadStorage = Cast<USaveStorage>(UGameplayStatics::LoadGameFromSlot(m_StorageSlotName, slotIndex));

    if(!LoadStorage)//for ALready Made Player
    {
        PRINTF("No StorageLoad-CreateNew");
        TArray<TArray<FItemInstance>> AryAryStorage;
        AryAryStorage.Init(TArray<FItemInstance>(),3);
        int i=0;
        while (i<3)
        {
            AryAryStorage[i].Init(FItemInstance(),INVEN_X*INVEN_Y);

            i++;
        }
        //Open bool
        TArray<bool> AryDgOpen;
        AryDgOpen.Init(false,5);
        AryDgOpen[0]=true;
        SaveStorage(slotIndex,AryDgOpen,AryAryStorage);
        LoadStorage=m_AryLoadedStorage[slotIndex];
    }

    LoadItemDataForInstance(LoadStorage->m_AryStorageItems1,LoadStorage->m_SaveVersion);
    LoadItemDataForInstance(LoadStorage->m_AryStorageItems2,LoadStorage->m_SaveVersion);
    LoadItemDataForInstance(LoadStorage->m_AryStorageItems3,LoadStorage->m_SaveVersion);
    
    m_AryLoadedStorage[slotIndex] = LoadStorage;

    PRINTF("LoadStorage");
}

void USaveLoadManager::LoadShareStorage()
{
    USaveShareStorage* LoadShareStorage = Cast<USaveShareStorage>(UGameplayStatics::LoadGameFromSlot(m_StorageShareSlotName, 0));

    if(!LoadShareStorage)//for ALready Made Player
      {
        PRINTF("No ShareStorageLoad-CreateNew");
        TArray<TArray<FItemInstance>> AryAryStorage;
        AryAryStorage.Init(TArray<FItemInstance>(),5);
        AryAryStorage[3].Init(FItemInstance(),STORAGE_X*STORAGE_Y);
        AryAryStorage[4].Init(FItemInstance(),STORAGE_X*STORAGE_Y);
        //Open bool
        TArray<bool> AryDgOpen;
        AryDgOpen.Init(false,5);
        //
        SaveShareStorage(AryDgOpen,AryAryStorage);
        LoadShareStorage=m_LoadShareStorage;
        //계정 공유 bool 배열 어디에서?
      }

    LoadItemDataForInstance(LoadShareStorage->m_AryStorageItems4,LoadShareStorage->m_SaveVersion);
    LoadItemDataForInstance(LoadShareStorage->m_AryStorageItems5,LoadShareStorage->m_SaveVersion);
    
    m_LoadShareStorage = LoadShareStorage;

    PRINTF("LoadShareStorage");
}

bool USaveLoadManager::DoesSaveDataExist(int slotIndex)
{
    bool CharResult = UGameplayStatics::DoesSaveGameExist(m_CharSlotName, slotIndex);

    return CharResult;
}

int USaveLoadManager::CreateNewCharacter(UPlayerCreateManager* plManager)
{
    int PlayerIndex = GetEmptyIndex();
    int MaxInven = INVEN_X*INVEN_Y;
    //
    if (PlayerIndex == -1)
    {
        PRINTF("Empty Index: %d", PlayerIndex);
        return -1;
    }
    //
    TArray<FItemInstance> AryEquip;
    SetEquipSaveDataFromCreation(plManager->GetCurrentCharData(),AryEquip);
    SaveEquipment(PlayerIndex,AryEquip);
    SaveCharacterStat(PlayerIndex,1,plManager->m_CurrentTextName,plManager->m_IndexFace,plManager->m_IndexHair,plManager->GetCurrentCharData().m_ClassID);
    
    TArray<FItemInstance> AryInven;
    AryInven.Init(FItemInstance(),MaxInven);
    SaveInventory(PlayerIndex,AryInven);
    //
    TArray<TArray<FItemInstance>> AryAryStorage;
    AryAryStorage.Init(TArray<FItemInstance>(),5);
    int i=0;
    while (i<5)
    {
        AryAryStorage[i].Init(FItemInstance(),MaxInven);

        i++;
    }
    TArray<bool> AryDgOpen;
    AryDgOpen.Init(false,5);
    AryDgOpen[0]=true;
    SaveStorage(PlayerIndex,AryDgOpen,AryAryStorage);
    //
    m_OnDataCreated.ExecuteIfBound(m_AryLoadedCharacters[PlayerIndex]);

    return PlayerIndex;
}

void USaveLoadManager::SetLoadedEquipDataToPlayer(int slotIndex)
{
    TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
    DiaPC->GetEquipment()->SetItemAry(m_AryLoadedEquipments[slotIndex]->m_EquipAry);
}

void USaveLoadManager::SetLoadedCharDataToPlayer(int slotIndex)
{
    TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
    TWeakObjectPtr<APlayerDiabloCharacter> DiaPl = DiaPC->GetPlayerPawn();
    DiaPl->SetLoadedData(m_AryLoadedCharacters[slotIndex]);
}

void USaveLoadManager::SetLoadedInvenDataToPlayer(int slotIndex)
{
    TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
    DiaPC->GetInven()->SetItemAry(m_AryLoadedInventory[slotIndex]->m_InvenAry);
}

void USaveLoadManager::SetLoadedStorageDataToPlayer(int slot_index)
{
    TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
    //
    DiaPC->GetStorageAry()[0]->SetItemAry(m_AryLoadedStorage[slot_index]->m_AryStorageItems1);
    DiaPC->GetStorageAry()[1]->SetItemAry(m_AryLoadedStorage[slot_index]->m_AryStorageItems2);
    DiaPC->GetStorageAry()[2]->SetItemAry(m_AryLoadedStorage[slot_index]->m_AryStorageItems3);
    DiaPC->GetStorageAry()[3]->SetItemAry(m_LoadShareStorage->m_AryStorageItems4);
    DiaPC->GetStorageAry()[4]->SetItemAry(m_LoadShareStorage->m_AryStorageItems5);
    //
    auto& AryDgOpen =DiaPC->GetStorageOpenAry();
    AryDgOpen[0]=m_AryLoadedStorage[slot_index]->m_bShareStorage1Opened;
    AryDgOpen[1]=m_AryLoadedStorage[slot_index]->m_bShareStorage2Opened;
    AryDgOpen[2]=m_AryLoadedStorage[slot_index]->m_bShareStorage3Opened;
    AryDgOpen[3]=m_LoadShareStorage->m_bShareStorage4Opened;
    AryDgOpen[4]=m_LoadShareStorage->m_bShareStorage5Opened;
    
}

void USaveLoadManager::CreateSetPlayerCharacter()
{
    int slotIndex=UPlayerCreateManager::Get->m_CurrentSelectSlot;
    SetLoadedCharDataToPlayer(slotIndex);
    SetLoadedEquipDataToPlayer(slotIndex);
    SetLoadedInvenDataToPlayer(slotIndex);
    SetLoadedStorageDataToPlayer(slotIndex);
}

const TArray<USaveCharacterStatus*>& USaveLoadManager::GetLoadedChars() const
{
    return m_AryLoadedCharacters;
}

const TArray<USaveEquipment*>& USaveLoadManager::GetLoadedEquip() const
{
    return m_AryLoadedEquipments;
}

const TArray<USaveInventory*>& USaveLoadManager::GetLoadedInven() const
{
    return m_AryLoadedInventory;
}

const TArray<USaveStorage*>& USaveLoadManager::GetLoadedStorage() const
{
    return m_AryLoadedStorage;
}

int USaveLoadManager::GetEmptyIndex()
{
    for (int i = 0; i < m_AryLoadedCharacters.Num(); i++)
    {
        if (m_AryLoadedCharacters[i] == nullptr)
        {
            return i;
        }
    }

    return -1;
}

void USaveLoadManager::LoadItemDataForInstance(TArray<FItemInstance>& itemAry,ESaveVersion version)
{
    for (auto& ItemInst : itemAry)
    {
        if (ItemInst.m_ItemID == NAME_None)
        {
            continue;
        }

        ItemInst.m_ItemData = UItemDataTable::GetItemDataPtr(ItemInst.m_ItemID);

        if(ItemInst.m_ItemData==nullptr)
        {
            ItemInst.m_ItemData = UItemDataTable::GetUniqueItemPtr(ItemInst.m_ItemID);    
        }
        
        ItemInst.m_ItemTier = UItemDataTable::GetItemTierPtr(ItemInst.m_TierID);

        // if(ItemInst.m_ItemTier==nullptr)
        // {
        //     switch (version)
        //     {
        //         
        //     }
        // }

        for(auto& OO: ItemInst.m_AryOptions)
        {
            OO.m_DataOption=UOptionDataTable::GetOptionPtr(OO.m_OptionID);
        }
    }
}

FName USaveLoadManager::GetCurrentPlayerClassName()
{
    return m_AryLoadedCharacters[UPlayerCreateManager::Get->m_CurrentSelectSlot]->m_ClassName;
}
