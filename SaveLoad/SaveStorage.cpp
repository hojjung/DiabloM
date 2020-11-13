// My First Hack n Slash


#include "SaveStorage.h"

void USaveStorage::SetSaveStorage(ESaveVersion saveV, const TArray<bool>& aryOpen,
    const TArray<TArray<FItemInstance>>& aryItems012)
{
    m_SaveVersion=saveV;
    
    m_bShareStorage1Opened=aryOpen[0];
    m_bShareStorage2Opened=aryOpen[1];
    m_bShareStorage3Opened=aryOpen[2];
    
    m_AryStorageItems1=aryItems012[0];
    m_AryStorageItems2=aryItems012[1];
    m_AryStorageItems3=aryItems012[2];
}
