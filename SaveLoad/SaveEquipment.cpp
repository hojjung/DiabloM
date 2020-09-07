// ReSharper disable All
#include "SaveEquipment.h"
#include "DiabloM.h"


void USaveEquipment::SetEquipSaveData(const TArray<FEquipSlot*>& equip)
{
    for(FEquipSlot* AA : equip)
    {
        m_EquipAry.Emplace(AA->m_Item);
    }
}

void USaveEquipment::SetEquipLoadData(EquipmentSystem** outEquip)
{
    PRINTF("EquipLoaded:%d",m_EquipAry.Num());

    for(auto& ItemLoad : m_EquipAry)
    {
        if(ItemLoad.m_ItemID == NAME_None)
        {
            continue;
        }
        ItemLoad.m_ItemData = UItemDataTable::GetItemDataPtr(ItemLoad.m_ItemID);//id가 저장이안됨
    }
   
    (*outEquip)->SetItemAry(m_EquipAry);
}

void USaveEquipment::SetEquipSaveDataFromCreation(const FCurrentCharData& charData)
{
    m_EquipAry.Reserve(10);
    m_EquipAry.Init(FItemInstance(),(int)EItemType::Length);
    //9
    if(!charData.m_CurrentArmor->m_HelmetHandle.IsNull())
    {
        m_EquipAry[(int)ESlots::Head] = FItemInstance( charData.m_CurrentArmor->m_HelmetHandle.GetRow<FItemData>(""),(int)ESlots::Head,nullptr); 
    }
    if(!charData.m_CurrentArmor->m_BodyArmorHandle.IsNull())
    {
        m_EquipAry[(int)ESlots::Torso] = FItemInstance( charData.m_CurrentArmor->m_BodyArmorHandle.GetRow<FItemData>(""),(int)ESlots::Torso,nullptr);  
    }
    if(!charData.m_CurrentArmor->m_GloveHandle.IsNull())
    {
        m_EquipAry[(int)ESlots::Hand] = FItemInstance( charData.m_CurrentArmor->m_GloveHandle.GetRow<FItemData>(""),(int)ESlots::Hand,nullptr); 
    }
    if(!charData.m_CurrentArmor->m_ShoeHandle.IsNull())
    {
        m_EquipAry[(int)ESlots::Leg] = FItemInstance( charData.m_CurrentArmor->m_ShoeHandle.GetRow<FItemData>(""),(int)ESlots::Leg,nullptr); 
    }
    if(!charData.m_CurrentArmor->m_Shoulderandle.IsNull())
    {
        m_EquipAry[(int)ESlots::Shoulder] = FItemInstance( charData.m_CurrentArmor->m_Shoulderandle.GetRow<FItemData>(""),(int)ESlots::Shoulder,nullptr); 
    }
    if(!charData.m_CurrentArmor->m_BackpackHandle.IsNull())
    {
        //TODO BackPack? 
    }
    if(!charData.m_CurrentArmor->m_BeltHandle.IsNull())
    {
        m_EquipAry[(int)ESlots::Waist] = FItemInstance( charData.m_CurrentArmor->m_BeltHandle.GetRow<FItemData>(""),(int)ESlots::Waist,nullptr); 
    }
    if(!charData.m_CurrentArmor->m_RightWeaponHandle.IsNull())
    {
        m_EquipAry[(int)ESlots::WeaponRight] = FItemInstance( charData.m_CurrentArmor->m_RightWeaponHandle.GetRow<FItemData>(""),(int)ESlots::WeaponRight,nullptr); 
    }
    if(!charData.m_CurrentArmor->m_LeftWeaponHandle.IsNull())
    {
        m_EquipAry[(int)ESlots::WeaponLeft] = FItemInstance( charData.m_CurrentArmor->m_LeftWeaponHandle.GetRow<FItemData>(""),(int)ESlots::WeaponLeft,nullptr); 
    }
    
}

