// My First Hack n Slash


#include "SaveStorage.h"

void USaveStorage::SetSaveStorage(ESaveVersion saveV, const TArray<bool>& aryOpen,
    const TArray<TArray<FItemInstance>>& aryItems)
{
    m_SaveVersion=saveV;
    m_AryIsStorageOpened=aryOpen;
    m_AryStorageItems1=aryItems[0];
    m_AryStorageItems2=aryItems[1];
    m_AryStorageItems3=aryItems[2];
    m_AryStorageItems4=aryItems[3];
    m_AryStorageItems5=aryItems[4];
}
