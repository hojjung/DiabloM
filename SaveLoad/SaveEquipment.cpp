#include "SaveEquipment.h"
#include "SaveEquipment.h"
#include "Item/ItemManager.h"

void USaveEquipment::SetEquipSaveData(const TArray<FItemInstance>& equip)
{
    m_EquipAry = equip;

    for(auto& Item : m_EquipAry)
    {
        Item.m_Holder=nullptr;
    }
}

