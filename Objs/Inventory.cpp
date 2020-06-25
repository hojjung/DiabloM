#include "Inventory.h"
#include "Datas/ItemDataTable.h"

Inventory::~Inventory()
{
	m_nCurrentEmptyIndex = -1;
	m_nXGridCount = -1;
	m_nYGridCount = -1;

	//m_ItemDic.Empty();
}

void Inventory::InitInven(int xCount, int yCount)
{
	m_nCurrentEmptyIndex = 0;

	m_nXGridCount = xCount;

	m_nYGridCount = yCount;

	int CountTotal = m_nXGridCount * m_nYGridCount;

	m_ItemAry.Reserve(CountTotal);

	m_ItemAry.Init(FItemInstance(), CountTotal);
	//m_ItemDic.Reserve(CountTotal);
}

void Inventory::AddItem(int index, FItemInstance & itemWantAdd)
{
	m_ItemAry[index] = itemWantAdd;
	m_ItemAry[index].m_nGridIndex = index;
	m_OnSlotChanged.ExecuteIfBound(index, m_ItemAry[index]);
}

void Inventory::AddItemStack(int index)
{
	m_ItemAry[index].AddStack();
}

void Inventory::RemoveItem(int index)
{
	m_ItemAry[index].ClearData();
	m_OnSlotChanged.ExecuteIfBound(index, m_ItemAry[index]);
}

void Inventory::RemoveItemStack(int index)
{
	m_ItemAry[index].RemoveStack();
}

void Inventory::SwapItemIndex(int aIndex, int bIndex)
{
	FItemInstance A = m_ItemAry[aIndex];
	FItemInstance B = m_ItemAry[bIndex];

	AddItem(aIndex, A);

	AddItem(bIndex, B);
}

bool Inventory::StackMoveItem(int increaseIndex, int decreaseIndex)
{
	FItemInstance I = m_ItemAry[increaseIndex];//
	FItemInstance D = m_ItemAry[decreaseIndex];//

	if (!I.CheckCanStack() || !I.GetIsStackable() || I.m_ItemData != D.m_ItemData)
	{
		return false;
	}




	return true;
}

bool Inventory::OnDropIndexHeaped(int droppedSlot, int draggedDDO)
{
	if (droppedSlot == draggedDDO)
	{
		return false;
	}

	if (!m_ItemAry[droppedSlot].m_ItemData)
	{
		AddItem(droppedSlot, GetItemRef(draggedDDO));
		RemoveItem(draggedDDO);
		PRINTF("Success1");
		return true;
	}

	FItemInstance Drop = m_ItemAry[droppedSlot];

	FItemInstance Drag = GetItemRef(draggedDDO);

	//Stack
	if (Drag.GetIsStackable()&& Drag.CheckCanStack() && Drop.m_ItemData->m_NameID == Drag.m_ItemData->m_NameID)//스왑방지코드
	{
		StackMove(Drop, Drag, droppedSlot, draggedDDO);

		PRINTF("Stack");
	}
	else
	{

		PRINTF("SWap");
		//Swap
		SwapMove(draggedDDO, Drop, droppedSlot, Drag);
	}

	m_OnSlotChanged.ExecuteIfBound(droppedSlot, m_ItemAry[droppedSlot]);
	m_OnSlotChanged.ExecuteIfBound(draggedDDO, m_ItemAry[draggedDDO]);

	return true;
}

void Inventory::SwapMove(int draggedDDO, FItemInstance &Drop, int droppedSlot, FItemInstance &Drag)
{
	AddItem(draggedDDO, Drop);

	AddItem(droppedSlot, Drag);
}

void Inventory::StackMove(FItemInstance &Drop, FItemInstance &Drag, int droppedSlot, int draggedDDO)
{
	int DiffStackCount = Drop.m_ItemData->m_nMaxStack - Drop.m_nCurrentStack;

	int Count = FMath::Min(DiffStackCount, Drag.m_nCurrentStack);

	while (Count--)
	{
		AddItemStack(droppedSlot);
		RemoveItemStack(draggedDDO);
	}

	if (m_ItemAry[draggedDDO].m_nCurrentStack <= 0)
	{
		m_ItemAry[draggedDDO].ClearData();
	}
}

