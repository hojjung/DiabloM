#include "SaveInventory.h"


USaveInventory::USaveInventory()
{
}


void USaveInventory::SetSaveData(const TArray<FItemInstance>& inven)
{
    m_InvenAry = inven;
    
}

