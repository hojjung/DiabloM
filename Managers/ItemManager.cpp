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

	TArray<FOptionValue> RandomOptionForItem;

	if (CreateRandomOption(1, *ItemData, RandomOptionForItem))
	{
		ItemCreated = FItemInstance(ItemData, m_nCurrentIndex, this, &RandomOptionForItem);
	}
	else
	{
		ItemCreated = FItemInstance(ItemData, m_nCurrentIndex, this);
	}
	

	return ItemCreated;
}


bool ItemManager::CreateRandomOption(int level, const FItemData & itemData, TArray<FOptionValue>& outOption)
{
	//등급에 따라 옵션의 개수?
	//레벨에 따라 옵션의 종류 및 강함?
	//생각할게 많지만 지금은 옵션의 개수만 랜덤으로 돌린다.
	//등급 자체는 정해저있지 않나?
	//등급이 정해저있는데 옵션의 개수가 무슨 의미인가

	if (itemData.m_bStackable)
	{
		return false;
	}

	int NumMaxOption = itemData.m_Options.Num();

	if (NumMaxOption <= 0)
	{
		return false;
	}

	int OptionRandomCount = FMath::Rand() % itemData.GetItemTier().m_nOptionMaxCount;//생성할 옵션의 개수는 등급과 옵션의 개수에 따라 상이하다.

	OptionRandomCount = FMath::Min<int>(OptionRandomCount, NumMaxOption);


	TArray<int> optionRandom;

	CreateIntAryForShuffle(NumMaxOption, optionRandom);


	int i = 0;
	while (i++ <= OptionRandomCount)
	{
		outOption.Add(CreateRandomOptionValue(optionRandom[i], itemData));
	}
	//아웃옵션한텐 인덱스를 줘야한다.

	return true;
}

void ItemManager::CreateIntAryForShuffle(int maxAryLen, TArray<int>& outIndexAry)
{
	int NumMaxOption = maxAryLen;
	int iter = NumMaxOption * 2;

	outIndexAry.Reset();

	int index = 0;
	while (index < maxAryLen)
	{
		outIndexAry.Add(index++);
	}

	for (int a = 0; a < iter; a++)
	{
		int RandIndex = FMath::Rand() % NumMaxOption;

		auto First = outIndexAry[a];
		auto Second = outIndexAry[RandIndex];

		outIndexAry[a] = Second;
		outIndexAry[RandIndex] = First;
	}

}

FOptionValue ItemManager::CreateRandomOptionValue(int indexRandomd, const FItemData & itemData)
{
	auto& OptionData = itemData.GetOption(indexRandomd);
	float RandValue = FMath::FRandRange(OptionData.m_fMinValue, OptionData.m_fMaxValue);
	return FOptionValue(indexRandomd, RandValue);
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