bool Inventory::AddItemAuto(FItemInstance& itemWantAdd)//use for auto add
{
	AddItem(m_nCurrentEmptyIndex++, itemWantAdd);
	//FName ItemID = itemWantAdd.GetItemID();

	//bool IsStackable = itemWantAdd.GetIsStackable();

	//if (!IsStackable)
	//{
	//	if (m_nCurrentEmptyIndex == -1)
	//	{
	//		PRINTF("Inven Max 1");

	//		return false;
	//	}
	//}

	//TArray<FItemInstance>*  FoundItemList = nullptr;

	//if (CheckItemExist(FoundItemList, ItemID, IsStackable))//이미 아이템이 존재하는지
	//{
	//	if (IsStackable)//스택이 가능한 아이템이야?
	//	{
	//		FItemInstance* FoundItem=nullptr;

	//		if (TryGetStackable(*FoundItemList, FoundItem))//스택추가 가능한거 찾아냄
	//		{
	//			FoundItem->AddStack();

	//			m_OnSlotChanged.ExecuteIfBound(FoundItem->GetIndex(), *FoundItem);
	//			//스텍만 늘어남
	//			return true;
	//		}
	//	}

	//	return AddToList(*FoundItemList, itemWantAdd);
	//}

	//
	//TArray<FItemInstance> NewList;
	//if (!AddToList(NewList, itemWantAdd))
	//{
	//	return false;
	//}
	//m_ItemContainer.Emplace(ItemID, NewList);
	//리스트 자체 추가
	return true;
}

bool Inventory::AddItemToIndex(int index, FItemInstance & itemWantAdd)
{
	if (!m_ItemAry[index].m_ItemData)//비어있음
	{
		m_ItemAry[index] = itemWantAdd;
		m_ItemAry[index].m_nGridIndex = index;
		m_OnSlotChanged.ExecuteIfBound(index, m_ItemAry[index]);
		return true;
	}

	//비어있지 않다면?

	//스택 불가 아이템이면 불가능 혹은 스왑,스왑못하지 지금,저 아이템이 어디서 온건지 어케알아

	/*if (!itemWantAdd.GetIsStackable() || !m_ItemAry[index].CheckCanStack())
	{
		return false;
	}*/
	//

	if (itemWantAdd.GetIsStackable() && itemWantAdd.m_ItemData->m_NameID == m_ItemAry[index].m_ItemData->m_NameID)//스왑방지코드
	{
		while (m_ItemAry[index].CheckCanStack() && itemWantAdd.m_nCurrentStack > 0)
		{
			m_ItemAry[index].AddStack();
			itemWantAdd.RemoveStack();
		}
	}
	else
	{
		m_ItemAry[index] = itemWantAdd;
		m_ItemAry[index].m_nGridIndex = index;
		m_OnSlotChanged.ExecuteIfBound(index, m_ItemAry[index]);
	}



	m_OnSlotChanged.ExecuteIfBound(index, m_ItemAry[index]);

	return true;

}

FItemInstance Inventory::RemoveItemFromIndex(int index)
{
	FItemInstance RemovedData = m_ItemAry[index];
	m_ItemAry[index].ClearData();

	return RemovedData;
}

bool Inventory::AddToList(TArray<FItemInstance>& FoundItemList, FItemInstance &itemWantAdd)
{
	/*if (m_nCurrentEmptyIndex == -1)
	{
		return false;
	}
	itemWantAdd.SetGridNewIndex(m_nCurrentEmptyIndex);
	int Index= FoundItemList.Add(itemWantAdd);
	m_GridItem[m_nCurrentEmptyIndex] = true;
	m_OnSlotChanged.ExecuteIfBound(m_nCurrentEmptyIndex, FoundItemList[Index]);
	SetEmptyIndex();*/

	return true;
}

void Inventory::SetEmptyIndex()
{
	//m_nCurrentEmptyIndex = -1;
	//for (int i = 0; i < m_GridItem.Num(); i++)
	//{
	//	if (!m_GridItem[i])
	//	{
	//		m_nCurrentEmptyIndex = i;
	//		return;
	//	}
	//}

	//remove 시 누가 더작은지 비교
}


bool Inventory::CheckItemExist(TArray<FItemInstance>*&  foundItemList, FName id, bool stackable, int countWant)//지금은 개수가 중요하다
{
	/*int FoundCount = 0;

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
	}*/
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
	//int Count = m_ItemContainer.Num();
	PRINTF("----InvenPrint----");
	//PRINTF("There is %d of Item In Inventory", Count);

	//for (auto& List : m_ItemContainer)
	//{
	//	PRINTF("ItemName: %s",  *List.Key.ToString());

	//	for (FItemInstance& ItemInst : List.Value)
	//	{
	//		PRINTF("Index: %d , Stack: %d, Ptr:%x", ItemInst.GetIndex(), ItemInst.GetCurrentStack(), &ItemInst);
	//	}
	//}

	for (auto& Item : m_ItemAry)
	{
		if (!Item.m_ItemData)
		{
			continue;
		}

		PRINTF("ItemName: %s,Index: %d, Stack:%d", *Item.m_ItemData->m_NameID.ToString(), Item.m_nGridIndex, Item.m_nCurrentStack);
	}
}

void Inventory::GetInvenSize(int & x, int & y)
{
	x = m_nXGridCount;
	y = m_nYGridCount;
}

FItemInstance & Inventory::GetItemRef(int index)
{
	return m_ItemAry[index];
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
