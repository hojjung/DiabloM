#include "SaveInventory.h"


USaveInventory::USaveInventory()
{
    m_nCount = -1;
}


void USaveInventory::SetSaveData(const TArray<FItemInstance>& inven)
{
    m_InvenAry = inven;
    m_nCount = m_InvenAry.Num();

    PRINTF("m_nCount:%d", m_nCount);
}

void USaveInventory::SetInvenLoadData(UInventory* outInven)
{
    //m_SaveVersion

    for (FItemInstance& ItemLoad : m_InvenAry)
    {
        if (ItemLoad.m_ItemID == NAME_None)
        {
            continue;
        }
        ItemLoad.m_ItemData = UItemDataTable::GetItemDataPtr(ItemLoad.m_ItemID);
        ItemLoad.m_ItemTier = UItemDataTable::GetItemTierPtr(ItemLoad.m_TierID);
    }

    outInven->SetItemAry(m_InvenAry);


    PRINTF("m_nCount:%d", m_nCount);
}
