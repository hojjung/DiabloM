#include "SaveEquipment.h"
#include "SaveEquipment.h"
#include "Item/ItemManager.h"

void USaveEquipment::SetEquipSaveData(const TArray<FItemInstance>& equip)
{
    m_EquipAry = equip;
}

void USaveEquipment::SetEquipLoadData(UEquipmentSystem** outEquip)
{
    PRINTF("EquipLoaded:%d", m_EquipAry.Num());

    for (FItemInstance& ItemLoad : m_EquipAry)
    {
        if (ItemLoad.m_ItemID == NAME_None)
        {
            continue;
        }
        ItemLoad.m_ItemData = UItemDataTable::GetItemDataPtr(ItemLoad.m_ItemID); //id가 저장이안됨
        ItemLoad.m_ItemTier = UItemDataTable::GetItemTierPtr(ItemLoad.m_TierID);
    }

    (*outEquip)->SetItemAry(m_EquipAry);
}
