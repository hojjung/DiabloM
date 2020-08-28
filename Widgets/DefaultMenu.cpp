#include "DefaultMenu.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Objs/PlayerSkillSystem.h"
#include "Objs/EquipmentSystem.h"
#include "Objs/Inventory.h"
#include "Widgets/DiaInvenGridPanel.h"

void UDefaultMenu::Init(ADiabloPlayerController * playerCon, APlayerDiabloCharacter * playerChar, PlayerSkillSystem * skill, EquipmentSystem * equipment, Inventory * inven)
{
	m_PlayerCon = playerCon;
	m_PlayerChar = playerChar;
	m_Skill = skill;
	m_Equipment = equipment;
	m_Inven = inven;

	m_InvenGridPanel->Init(m_Inven);

	m_EquipPanel->Init(m_Equipment);

}

void UDefaultMenu::OpenMainMenu()
{
	this->SetVisibility((ESlateVisibility::SelfHitTestInvisible));
}

void UDefaultMenu::CloseMainMenu()
{
	this->SetVisibility((ESlateVisibility::Hidden));

	//should close tool tip
}

