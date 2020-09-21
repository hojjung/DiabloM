#include "DefaultMenu.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"
#include "Item/EquipmentSystem.h"
#include "Item/Inventory.h"
#include "Widgets/WorldMap/DefaultMenu/DiaInvenGridPanel.h"

void UDefaultMenu::Init(ADiabloPlayerController * playerCon, APlayerDiabloCharacter * playerChar,
	 UEquipmentSystem * equipment, UInventory * inven)
{
	m_PlayerCon = playerCon;
	m_PlayerChar = playerChar;
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


