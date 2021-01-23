// My First Hack n Slash


#include "SaveStorage.h"

void USaveStorage::SetSaveStorage(ESaveVersion saveV, const TArray<bool>& aryOpen,
    const TArray<UInventory*>& aryItems)
{
    m_SaveVersion=saveV;
    
    m_bShareStorage1Opened=aryOpen[0];
    m_bShareStorage2Opened=aryOpen[1];
    m_bShareStorage3Opened=aryOpen[2];
    
    m_AryStorageItems1=aryItems[0]->GetItemAry();
    m_AryStorageItems2=aryItems[1]->GetItemAry();
    m_AryStorageItems3=aryItems[2]->GetItemAry();

    for(auto& Item : m_AryStorageItems1)
    {
        Item.m_Holder=nullptr;
    }
    for(auto& Item : m_AryStorageItems2)
    {
        Item.m_Holder=nullptr;
    }
    for(auto& Item : m_AryStorageItems3)
    {
        Item.m_Holder=nullptr;
    }
}
