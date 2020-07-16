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

bool Inventory::CheckSlotValid(int droppedIndex, FItemInstance & itemWantAdd)
{
	return &m_ItemAry[droppedIndex];
}

void Inventory::SetItem(int droppedIndex, FItemInstance & itemWantAdd)
{
	m_ItemAry[droppedIndex] = itemWantAdd;
	m_ItemAry[droppedIndex].m_nGridIndex = droppedIndex;
	m_ItemAry[droppedIndex].m_Holder = this;
	m_OnSlotChanged.Broadcast(droppedIndex, m_ItemAry[droppedIndex]);
}

void Inventory::AddItemStack(int index)
{
	m_ItemAry[index].m_nCurrentStack++;

	m_OnSlotChanged.Broadcast(index, m_ItemAry[index]);
}

void Inventory::RemoveItem(FItemInstance & itemWantErase)
{
	RemoveItemByIndex(itemWantErase.m_nGridIndex);
}

void Inventory::RemoveItemByIndex(int index)
{
	m_ItemAry[index].ClearData();
	m_OnSlotChanged.Broadcast(index, m_ItemAry[index]);
}

void Inventory::RemoveItemStack(int index)
{
	m_ItemAry[index].m_nCurrentStack--;

	if (m_ItemAry[index].m_nCurrentStack <= 0)
	{
		RemoveItemByIndex(index);
	}

	m_OnSlotChanged.Broadcast(index, m_ItemAry[index]);
}

bool Inventory::AddItem(int droppedIndex, FItemInstance& itemWantAdd)//빌드후 여기도
{
	if (this == static_cast<Inventory*>(itemWantAdd.m_Holder) && droppedIndex == itemWantAdd.m_nGridIndex)
	{
		PRINTF("Prevent MySelf");
		return false;
	}

	if (CheckSlotValid(droppedIndex,itemWantAdd) && !m_ItemAry[droppedIndex].m_ItemData)
	{
		SetItem(droppedIndex, itemWantAdd);
		itemWantAdd.m_Holder->RemoveItem(itemWantAdd);
		PRINTF("Success1");
		return true;
	}

	int DragIndex = itemWantAdd.m_nGridIndex;

	FItemInstance Drop = m_ItemAry[droppedIndex];

	//Stack
	bool Result = false;

	if (itemWantAdd.GetIsStackable() && itemWantAdd.CheckCanStack()&&
		Drop.GetIsStackable() && Drop.CheckCanStack() &&
		Drop.m_ItemData == itemWantAdd.m_ItemData)//스왑방지코드
	{
		StackMove(Drop, itemWantAdd, itemWantAdd.m_Holder);

		Result = true;
		PRINTF("Stack");
	}
	else
	{
		//Swap
		PRINTF("SWap");
		Result=SwapMove(Drop, itemWantAdd);
	}

	if (Result)
	{
		m_OnSlotChanged.Broadcast(droppedIndex, m_ItemAry[droppedIndex]);
		m_OnSlotChanged.Broadcast(DragIndex, m_ItemAry[DragIndex]);
	}

	return Result;
}

bool Inventory::SwapMove(FItemInstance &Drop, FItemInstance &Drag)
{
	int DropIndex = Drop.m_nGridIndex;
	int DragIndex = Drag.m_nGridIndex;

	if (!Drag.m_Holder->CheckSlotValid(DragIndex, Drop))
	{
		return false;
	}

	Drag.m_Holder->SetItem(DragIndex, Drop);
	SetItem(DropIndex, Drag);

	return true;
}

void Inventory::StackMove(FItemInstance &Drop, FItemInstance &Drag, IItemHolder* preItemHolder)
{
	int DiffStackCount = Drop.m_ItemData->m_nMaxStack - Drop.m_nCurrentStack;

	int Count = FMath::Min(DiffStackCount, Drag.m_nCurrentStack);

	while (Count--)
	{
		AddItemStack(Drop.m_nGridIndex);
		Drag.m_nCurrentStack--;
	}

	if (Drag.m_nCurrentStack <= 0)
	{
		preItemHolder->RemoveItem(Drag);
	}

}

void Inventory::PrintInven()
{
	PRINTF("----InvenPrint----");

	for (auto& Item : m_ItemAry)
	{
		if (!Item.m_ItemData)
		{
			continue;
		}

		PRINTF("ItemName: %s,Index: %d, Stack:%d", *Item.m_ItemData->m_ShowingName.ToString(), Item.m_nGridIndex, Item.m_nCurrentStack);
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

