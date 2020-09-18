#include "SaveLoadManager.h"
#include "Characters/DiabloPlayerController.h"
#include "DiabloM.h"
#include "SaveEquipment.h"
#include "Kismet/GameplayStatics.h"
#include "SaveLoad/SaveInventory.h"
#include "SaveLoad/SaveCharacterStatus.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Item/ItemManager.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/StartMap/PlayerCreateManager.h"


USaveLoadManager* USaveLoadManager::Get = nullptr;


USaveLoadManager::USaveLoadManager():
    m_InvenSlotName("Inventory"),
    m_EquipSlotName("Equipment"),
    m_CharSlotName("Character"),
    m_nMaxSlotCount(7),
    m_nCurrentSlotCount(5)
{
    USaveLoadManager::Get = this;
    m_AryLoadedCharacters.Init(nullptr, m_nCurrentSlotCount);
    m_AryLoadedEquipments.Init(nullptr, m_nCurrentSlotCount);
    m_AryLoadedInventory.Init(nullptr, m_nCurrentSlotCount);
    TryLoadAllCharacter();
}

USaveLoadManager::~USaveLoadManager()
{
    USaveLoadManager::Get = nullptr;
    m_AryLoadedCharacters.Empty();
    m_AryLoadedEquipments.Empty();
    m_AryLoadedInventory.Empty();
    m_OnDataCreated.Unbind();
}

void USaveLoadManager::TryLoadAllCharacter()
{
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

            PRINTF("Success: %d", i);
        }
    }
}

void USaveLoadManager::DeleteSlot(int i)
{
    if (!UGameplayStatics::DeleteGameInSlot("Equipment", i))
    {
        //PRINTF("Fail Delete Equipment");
    }
    if (!UGameplayStatics::DeleteGameInSlot("Character", i))
    {
        //PRINTF("Fail Delete Character");
    }
    if (!UGameplayStatics::DeleteGameInSlot("InventoryOld", i))
    {
        //PRINTF("Fail Delete InventoryOld");
    }

    m_AryLoadedCharacters[i] = nullptr;
    m_AryLoadedEquipments[i] = nullptr;
    m_AryLoadedInventory[i] = nullptr;
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


    switch (LoadInven->m_SaveVersion)
    {
    case ESaveVersion::Init:
        break;

    default: ;
    }

    LoadItemDataForInstance(LoadInven->m_InvenAry);
    
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

void USaveLoadManager::CreateSetEquipSlotItem(TArray<FItemInstance>& arrayUsing, FName itemId, ESlots slot)
{
    TWeakObjectPtr<UItemManager> ItemManager = UDiabloGameInstance::Get->m_ItemManager;
    int GridIndex = static_cast<int>(slot);
    arrayUsing[GridIndex] = ItemManager->CreateItemInstance(itemId, 1);
    arrayUsing[GridIndex].m_nGridIndex = GridIndex;
    arrayUsing[GridIndex].m_Holder = nullptr;
}

void USaveLoadManager::SetEquipSaveDataFromCreation(const FCurrentCharData& charData,TArray<FItemInstance>& newEquipAry)
{
    newEquipAry.Reserve(15);
    newEquipAry.Init(FItemInstance(), static_cast<int>(ESlots::Length));
    //9

    if (charData.m_CurrentHelmet)
    {
        CreateSetEquipSlotItem(newEquipAry, charData.m_CurrentHelmet->m_ItemID, ESlots::Head);
    }
    if (charData.m_CurrentBody)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentBody->m_ItemID, ESlots::Torso);
    }
    if (charData.m_CurrentGlove)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentGlove->m_ItemID, ESlots::Hand);
    }
    if (charData.m_CurrentShoe)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentShoe->m_ItemID, ESlots::Leg);
    }
    if (charData.m_CurrentShoulder)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentShoulder->m_ItemID, ESlots::Shoulder);
    }
    //if(charData.m_CurrentBackpack)
    {
        //TODO BackPack? 
    }
    if (charData.m_CurrentBelt)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentBelt->m_ItemID, ESlots::Waist);
    }
    
    if (charData.m_CurrentRightWeapon)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentRightWeapon->m_ItemID, ESlots::WeaponRight);
    }
    
    if (charData.m_CurrentLeftWeapon)
    {
        CreateSetEquipSlotItem(newEquipAry,charData.m_CurrentLeftWeapon->m_ItemID, ESlots::WeaponLeft);
    }
}

void USaveLoadManager::LoadEquipment(int slotIndex)
{
    USaveEquipment* LoadEquip = Cast<USaveEquipment>(UGameplayStatics::LoadGameFromSlot(m_EquipSlotName, slotIndex));

    switch (LoadEquip->m_SaveVersion)
    {
    case ESaveVersion::Init:
        break;

    default: ;
    }

    LoadItemDataForInstance(LoadEquip->m_EquipAry);
    
    m_AryLoadedEquipments[slotIndex] = LoadEquip;

    PRINTF("LoadEquipment");
}
void USaveLoadManager::SaveCharacterStat(int slotIndex, int level, FText nameText, int faceIndex, int hairIndex)
{
    USaveCharacterStatus* SaveCharStat = Cast<USaveCharacterStatus>(
        UGameplayStatics::CreateSaveGameObject(USaveCharacterStatus::StaticClass()));

    SaveCharStat->m_nSlotIndex = slotIndex;
    SaveCharStat->m_nLevel = level;
    SaveCharStat->m_TextName = nameText.ToString();
    SaveCharStat->m_SaveVersion = m_SaveVersion;
    SaveCharStat->m_IndexFace = faceIndex;
    SaveCharStat->m_IndexHair = hairIndex;

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

bool USaveLoadManager::DoesSaveDataExist(int slotIndex)
{
    bool CharResult = UGameplayStatics::DoesSaveGameExist(m_CharSlotName, slotIndex);

    bool InvenResult = UGameplayStatics::DoesSaveGameExist(m_InvenSlotName, slotIndex);

    bool EquipResult = UGameplayStatics::DoesSaveGameExist(m_EquipSlotName, slotIndex);

    return CharResult && InvenResult && EquipResult;
}

int USaveLoadManager::CreateNewCharacter(UPlayerCreateManager* plManager)
{
    int PlayerIndex = GetEmptyIndex();
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
    SaveCharacterStat(PlayerIndex,1,plManager->m_CurrentTextName,plManager->m_IndexFace,plManager->m_IndexHair);
    TArray<FItemInstance> AryInven;
    AryInven.Init(FItemInstance(),40);
    SaveInventory(PlayerIndex,AryInven);
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

void USaveLoadManager::CreateSetPlayerCharacter()
{
    int slotIndex=UPlayerCreateManager::Get->m_CurrentSelectSlot;
    SetLoadedCharDataToPlayer(slotIndex);
    SetLoadedEquipDataToPlayer(slotIndex);
    SetLoadedInvenDataToPlayer(slotIndex);
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

void USaveLoadManager::LoadItemDataForInstance(TArray<FItemInstance>& itemAry)
{
    for (auto& ItemInst : itemAry)
    {
        if (ItemInst.m_ItemID == NAME_None)
        {
            continue;
        }
        ItemInst.m_ItemData = UItemDataTable::GetItemDataPtr(ItemInst.m_ItemID);
        ItemInst.m_ItemTier = UItemDataTable::GetItemTierPtr(ItemInst.m_TierID);
    }
}
