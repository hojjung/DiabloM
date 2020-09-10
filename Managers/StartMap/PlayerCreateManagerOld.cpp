#include "PlayerCreateManagerOld.h"
#include "Managers/DiabloGameInstance.h"

PlayerCreateManagerOld* PlayerCreateManagerOld::Get=nullptr;

PlayerCreateManagerOld::PlayerCreateManagerOld(): m_IndexHair(0), m_IndexFace(0), m_IndexArmor(0), m_IndexWeapon(0),
                                            m_IndexItem(0),
                                            m_IndexPerk(0)
{
    PlayerCreateManagerOld::Get=this;
}

PlayerCreateManagerOld::~PlayerCreateManagerOld()
{
    PlayerCreateManagerOld::Get=nullptr;
    m_AryHair.Empty();
    m_AryFace.Empty();
    m_AryArmor.Empty();
    m_AryItem.Empty();
    m_AryPerk.Empty();
    m_CurrentCharData.Clear();
}

void PlayerCreateManagerOld::Init(UDiabloGameInstance* gameInst)
{
    InitArraysFromTable();
    SetArmorFromSetting();
    SetFaceFromSetting();
    SetPerkFromSetting();
    SetItemFromSetting();
    SetHairFromSetting();
}

void PlayerCreateManagerOld::ClearIndex()
{
     m_IndexHair=0;
     m_IndexFace=0;
     m_IndexArmor=0;
     m_IndexWeapon=0;
     m_IndexItem=0;
     m_IndexPerk=0;
}

void PlayerCreateManagerOld::InitArraysFromTable()
{
    UPlayerInitDataTable::GetPlayerHairTable->GetAllRows("FailHair",m_AryHair);
    UPlayerInitDataTable::GetPlayerFaceTable->GetAllRows("FailFace",m_AryFace);
    UPlayerInitDataTable::GetPlayerArmorTable->GetAllRows("FailArmor",m_AryArmor);
    UPlayerInitDataTable::GetPlayerItemTable->GetAllRows("FailItem",m_AryItem);
    UPlayerInitDataTable::GetPlayerPerkTable->GetAllRows("FailPerk",m_AryPerk);
}

void PlayerCreateManagerOld::SetPerkFromSetting()
{
    m_CurrentCharData.m_TextNamePerk = m_AryPerk[m_IndexPerk]->m_ShowingName;
    //m_CurrentCharData.m_CurrentPerk = m_AryPerk[m_IndexPerk];
}

void PlayerCreateManagerOld::SetItemFromSetting()
{
    m_CurrentCharData.m_TextNameItem = m_AryItem[m_IndexItem]->m_ShowingName;
    //m_CurrentCharData.m_CurrentItem = m_AryItem[m_IndexItem];
}

void PlayerCreateManagerOld::SetFaceFromSetting()
{
    m_CurrentCharData.m_CurrentFace = GetFace(m_IndexFace);
    m_CurrentCharData.m_TextNameFace = m_AryFace[m_IndexFace]->m_ShowingName;
}

void PlayerCreateManagerOld::SetHairFromSetting()
{
    bool HasHelmet=m_CurrentCharData.m_CurrentHelmet;
    m_CurrentCharData.m_CurrentHair = GetHair(m_IndexHair,HasHelmet);
    m_CurrentCharData.m_TextNameHair = m_AryHair[m_IndexHair]->m_ShowingName;
}

void PlayerCreateManagerOld::SetArmorFromSetting()
{
    m_CurrentCharData.m_TextNameArmor = m_AryArmor[m_IndexArmor]->m_ShowingName;
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
void PlayerCreateManagerOld::OnDataChanged()
{
    m_OnVisualChange.Broadcast(m_CurrentCharData);
}

void PlayerCreateManagerOld::SetCurrentDataFromSaveFile(const USaveCharacterStatus* char_stat,
    const USaveEquipment* save_equipment)
{
    m_CurrentCharData.m_CurrentBody=save_equipment->m_EquipAry[(int)ESlots::Torso].m_ItemData;
    m_CurrentCharData.m_CurrentHelmet = save_equipment->m_EquipAry[(int)ESlots::Head].m_ItemData;
    m_CurrentCharData.m_CurrentHair=GetHair(char_stat->m_IndexHair,m_CurrentCharData.m_CurrentHelmet);
    m_CurrentCharData.m_CurrentFace=GetFace(char_stat->m_IndexFace);
    m_CurrentCharData.m_CurrentShoe= save_equipment->m_EquipAry[(int)ESlots::Leg].m_ItemData;
    m_CurrentCharData.m_CurrentGlove= save_equipment->m_EquipAry[(int)ESlots::Hand].m_ItemData;
    m_CurrentCharData.m_CurrentShoulder= save_equipment->m_EquipAry[(int)ESlots::Shoulder].m_ItemData;
    m_CurrentCharData.m_CurrentBelt= save_equipment->m_EquipAry[(int)ESlots::Waist].m_ItemData;
    m_CurrentCharData.m_CurrentRightWeapon= save_equipment->m_EquipAry[(int)ESlots::WeaponRight].m_ItemData;
    m_CurrentCharData.m_CurrentLeftWeapon= save_equipment->m_EquipAry[(int)ESlots::WeaponLeft].m_ItemData;

    OnDataChanged();
}

void PlayerCreateManagerOld::DecreaseHair()
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

void PlayerCreateManagerOld::IncreaseHair()
{
    m_IndexHair++;

    if(m_IndexHair>=m_AryHair.Num())
    {
        m_IndexHair=0;
    }
    
    SetHairFromSetting();
    OnDataChanged();
}

void PlayerCreateManagerOld::DecreaseFace()
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

void PlayerCreateManagerOld::IncreaseFace()
{
    m_IndexFace++;

    if(m_IndexFace>=m_AryFace.Num())
    {
        m_IndexFace=0;
    }

    SetFaceFromSetting();
    OnDataChanged();
}

void PlayerCreateManagerOld::DecreaseArmor()
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

void PlayerCreateManagerOld::IncreaseArmor()
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


void PlayerCreateManagerOld::DecreaseItem()
{
    m_IndexItem--;

    if(m_IndexItem<0)
    {
        m_IndexItem=m_AryItem.Num()-1;
    }
    
    SetItemFromSetting();
    OnDataChanged();
}

void PlayerCreateManagerOld::IncreaseItem()
{
    m_IndexItem++;
    
    if(m_IndexItem>=m_AryItem.Num())
    {
        m_IndexItem=0;
    }

    SetItemFromSetting();
    OnDataChanged();
}

void PlayerCreateManagerOld::DecreasePerk()
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

void PlayerCreateManagerOld::IncreasePerk()
{
    m_IndexPerk++;
    
    if(m_IndexPerk>=m_AryPerk.Num())
    {
        m_IndexPerk=0;
    }
    
    SetPerkFromSetting();
    OnDataChanged();
}

void PlayerCreateManagerOld::DoneCreateCharcter()
{
    SaveLoadManagerOld::Get->CreateNewCharacter(this);
}

USkeletalMesh* PlayerCreateManagerOld::GetFace(int index)
{
    return m_AryFace[index]->m_MeshFace;
}

USkeletalMesh* PlayerCreateManagerOld::GetHair(int index,bool hasHelMet)
{
    return !hasHelMet ? m_AryHair[index]->m_MeshFullHair : m_AryHair[index]->m_MeshHalfHair;
}

const FCurrentCharData& PlayerCreateManagerOld::GetCurrentCharData() const
{
    return m_CurrentCharData;
}

