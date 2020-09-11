#include "SaveCharacterStatus.h"
#include "Characters/PlayerDiabloCharacter.h"

void USaveCharacterStatus::SaveCharStatSave(int index, int level, FText playerName)
{
    m_nSlotIndex= index;

    m_nLevel = level;
    //m_TextUnitName
    m_TextName = playerName.ToString();
}

void USaveCharacterStatus::SetCharStatLoad(ADiabloPlayerController* diaPl)
{
    PRINTF("Load-%d, Name:%s",m_nSlotIndex,*m_TextName);
    
    diaPl->GetPlayerPawn()->m_TextUnitName =FText::FromString(m_TextName);

    diaPl->GetPlayerPawn()->SetLevel(m_nLevel);

    
}
