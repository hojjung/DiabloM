#include "Inventory.h"
#include "Datas/ItemDataTable.h"

Inventory::~Inventory()
{
	m_nCurrentEmptyIndex = -1;
	m_nXGridCount = -1;
	m_nYGridCount = -1;

	for (auto& ItemList : m_ItemContainer)
	{
		for (auto ItemInst : ItemList.Value)
		{
			ItemInst.SetGridNewIndex(-1);
		}
	}

	m_ItemContainer.Empty();
}

void Inventory::InitInven(int xCount, int yCount)
{
	m_nCurrentEmptyIndex = 0;

	m_nXGridCount = xCount;

	m_nYGridCount = yCount;

	int CountTotal = m_nXGridCount * m_nYGridCount;

	m_ItemContainer.Reserve(CountTotal);

	m_GridItem.Init(false, CountTotal);

}

bool Inventory::AddItem(FItemInstance itemWantAdd)//use for auto add
{
	FName ItemID = itemWantAdd.GetItemID();

	TArray<FItemInstance>*  FoundItemList = nullptr;


	bool IsStackable = itemWantAdd.GetIsStackable();

	if (!IsStackable)
	{
		if (m_nCurrentEmptyIndex == -1)
		{
			PRINTF("Inven Max 1");

			return false;
		}
	}

	if (CheckItemExist(FoundItemList, ItemID, IsStackable))//이미 아이템이 존재하는지
	{
		if (IsStackable)//스택이 가능한 아이템이야?
		{
			FItemInstance* FoundItem=nullptr;

			if (TryGetStackable(*FoundItemList, FoundItem))//스택추가 가능한거 찾아냄
			{
				FoundItem->AddStack();

				m_OnSlotChanged.ExecuteIfBound(FoundItem->GetIndex(), *FoundItem);
				//스텍만 늘어남
				return true;
			}
		}


		return AddToList(*FoundItemList, itemWantAdd);
	}

	
	TArray<FItemInstance> NewList;
	if (!AddToList(NewList, itemWantAdd))
	{
		return false;
	}
	m_ItemContainer.Emplace(ItemID, NewList);
	//리스트 자체 추가
	return true;
}

bool Inventory::AddToList(TArray<FItemInstance>& FoundItemList, FItemInstance &itemWantAdd)
{
	if (m_nCurrentEmptyIndex == -1)
	{
		return false;
	}
	itemWantAdd.SetGridNewIndex(m_nCurrentEmptyIndex);
	int Index= FoundItemList.Add(itemWantAdd);
	m_GridItem[m_nCurrentEmptyIndex] = true;
	m_OnSlotChanged.ExecuteIfBound(m_nCurrentEmptyIndex, FoundItemList[Index]);
	SetEmptyIndex();

	return true;
}

void Inventory::SetEmptyIndex()
{
	m_nCurrentEmptyIndex = -1;
	for (int i = 0; i < m_GridItem.Num(); i++)
	{
		if (!m_GridItem[i])
		{
			m_nCurrentEmptyIndex = i;
			return;
		}
	}

	//remove 시 누가 더작은지 비교
}


bool Inventory::CheckItemExist(TArray<FItemInstance>*&  foundItemList, FName id, bool stackable, int countWant)//지금은 개수가 중요하다
{
	int FoundCount = 0;

	if (m_ItemContainer.Contains(id))
	{
		if (!stackable)
		{
			for (FItemInstance& InItem : m_ItemContainer[id])
			{
				FoundCount++;
			}
		}
		else
		{
			for (FItemInstance& InItem : m_ItemContainer[id])
			{
				FoundCount += InItem.GetCurrentStack();
			}
		}

		if (FoundCount >= countWant)
		{
			foundItemList = &m_ItemContainer[id];

			return true;
		}
	}
	return false;
}

bool Inventory::RemoveItemByName(FName id, int itemCount)
{
	return false;
}

void Inventory::RemoveItemInstance(FItemInstance itemWantRemove)
{
}

void Inventory::PrintInven()
{
	int Count = m_ItemContainer.Num();
	PRINTF("----InvenPrint----");
	PRINTF("There is %d of Item In Inventory", Count);

	for (auto& List : m_ItemContainer)
	{
		PRINTF("ItemName: %s",  *List.Key.ToString());

		for (FItemInstance& ItemInst : List.Value)
		{
			PRINTF("Index: %d , Stack: %d, Ptr:%x", ItemInst.GetIndex(), ItemInst.GetCurrentStack(), &ItemInst);
		}
	}
}

void Inventory::GetInvenSize(int & x, int & y)
{
	x = m_nXGridCount;
	y = m_nYGridCount;
}

bool Inventory::TryGetStackable(TArray<FItemInstance>& itemList, FItemInstance*& outItemInst)
{
	for (auto& Item : itemList)
	{
		if (Item.CheckCanStack())
		{
			outItemInst = &Item;

			return true;
		}
	}

	return false;
}
