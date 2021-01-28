#include "PlayerCreateManager.h"
#include "Managers/DiabloGameInstance.h"
#include "SaveLoad/SaveLoadManager.h"

UPlayerCreateManager* UPlayerCreateManager::Get=nullptr;


UPlayerCreateManager::~UPlayerCreateManager()
{
    m_IndexHair=0;
    m_IndexFace=0;
    m_IndexArmor=0;
    m_IndexWeapon=0;
    m_IndexItem=0;
    m_IndexPerk=0;
    
    UPlayerCreateManager::Get=nullptr;
    m_AryHair.Empty();
    m_AryFace.Empty();
    m_AryArmor.Empty();
    m_AryItem.Empty();
    m_AryPerk.Empty();
    m_CurrentCharData.Clear();
}

void UPlayerCreateManager::Init(UDiabloGameInstance* gameInst)
{
    UPlayerCreateManager::Get=this;
    InitArraysFromTable();
    SetArmorFromSetting();
    SetFaceFromSetting();
    SetPerkFromSetting();
    SetItemFromSetting();
    SetHairFromSetting();
}

void UPlayerCreateManager::ClearIndex()
{
     m_IndexHair=0;
     m_IndexFace=0;
     m_IndexArmor=0;
     m_IndexWeapon=0;
     m_IndexItem=0;
     m_IndexPerk=0;
}

void UPlayerCreateManager::InitArraysFromTable()
{
    UPlayerInitDataTable::GetPlayerHairTable->GetAllRows("FailHair",m_AryHair);
    UPlayerInitDataTable::GetPlayerFaceTable->GetAllRows("FailFace",m_AryFace);
    UPlayerInitDataTable::GetPlayerArmorTable->GetAllRows("FailArmor",m_AryArmor);
    UPlayerInitDataTable::GetPlayerItemTable->GetAllRows("FailItem",m_AryItem);
    UPlayerInitDataTable::GetPlayerPerkTable->GetAllRows("FailPerk",m_AryPerk);
}

void UPlayerCreateManager::SetPerkFromSetting()
{
    m_CurrentCharData.m_TextNamePerk = m_AryPerk[m_IndexPerk]->m_ShowingName;
    //m_CurrentCharData.m_CurrentPerk = m_AryPerk[m_IndexPerk];
}

void UPlayerCreateManager::SetItemFromSetting()
{
    m_CurrentCharData.m_TextNameItem = m_AryItem[m_IndexItem]->m_ShowingName;
    //m_CurrentCharData.m_CurrentItem = m_AryItem[m_IndexItem];
}

void UPlayerCreateManager::SetFaceFromSetting()
{
    m_CurrentCharData.m_CurrentFace = GetFace(m_IndexFace);
    m_CurrentCharData.m_TextNameFace = m_AryFace[m_IndexFace]->m_ShowingName;
}

void UPlayerCreateManager::SetHairFromSetting()
{
    bool HasHelmet=m_CurrentCharData.m_CurrentHelmet;
    m_CurrentCharData.m_CurrentHair = GetHair(m_IndexHair,HasHelmet);
    m_CurrentCharData.m_TextNameHair = m_AryHair[m_IndexHair]->m_ShowingName;
}

