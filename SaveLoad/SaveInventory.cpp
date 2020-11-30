#include "SaveInventory.h"


USaveInventory::USaveInventory()
{
}


void USaveInventory::SetSaveData(const TArray<FItemInstance>& inven)
{
    m_InvenAry = inven;
    
    for(auto& Item : m_InvenAry)
    {
        Item.m_Holder=nullptr;
    }
}

