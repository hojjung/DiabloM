// My First Hack n Slash


#include "SaveShareStorage.h"

void USaveShareStorage::SetSaveShareStorage(ESaveVersion saveV, const TArray<bool>& aryOpen34,
    const TArray<UInventory*>& aryItems)
{
    m_SaveVersion=saveV;
    
    m_bShareStorage4Opened=aryOpen34[3];
    m_bShareStorage5Opened=aryOpen34[4];
    
    m_AryStorageItems4=aryItems[3]->GetItemAry();
    m_AryStorageItems5=aryItems[4]->GetItemAry();


    for(auto& Item : m_AryStorageItems4)
    {
        Item.m_Holder=nullptr;
    }
    for(auto& Item : m_AryStorageItems5)
    {
        Item.m_Holder=nullptr;
    }
}
