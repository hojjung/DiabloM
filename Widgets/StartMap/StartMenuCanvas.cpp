#include "StartMenuCanvas.h"
#include "Characters/StartMap/PlayerCreateController.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/StartMap/PlayerCreateManager.h"
#include "SaveLoad/SaveLoadManager.h"


void UStartMenuCanvas::InitStartMenu()
{
    PRINTF("InitStartMenu");
    m_CharCreate->Init(GetGameInstance<UDiabloGameInstance>()->GetPlCreateManager());
    m_CharSelect->Init(GetGameInstance<UDiabloGameInstance>()->GetSaveLoadManager());
    m_CharSelect->m_BtnCreateNewChar->OnClicked.AddDynamic(this,&UStartMenuCanvas::CreationStart);
    m_CharCreate->m_BtnContinue->OnClicked.AddDynamic(this,&UStartMenuCanvas::CreationEnd);
    m_CharCreate->m_CharSelect=m_CharSelect;
}

bool UStartMenuCanvas::HaveEmptySlot()
{
    return USaveLoadManager::Get->GetEmptyIndex() != -1;
}

void UStartMenuCanvas::CreationStart()
{
    if(!HaveEmptySlot())
    {
        return;
    }
    
    if(m_CharSelect->m_FocusedIndex!=-1)
        m_CharSelect->m_AryCharInfoSlot[m_CharSelect->m_FocusedIndex]->DeselectSlot();
    
    m_CharCreate->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_CharSelect->SetVisibility(ESlateVisibility::Hidden);
    m_CharCreate->StartCreation();
    Cast<APlayerCreateController>( GetOwningPlayer())->GetPlayerVisual()->ShowMesh();
    UPlayerCreateManager::Get->ClearIndex();
    UPlayerCreateManager::Get->SetArmorFromSetting();
    UPlayerCreateManager::Get->SetFaceFromSetting();
    UPlayerCreateManager::Get->SetPerkFromSetting();
    UPlayerCreateManager::Get->SetItemFromSetting();
    UPlayerCreateManager::Get->SetHairFromSetting();
    UPlayerCreateManager::Get->OnDataChanged();
}

void UStartMenuCanvas::CreationEnd()
{
    m_CharCreate->SetVisibility(ESlateVisibility::Hidden);
    m_CharSelect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    //Cast<APlayerCreateController>( GetOwningPlayer())->GetPlayerVisual()->HideMesh();
}
