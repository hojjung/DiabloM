#include "PlayerCreateManager.h"
#include "Managers/DiabloGameInstance.h"
#include "SaveLoad/SaveLoadManager.h"

UPlayerCreateManager* UPlayerCreateManager::Get=nullptr;


UPlayerCreateManager::~UPlayerCreateManager()
{
    m_IndexItem=0;
    UPlayerCreateManager::Get=nullptr;
    m_CurrentCharData.Clear();
}

void UPlayerCreateManager::Init(UDiabloGameInstance* gameInst)
{
    UPlayerCreateManager::Get=this;
    InitArraysFromTable();
    SetItemFromSetting();
}

void UPlayerCreateManager::ClearIndex()
{
     m_IndexItem=0;
}

void UPlayerCreateManager::InitArraysFromTable()
{
    UPlayerInitDataTable::GetPlayerItemTable->GetAllRows("FailItem",m_AryInitItem);
}

void UPlayerCreateManager::SetItemFromSetting()
{
    FPlayerEntityTable* ClassEntity=m_AryInitItem[m_IndexItem]->m_PlayerClass.GetRow<FPlayerEntityTable>("");
    m_CurrentCharData.m_ClassID=ClassEntity->m_NameID;
    m_CurrentCharData.m_TextNameClass = ClassEntity->m_ShowingName;
    m_CurrentCharData.m_CurrentSkin=m_AryInitItem[m_IndexItem]->m_CoolMesh;
    m_CurrentCharData.m_CurrentRightWeapon=m_AryInitItem[m_IndexItem]->m_RightHandItem.GetRow<FItemData>("");
    m_CurrentCharData.m_CurrentLeftWeapon=m_AryInitItem[m_IndexItem]->m_LeftHandItem.GetRow<FItemData>("");

    OnDataChanged();
}
//
void UPlayerCreateManager::OnDataChanged()
{
    m_OnVisualChange.Broadcast(m_CurrentCharData);
}

void UPlayerCreateManager::SetCurrentDataFromSaveFile(const USaveCharacterStatus* char_stat,
    const USaveEquipment* save_equipment)
{
    m_CurrentCharData.m_ClassID = char_stat->m_ClassName;
    m_CurrentCharData.m_TextNameClass = UCharacterDataTable::GetPlayerEntity(char_stat->m_ClassName).m_ShowingName;
    m_CurrentCharData.m_CurrentSkin=m_AryInitItem[m_IndexItem]->m_CoolMesh;
    m_CurrentCharData.m_CurrentRightWeapon= save_equipment->m_EquipAry[(int)ESlotsEquipAry::WeaponRight].m_ItemData;
    m_CurrentCharData.m_CurrentLeftWeapon= save_equipment->m_EquipAry[(int)ESlotsEquipAry::WeaponLeft].m_ItemData;

    OnDataChanged();
}


void UPlayerCreateManager::DecreaseItem()
{
    m_IndexItem--;

    if(m_IndexItem<0)
    {
        m_IndexItem=m_AryInitItem.Num()-1;
    }
    
    SetItemFromSetting();
    OnDataChanged();
}

void UPlayerCreateManager::IncreaseItem()
{
    m_IndexItem++;
    
    if(m_IndexItem>=m_AryInitItem.Num())
    {
        m_IndexItem=0;
    }

    SetItemFromSetting();
    OnDataChanged();
}

int UPlayerCreateManager::DoneCreateCharcter()
{
    return USaveLoadManager::Get->CreateNewCharacter(this);
}

const FCurrentCharData& UPlayerCreateManager::GetCurrentCharData() const
{
    return m_CurrentCharData;
}

void UPlayerCreateManager::SetCurrentDataWithPlayer()
{
    UEquipmentSystem* Equip = ADiabloPlayerController::Get->GetEquipment();
    
    m_CurrentCharData.m_CurrentRightWeapon=   Equip->GetItem((int)ESlotsEquipAry::WeaponRight).m_ItemData;
    m_CurrentCharData.m_CurrentLeftWeapon=  Equip->GetItem((int)ESlotsEquipAry::WeaponLeft).m_ItemData;

    OnDataChanged();
}

