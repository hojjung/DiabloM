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
    m_CurrentCharData.m_CoolIdle=m_AryInitItem[m_IndexItem]->m_CoolIdle;
    //
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::WeaponRight] =m_AryInitItem[m_IndexItem]->m_RightHandItem.GetRow<FItemData>("");
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::WeaponLeft] =m_AryInitItem[m_IndexItem]->m_LeftHandItem.GetRow<FItemData>("");
    OnDataChanged();
}
//
void UPlayerCreateManager::OnDataChanged()
{
    m_OnVisualChange.Broadcast(m_CurrentCharData);
}

void UPlayerCreateManager::SetCurrentDataFromSaveFile()
{
    const USaveEquipment* save_equipment =  USaveLoadManager::Get->GetLoadedEquip()[m_CurrentSelectSlot];
    const USaveCharacterStatus* char_stat =USaveLoadManager::Get->GetLoadedChars()[m_CurrentSelectSlot];
    const FPlayerEntityTable& PlayerEntity = UCharacterDataTable::GetPlayerEntity(char_stat->m_ClassName);
    
    m_CurrentCharData.m_ClassID = char_stat->m_ClassName;
    m_CurrentCharData.m_TextNameClass = PlayerEntity.m_ShowingName;
    
    m_CurrentCharData.m_CurrentSkin=PlayerEntity.m_AryPlayerSkin[char_stat->m_IndexSkin];
    
    m_CurrentCharData.m_CoolIdle=m_AryInitItem[m_IndexItem]->m_CoolIdle;
    //
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Head       ] = save_equipment->m_EquipAry[(int)ESlotsEquipAry::Head].m_ItemData;
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Neck       ] = save_equipment->m_EquipAry[(int)ESlotsEquipAry::Neck].m_ItemData;
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Torso      ] = save_equipment->m_EquipAry[(int)ESlotsEquipAry::Torso].m_ItemData;
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Waist      ] = save_equipment->m_EquipAry[(int)ESlotsEquipAry::Waist].m_ItemData;
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Leg        ] = save_equipment->m_EquipAry[(int)ESlotsEquipAry::Leg].m_ItemData;
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Hand       ] = save_equipment->m_EquipAry[(int)ESlotsEquipAry::Hand].m_ItemData;
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Shoulder   ] = save_equipment->m_EquipAry[(int)ESlotsEquipAry::Shoulder].m_ItemData;
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::WeaponRight] = save_equipment->m_EquipAry[(int)ESlotsEquipAry::WeaponRight].m_ItemData;
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::WeaponLeft ] = save_equipment->m_EquipAry[(int)ESlotsEquipAry::WeaponLeft].m_ItemData;
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::FingerRight] =  save_equipment->m_EquipAry[(int)ESlotsEquipAry::FingerRight].m_ItemData;
    m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::FingerLeft ] = save_equipment->m_EquipAry[(int)ESlotsEquipAry::FingerLeft].m_ItemData;
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
   
   m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Head       ]   = Equip->GetItem(ESlotsEquipAry::Head).m_ItemData;
   m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Neck       ]  = Equip->GetItem(ESlotsEquipAry::Neck).m_ItemData;
   m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Torso      ]  = Equip->GetItem(ESlotsEquipAry::Torso).m_ItemData;
   m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Waist      ]  = Equip->GetItem(ESlotsEquipAry::Waist).m_ItemData;
   m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Leg        ]  = Equip->GetItem(ESlotsEquipAry::Leg).m_ItemData;
   m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Hand       ]  = Equip->GetItem(ESlotsEquipAry::Hand).m_ItemData;
   m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::Shoulder   ]  = Equip->GetItem(ESlotsEquipAry::Shoulder).m_ItemData;
   m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::WeaponRight]   = Equip->GetItem(ESlotsEquipAry::WeaponRight).m_ItemData;
   m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::WeaponLeft ]   = Equip->GetItem(ESlotsEquipAry::WeaponLeft).m_ItemData;
   m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::FingerRight]  = Equip->GetItem(ESlotsEquipAry::FingerRight).m_ItemData;
   m_CurrentCharData.m_AryEquipItemData[(int)ESlotsEquipAry::FingerLeft ]  = Equip->GetItem(ESlotsEquipAry::FingerLeft).m_ItemData;

    PRINTF("UPlayerCreateManager::SetCurrentDataWithPlayer");
    OnDataChanged();
}

