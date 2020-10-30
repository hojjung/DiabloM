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

