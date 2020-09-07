#include "SaveCharacterStatus.h"
#include "SaveLoadManager.h"

#include "Characters/PlayerDiabloCharacter.h"

void USaveCharacterStatus::SaveCharStatSave(int index, int level, FText playerName)
{
    m_nSlotIndex= index;

    m_nLevel = level;
    //m_TextUnitName
    m_TextName = playerName;
}

void USaveCharacterStatus::SetCharStatLoad(ADiabloPlayerController* diaPl)
{
    PRINTF("Load-%d, Name:%s",m_nSlotIndex,*m_TextName.ToString());
    
    diaPl->GetUnitPawn()->m_TextUnitName = m_TextName;

    diaPl->GetUnitPawn()->SetLevel(m_nLevel);

    
}