void UPlayerCreateManager::SetArmorFromSetting()
{
    FPlayerEntityTable* ClassEntity=m_AryArmor[m_IndexArmor]->m_ClassEntityHandle.GetRow<FPlayerEntityTable>("");
    m_CurrentCharData.m_ClassID=ClassEntity->m_NameID;
    m_CurrentCharData.m_TextNameClass = ClassEntity->m_ShowingName;
    m_CurrentCharData.m_CurrentBody=m_AryArmor[m_IndexArmor]->m_BodyArmorHandle.IsNull() ? nullptr : m_AryArmor[m_IndexArmor]->m_BodyArmorHandle.GetRow<FItemData>("");
    m_CurrentCharData.m_CurrentHelmet=m_AryArmor[m_IndexArmor]->m_HelmetHandle.IsNull() ? nullptr : m_AryArmor[m_IndexArmor]->m_HelmetHandle.GetRow<FItemData>("");        
    m_CurrentCharData.m_CurrentShoe=m_AryArmor[m_IndexArmor]->m_ShoeHandle.IsNull() ? nullptr : m_AryArmor[m_IndexArmor]->m_ShoeHandle.GetRow<FItemData>("");         
    m_CurrentCharData.m_CurrentGlove=m_AryArmor[m_IndexArmor]->m_GloveHandle.IsNull() ? nullptr : m_AryArmor[m_IndexArmor]->m_GloveHandle.GetRow<FItemData>("");        
    m_CurrentCharData.m_CurrentShoulder=m_AryArmor[m_IndexArmor]->m_ShoulderHandle.IsNull() ? nullptr : m_AryArmor[m_IndexArmor]->m_ShoulderHandle.GetRow<FItemData>("");     
    m_CurrentCharData.m_CurrentBelt=m_AryArmor[m_IndexArmor]->m_BeltHandle.IsNull() ? nullptr : m_AryArmor[m_IndexArmor]->m_BeltHandle.GetRow<FItemData>("");         
    //m_CurrentCharData.m_CurrentBackpack=m_AryArmor[m_IndexArmor]->m_BackpackHandle.IsNull() ? nullptr : m_AryArmor[m_IndexArmor]->m_BackpackHandle.GetRow<FItemData>("");     
    m_CurrentCharData.m_CurrentRightWeapon=m_AryArmor[m_IndexArmor]->m_RightWeaponHandle.IsNull() ? nullptr : m_AryArmor[m_IndexArmor]->m_RightWeaponHandle.GetRow<FItemData>("");  
    m_CurrentCharData.m_CurrentLeftWeapon=m_AryArmor[m_IndexArmor]->m_LeftWeaponHandle.IsNull() ? nullptr : m_AryArmor[m_IndexArmor]->m_LeftWeaponHandle.GetRow<FItemData>("");
  
}
//
void UPlayerCreateManager::OnDataChanged()
{
    m_OnVisualChange.Broadcast(m_CurrentCharData);
}

void UPlayerCreateManager::SetCurrentDataFromSaveFile(const USaveCharacterStatus* char_stat,
    const USaveEquipment* save_equipment)
{
    m_CurrentCharData.m_TextNameClass = UCharacterDataTable::GetPlayerEntity(char_stat->m_ClassName).m_ShowingName;
    
    m_CurrentCharData.m_CurrentBody=save_equipment->m_EquipAry[(int)ESlotsEquipAry::Torso].m_ItemData;
    m_CurrentCharData.m_CurrentHelmet = save_equipment->m_EquipAry[(int)ESlotsEquipAry::Head].m_ItemData;
    m_CurrentCharData.m_CurrentHair=GetHair(char_stat->m_IndexHair,m_CurrentCharData.m_CurrentHelmet);
    m_CurrentCharData.m_nIndexHair = char_stat->m_IndexHair;
    m_CurrentCharData.m_CurrentFace=GetFace(char_stat->m_IndexFace);
    m_CurrentCharData.m_nIndexFace = char_stat->m_IndexFace;
    m_CurrentCharData.m_CurrentShoe= save_equipment->m_EquipAry[(int)ESlotsEquipAry::Leg].m_ItemData;
    m_CurrentCharData.m_CurrentGlove= save_equipment->m_EquipAry[(int)ESlotsEquipAry::Hand].m_ItemData;
    m_CurrentCharData.m_CurrentShoulder= save_equipment->m_EquipAry[(int)ESlotsEquipAry::Shoulder].m_ItemData;
    m_CurrentCharData.m_CurrentBelt= save_equipment->m_EquipAry[(int)ESlotsEquipAry::Waist].m_ItemData;
    m_CurrentCharData.m_CurrentRightWeapon= save_equipment->m_EquipAry[(int)ESlotsEquipAry::WeaponRight].m_ItemData;
    m_CurrentCharData.m_CurrentLeftWeapon= save_equipment->m_EquipAry[(int)ESlotsEquipAry::WeaponLeft].m_ItemData;

    OnDataChanged();
}

void UPlayerCreateManager::DecreaseHair()
{
    m_IndexHair--;

    if(m_IndexHair<0)
    {
        m_IndexHair=m_AryHair.Num()-1;
    }
    //text change
    SetHairFromSetting();
    OnDataChanged();
}

void UPlayerCreateManager::IncreaseHair()
{
    m_IndexHair++;

    if(m_IndexHair>=m_AryHair.Num())
    {
        m_IndexHair=0;
    }
    
    SetHairFromSetting();
    OnDataChanged();
}

