#include "CharInfo.h"
#include "Characters/StartMap/PlayerCreateController.h"
#include "SaveLoad/SaveLoadManager.h"

void UCharInfo::Init(const USaveCharacterStatus* char_stats)
{
    m_TextName->SetText(FText::FromString( char_stats->m_TextName));
    m_TextLevel->SetText(FText::AsNumber(char_stats->m_nLevel));
    m_BtnSelect->OnClicked.AddDynamic(this,&UCharInfo::SelectSlot);
    m_nSlotIndex=char_stats->m_nSlotIndex;

    m_BtnDelete->OnClicked.AddDynamic(this,&UCharInfo::DeleteSlot);
}

void UCharInfo::SelectSlot()
{
    m_ImgFocus->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_OnSelect.ExecuteIfBound(m_nSlotIndex,this);
}

void UCharInfo::DeselectSlot()
{
    m_ImgFocus->SetVisibility(ESlateVisibility::Hidden);
}

void UCharInfo::DeleteSlot()
{
    Cast<APlayerCreateController> (GetOwningPlayer())->GetPlayerVisual()->HideMesh();
    USaveLoadManager::Get->DeleteSlot(m_nSlotIndex);
    PRINTF("SlotDelete");
    SetVisibility(ESlateVisibility::Collapsed);
    //Destroy();
}
