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

void UDefaultMenu::ThrowItem(const FItemInstance& itemThrow)
{
	//드랍한순간
	//아이템을 액터로 생성
	//플레이어 주위에 랜덤 위치
	//해당 아이템 인벤에서 삭제
	FVector RandomPos = m_PlayerChar->GetActorLocation();
	FItemInstance item =itemThrow;
	itemThrow.m_Holder->RemoveItemByIndex(itemThrow.m_nGridIndex);
	GetGameInstance<UDiabloGameInstance>()->m_ItemManager->CreateItemActor(item,RandomPos);
}