void UPlayerCreateManager::DecreaseFace()
{
    m_IndexFace--;

    if(m_IndexFace<0)
    {
        m_IndexFace=m_AryFace.Num()-1;
    }
    //text change
    SetFaceFromSetting();
    OnDataChanged();
}

void UPlayerCreateManager::IncreaseFace()
{
    m_IndexFace++;

    if(m_IndexFace>=m_AryFace.Num())
    {
        m_IndexFace=0;
    }

    SetFaceFromSetting();
    OnDataChanged();
}

void UPlayerCreateManager::DecreaseArmor()
{
    m_IndexArmor--;

    if(m_IndexArmor<0)
    {
        m_IndexArmor=m_AryArmor.Num()-1;
    }
    //text change
    SetArmorFromSetting();
    SetHairFromSetting();
    OnDataChanged();
}

void UPlayerCreateManager::IncreaseArmor()
{
    m_IndexArmor++;

    if(m_IndexArmor>=m_AryArmor.Num())
    {
        m_IndexArmor=0;
    }

    SetArmorFromSetting();
    SetHairFromSetting();
    OnDataChanged();
}


void UPlayerCreateManager::DecreaseItem()
{
    m_IndexItem--;

    if(m_IndexItem<0)
    {
        m_IndexItem=m_AryItem.Num()-1;
    }
    
    SetItemFromSetting();
    OnDataChanged();
}

void UPlayerCreateManager::IncreaseItem()
{
    m_IndexItem++;
    
    if(m_IndexItem>=m_AryItem.Num())
    {
        m_IndexItem=0;
    }

    SetItemFromSetting();
    OnDataChanged();
}

void UPlayerCreateManager::DecreasePerk()
{
    m_IndexPerk--;
    
    if(m_IndexPerk<0)
    {
        m_IndexPerk=m_AryPerk.Num()-1;
    }
    //text change
    SetPerkFromSetting();
    OnDataChanged();
}

void UPlayerCreateManager::IncreasePerk()
{
    m_IndexPerk++;
    
    if(m_IndexPerk>=m_AryPerk.Num())
    {
        m_IndexPerk=0;
    }
    
    SetPerkFromSetting();
    OnDataChanged();
}

int UPlayerCreateManager::DoneCreateCharcter()
{
    return USaveLoadManager::Get->CreateNewCharacter(this);
}

USkeletalMesh* UPlayerCreateManager::GetFace(int index)
{
    return m_AryFace[index]->m_MeshFace;
}

USkeletalMesh* UPlayerCreateManager::GetHair(int index,bool hasHelMet)
{
    return !hasHelMet ? m_AryHair[index]->m_MeshFullHair : m_AryHair[index]->m_MeshHalfHair;
}

const FCurrentCharData& UPlayerCreateManager::GetCurrentCharData() const
{
    return m_CurrentCharData;
}

void UPlayerCreateManager::SetCurrentDataWithPlayer()
{
    UEquipmentSystem* Equip = ADiabloPlayerController::Get->GetEquipment();
    
    m_CurrentCharData.m_CurrentBody=Equip->GetItem((int)ESlotsEquipAry::Torso).m_ItemData;
    
    m_CurrentCharData.m_CurrentHelmet = Equip->GetItem((int)ESlotsEquipAry::Head).m_ItemData;
    
    m_CurrentCharData.m_CurrentHair=GetHair(m_CurrentCharData.m_nIndexHair,m_CurrentCharData.m_CurrentHelmet);
    
    m_CurrentCharData.m_CurrentFace=GetFace(m_CurrentCharData.m_nIndexFace);
    
    m_CurrentCharData.m_CurrentShoe=  Equip->GetItem((int)ESlotsEquipAry::Leg).m_ItemData;
    
    m_CurrentCharData.m_CurrentGlove=  Equip->GetItem((int)ESlotsEquipAry::Hand).m_ItemData;
    
    m_CurrentCharData.m_CurrentShoulder=  Equip->GetItem((int)ESlotsEquipAry::Shoulder).m_ItemData;
    
    m_CurrentCharData.m_CurrentBelt=  Equip->GetItem((int)ESlotsEquipAry::Waist).m_ItemData;
    
    m_CurrentCharData.m_CurrentRightWeapon=  Equip->GetItem((int)ESlotsEquipAry::WeaponRight).m_ItemData;
    
    m_CurrentCharData.m_CurrentLeftWeapon=  Equip->GetItem((int)ESlotsEquipAry::WeaponLeft).m_ItemData;

    OnDataChanged();
}

