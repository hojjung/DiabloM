#include "DiaGameOverScreen.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"



void UDiaGameOverScreen::Init(ADiabloPlayerController* plCon, APlayerDiabloCharacter* player)
{
    m_Player=player;
    m_PlayerController=plCon;
    
    m_BtnReviveHere->OnClicked.AddDynamic(this,&UDiaGameOverScreen::ReviveHere);
    m_BtnRestartDungeon->OnClicked.AddDynamic(this,&UDiaGameOverScreen::RestartDungeon);
    m_BtnBackToVillage->OnClicked.AddDynamic(this,&UDiaGameOverScreen::BackToVillage);
}

void UDiaGameOverScreen::ReviveHere()
{
    m_Player->Revive();
    //SetVisibility(ESlateVisibility::Hidden);
}

void UDiaGameOverScreen::RestartDungeon()
{
    if(!UDiabloGameInstance::Get->IsDungeonOpened())
    {
        return;
    }
    UDiabloGameInstance::Get->GetDungeonManager()->RestartDungeon();
    m_Player->Revive();
   // SetVisibility(ESlateVisibility::Hidden);
}

void UDiaGameOverScreen::BackToVillage()    
{
    if(!UDiabloGameInstance::Get->IsDungeonOpened())
    {
        return;
    }
    UDiabloGameInstance::Get->GetDungeonManager()->PortalToVillage(false);
    m_Player->Revive();
   // SetVisibility(ESlateVisibility::Hidden);
}
