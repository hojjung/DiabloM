#include "StartMenuCanvas.h"
#include "Characters/StartMap/PlayerCreateController.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/StartMap/PlayerCreateManager.h"
#include "SaveLoad/SaveLoadManager.h"


void UStartMenuCanvas::InitStartMenu()
{
    m_bIsCreationOpened=false;
    m_bIsSelectionOpened=false;
    PRINTF("InitStartMenu");
    m_CharCreate->Init(GetGameInstance<UDiabloGameInstance>()->GetPlCreateManager());
    m_CharSelect->Init(GetGameInstance<UDiabloGameInstance>()->GetSaveLoadManager());
    m_CharSelect->m_BtnCreateNewChar->OnClicked.AddDynamic(this,&UStartMenuCanvas::CreationStart);
    m_CharCreate->m_BtnContinue->OnClicked.AddDynamic(this,&UStartMenuCanvas::CreationEnd);
    m_CharCreate->m_CharSelect=m_CharSelect;
    m_BtnExit->OnClicked.AddDynamic(this,&UStartMenuCanvas::ExitGame);
    m_BtnPlay->OnClicked.AddDynamic(this,&UStartMenuCanvas::PlayPressed);
    m_BtnBackCancel->OnClicked.AddDynamic(this,&UStartMenuCanvas::BackCancel);
    m_BtnBackCancel->SetVisibility(ESlateVisibility::Hidden);
}

void UStartMenuCanvas::ShowMainMenu()
{
    m_BG->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_TextTitle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_BtnOption->SetVisibility(ESlateVisibility::Visible);
    m_BtnExit->SetVisibility(ESlateVisibility::Visible);
    m_BtnPlay->SetVisibility(ESlateVisibility::Visible);
    //
    m_CharSelect->SetVisibility(ESlateVisibility::Hidden);
    m_bIsSelectionOpened=false;
    //
    m_BtnBackCancel->SetVisibility(ESlateVisibility::Hidden);
}

void UStartMenuCanvas::HideMainMenu()
{
    m_BG->SetVisibility(ESlateVisibility::Hidden);
    m_TextTitle->SetVisibility(ESlateVisibility::Hidden);
    m_BtnOption->SetVisibility(ESlateVisibility::Hidden);
    m_BtnExit->SetVisibility(ESlateVisibility::Hidden);
    m_BtnPlay->SetVisibility(ESlateVisibility::Hidden);
    //
    m_CharSelect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_bIsSelectionOpened=true;

    m_BtnBackCancel->SetVisibility(ESlateVisibility::Visible);
}

bool UStartMenuCanvas::HaveEmptySlot()
{
    return USaveLoadManager::Get->GetEmptyIndex() != -1;
}

void UStartMenuCanvas::PlayPressed()
{
    HideMainMenu();
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
    m_BtnBackCancel->SetVisibility(ESlateVisibility::Visible);
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

    m_bIsCreationOpened=true;
}

void UStartMenuCanvas::CreationEnd()
{
    m_CharCreate->SetVisibility(ESlateVisibility::Hidden);
    m_CharSelect->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_bIsCreationOpened=false;
    
    
}

void UStartMenuCanvas::ExitGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}

void UStartMenuCanvas::BackCancel()
{
    if(m_bIsCreationOpened)
    {
        m_CharCreate->BackCancel();
        m_CharSelect->m_FocusedIndex=-1;
        Cast<APlayerCreateController>( GetOwningPlayer())->GetPlayerVisual()->HideMesh();
        CreationEnd();    
    }
    else if(m_bIsSelectionOpened)
    {
        ShowMainMenu();
    }
}
