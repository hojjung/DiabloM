#include "ItemManager.h"
#include "Managers/DiabloGameInstance.h"



ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}


void ItemManager::Init(UDiabloGameInstance * gameInstance)
{
	m_GameInstance = gameInstance;
	m_nCurrentIndex = 0;
	m_AryItemInWorld.Reserve(100);
}

FItemInstance ItemManager::CreateItemInstance(FName itemID)
{
	auto* ItemData=m_GameInstance->GetItemData(itemID);

	FItemInstance ItemCreated;

	AddItem(0, ItemCreated);

	ItemCreated = FItemInstance(ItemData, m_nCurrentIndex,this);

	return ItemCreated;
}

bool ItemManager::AddItem(int droppedIndex, FItemInstance& itemWantAdd)
{
	m_nCurrentIndex = m_AryItemInWorld.Add(&itemWantAdd);

	return true;
}

void ItemManager::RemoveItem(FItemInstance & itemWantErase)
{
	RemoveItemByIndex(itemWantErase.m_nGridIndex);
}

void ItemManager::RemoveItemByIndex(int index)
{
	m_AryItemInWorld[index]->ClearData();
}

bool ItemManager::CheckSlotValid(int droppedIndex, FItemInstance & itemWantAdd)
{
	return true;
}

void ItemManager::SetItem(int droppedIndex, FItemInstance & itemWantAdd)
{
}

bool ItemManager::SwapMove(FItemInstance & Drop, FItemInstance & Drag)
{
	return true;
}
