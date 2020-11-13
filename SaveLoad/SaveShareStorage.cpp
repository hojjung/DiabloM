// My First Hack n Slash


#include "SaveShareStorage.h"

void USaveShareStorage::SetSaveShareStorage(ESaveVersion saveV, const TArray<bool>& aryOpen34,
    const TArray<TArray<FItemInstance>>& aryItems34)
{
    m_SaveVersion=saveV;
    
    m_bShareStorage4Opened=aryOpen34[3];
    m_bShareStorage5Opened=aryOpen34[4];
    
    m_AryStorageItems4=aryItems34[3];
    m_AryStorageItems5=aryItems34[4];
}
