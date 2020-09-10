#include "SaveInventory.h"


USaveInventory::USaveInventory()
{
    m_nCount = -1;
}


void USaveInventory::SetSaveData(const UInventory* inven)
{
    m_InvenAry = inven->GetItemAry();
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
    }

    outInven->SetItemAry(m_InvenAry);


    PRINTF("m_nCount:%d", m_nCount);
}
