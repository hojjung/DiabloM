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

void USaveEquipment::SetEquipLoadData(EquipmentSystemOld** outEquip)
{
    PRINTF("EquipLoaded:%d",m_EquipAry.Num());

    for(FItemInstance& ItemLoad : m_EquipAry)
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
    m_EquipAry.Init(FItemInstance(),(int)ESlots::Length);
    //9
    if(charData.m_CurrentHelmet)
    {
        m_EquipAry[(int)ESlots::Head] = FItemInstance(charData.m_CurrentHelmet,(int)ESlots::Head,nullptr); 
    }
    if(charData.m_CurrentBody)
    {
        m_EquipAry[(int)ESlots::Torso] = FItemInstance(charData.m_CurrentBody,(int)ESlots::Torso,nullptr);  
    }
    if(charData.m_CurrentGlove)
    {
        m_EquipAry[(int)ESlots::Hand] = FItemInstance(charData.m_CurrentGlove,(int)ESlots::Hand,nullptr); 
    }
    if(charData.m_CurrentShoe)
    {
        m_EquipAry[(int)ESlots::Leg] = FItemInstance(charData.m_CurrentShoe,(int)ESlots::Leg,nullptr); 
    }
    if(charData.m_CurrentShoulder)
    {
        m_EquipAry[(int)ESlots::Shoulder] = FItemInstance( charData.m_CurrentShoulder,(int)ESlots::Shoulder,nullptr); 
    }
    //if(charData.m_CurrentBackpack)
    {
        //TODO BackPack? 
    }
    if(charData.m_CurrentBelt)
    {
        m_EquipAry[(int)ESlots::Waist] = FItemInstance(charData.m_CurrentBelt,(int)ESlots::Waist,nullptr); 
    }
    if(charData.m_CurrentRightWeapon)
    {
        m_EquipAry[(int)ESlots::WeaponRight] = FItemInstance(charData.m_CurrentRightWeapon,(int)ESlots::WeaponRight,nullptr); 
    }
    if(charData.m_CurrentLeftWeapon)
    {
        m_EquipAry[(int)ESlots::WeaponLeft] = FItemInstance(charData.m_CurrentLeftWeapon,(int)ESlots::WeaponLeft,nullptr); 
    }
    
}

