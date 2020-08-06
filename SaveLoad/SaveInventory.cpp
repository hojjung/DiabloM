#include "SaveInventory.h"
#include "DiabloM.h"

USaveInventory::USaveInventory()
{
    m_nCount=-1;
}

void USaveInventory::SetSaveData(const Inventory* inven)
{
    m_InvenAry =inven->GetItemAry();
    m_nCount=m_InvenAry.Num();
    PRINTF("m_nCount:%d",m_nCount);
}

void USaveInventory::SetInvenLoadData(Inventory* outInven)
{
    outInven->SetItemAry(m_InvenAry);

    PRINTF("m_nCount:%d",m_nCount);
}
