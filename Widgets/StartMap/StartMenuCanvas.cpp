#include "StartMenuCanvas.h"

#include "Characters/StartMap/PlayerCreateController.h"
#include "Managers/DiabloGameInstance.h"


void UStartMenuCanvas::InitStartMenu()
{
    PRINTF("InitStartMenu");
    m_CharCreate->Init(GetGameInstance<UDiabloGameInstance>()->m_PlCreateManager);
    m_CharSelect->Init(GetGameInstance<UDiabloGameInstance>()->m_SaveLoadManagerOld);
    m_CharSelect->m_BtnCreateNewChar->OnClicked.AddDynamic(this,&UStartMenuCanvas::CreationStart);
    m_CharCreate->m_BtnContinue->OnClicked.AddDynamic(this,&UStartMenuCanvas::CreationEnd);
}

bool UStartMenuCanvas::HaveEmptySlot()
{
    return SaveLoadManagerOld::Get->GetEmptyIndex() != -1;
}

void UStartMenuCanvas::CreationStart()
{
    if(!HaveEmptySlot())
    {
        return;
    }

    
    if(m_CharSelect->m_FocusedInfo)
        m_CharSelect->m_FocusedInfo->DeselectSlot();
    
    m_CharCreate->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_CharSelect->SetVisibility(ESlateVisibility::Hidden);
    m_CharCreate->StartCreation();
    Cast<APlayerCreateController>( GetOwningPlayer())->GetPlayerVisual()->ShowMesh();
    PlayerCreateManagerOld::Get->ClearIndex();
    PlayerCreateManagerOld::Get->SetArmorFromSetting();
    PlayerCreateManagerOld::Get->SetFaceFromSetting();
    PlayerCreateManagerOld::Get->SetPerkFromSetting();
    PlayerCreateManagerOld::Get->SetItemFromSetting();
    PlayerCreateManagerOld::Get->SetHairFromSetting();
    PlayerCreateManagerOld::Get->OnDataChanged();
}

void UStartMenuCanvas::CreationEnd()
{
    m_CharCreate->SetVisibility(ESlateVisibility::Hidden);
    m_CharSelect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    Cast<APlayerCreateController>( GetOwningPlayer())->GetPlayerVisual()->HideMesh();
}
