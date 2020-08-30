#include "MainCanvas.h"

#include "Characters/PlayerDiabloCharacter.h"
#include "Widgets/DefaultMenu.h"


void UMainCanvas::OpenMainMenu()
{
    m_MainMenu->OpenMainMenu();

    m_InvenButton->SetVisibility(ESlateVisibility::Hidden);
    m_SettingButton->SetVisibility(ESlateVisibility::Hidden);
    m_AttackButton->SetVisibility(ESlateVisibility::Hidden);
    m_InteractButton->SetVisibility(ESlateVisibility::Hidden);
}

void UMainCanvas::CloseMainMenu()
{
    m_MainMenu->CloseMainMenu();

    m_InvenButton->SetVisibility(ESlateVisibility::Visible);
    m_SettingButton->SetVisibility(ESlateVisibility::Visible);
    m_AttackButton->SetVisibility(ESlateVisibility::Visible);
    m_InteractButton->SetVisibility(ESlateVisibility::Visible);
}

void UMainCanvas::Interaction()
{
    m_PlayerPawn->InteractWithTarget();
}

void UMainCanvas::Attack()
{
    m_PlayerPawn->AttackInput(1.f);
}

void UMainCanvas::OpenSetting()
{
}

void UMainCanvas::CloseSetting()
{
    
}

void UMainCanvas::Init(ADiabloPlayerController * playerCon, APlayerDiabloCharacter * playerChar, PlayerSkillSystem * skill, EquipmentSystem * equipment, Inventory * inven)
{
    m_PlayerCon = playerCon;
    m_PlayerPawn = playerChar;
    m_SkillSys = skill;
    m_EquipSys = equipment;
    m_Inven = inven;
    m_MainMenu->Init(m_PlayerCon,m_PlayerPawn,m_SkillSys,m_EquipSys,m_Inven);

    m_InvenButton->OnClicked.AddDynamic(this,&UMainCanvas::OpenMainMenu);

    m_SettingButton->OnClicked.AddDynamic(this,&UMainCanvas::OpenSetting);

    m_InteractButton->OnClicked.AddDynamic(this,&UMainCanvas::Interaction);

    m_AttackButton->OnPressed.AddDynamic(this,&UMainCanvas::Attack);
}
