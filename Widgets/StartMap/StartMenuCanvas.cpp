#include "StartMenuCanvas.h"

#include "Characters/StartMap/PlayerCreateController.h"
#include "Managers/DiabloGameInstance.h"


void UStartMenuCanvas::InitStartMenu()
{
    PRINTF("InitStartMenu");
    m_CharCreate->Init(GetGameInstance<UDiabloGameInstance>()->m_PlCreateManager);
    m_CharSelect->Init(GetGameInstance<UDiabloGameInstance>()->m_SaveLoadManager);
    m_CharSelect->m_BtnCreateNewChar->OnClicked.AddDynamic(this,&UStartMenuCanvas::CreationStart);
    m_CharCreate->m_BtnContinue->OnClicked.AddDynamic(this,&UStartMenuCanvas::CreationEnd);
}

void UStartMenuCanvas::CreationStart()
{
    m_CharCreate->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_CharSelect->SetVisibility(ESlateVisibility::Hidden);
    m_CharSelect->StartCreation();
}

void UStartMenuCanvas::CreationEnd()
{
    m_CharCreate->SetVisibility(ESlateVisibility::Hidden);
    m_CharSelect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    PlayerCreateManager::Get->m_CurrentCharData.Clear();
    Cast<APlayerCreateController>( GetOwningPlayer())->GetPlayerVisual()->HideMesh();
}
