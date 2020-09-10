#include "Inventory.h"
#include "Datas/ItemDataTable.h"
#include "Managers/DiabloGameInstance.h"
#include "Item/ItemManager.h"

UInventory::~UInventory()
{
	m_nCurrentEmptyIndex = -1;
	m_nXGridCount = -1;
	m_nYGridCount = -1;

}

void UInventory::InitInven(int xCount, int yCount)
{
	m_nCurrentEmptyIndex = 0;

	m_nXGridCount = xCount;

	m_nYGridCount = yCount;

	int CountTotal = m_nXGridCount * m_nYGridCount;

	m_ItemAry.Reserve(CountTotal);

	m_ItemAry.Init(FItemInstance(), CountTotal);
}

bool UInventory::CheckSlotValid(int droppedIndex, FItemInstance & itemWantAdd)
{
	return &m_ItemAry[droppedIndex];
}

void UInventory::SetItem(int droppedIndex, FItemInstance & itemWantAdd)
{
	m_ItemAry[droppedIndex] = itemWantAdd;
	m_ItemAry[droppedIndex].m_nGridIndex = droppedIndex;
	m_ItemAry[droppedIndex].m_Holder = this;
	m_OnSlotChanged.Broadcast(droppedIndex, m_ItemAry[droppedIndex]);
}

void UInventory::AddItemStack(int index)
{
	m_ItemAry[index].m_nCurrentStack++;

	m_OnSlotChanged.Broadcast(index, m_ItemAry[index]);
}

void UInventory::RemoveItem(FItemInstance & itemWantErase)
{
	RemoveItemByIndex(itemWantErase.m_nGridIndex);
}

void UInventory::RemoveItemByIndex(int index)
{
	m_ItemAry[index].ClearData();
	m_OnSlotChanged.Broadcast(index, m_ItemAry[index]);
}

void UInventory::RemoveItemStack(int index)
{
	m_ItemAry[index].m_nCurrentStack--;

	if (m_ItemAry[index].m_nCurrentStack <= 0)
	{
		RemoveItemByIndex(index);
	}

	m_OnSlotChanged.Broadcast(index, m_ItemAry[index]);
}

bool UInventory::AddItem(int droppedIndex, FItemInstance& itemWantAdd)//빌드후 여기도
{
	//return true;
	if (this == static_cast<UInventory*>(itemWantAdd.m_Holder) && droppedIndex == itemWantAdd.m_nGridIndex)
	{
		PRINTF("Prevent MySelf");
		return false;
	}
//ㄴsafe
	//이아래임
	if (CheckSlotValid(droppedIndex,itemWantAdd) && m_ItemAry[droppedIndex].m_ItemID==NAME_None)
	{
		SetItem(droppedIndex, itemWantAdd);
		//이아래임
		itemWantAdd.m_Holder->RemoveItem(itemWantAdd);
		return true;
	}
	///
	int DragIndex = itemWantAdd.m_nGridIndex;

	FItemInstance Drop = m_ItemAry[droppedIndex];

	//Stack
	bool Result = false;
	//safe
	if (itemWantAdd.GetIsStackable() && itemWantAdd.CheckCanStack()&&
		Drop.GetIsStackable() && Drop.CheckCanStack() &&
		Drop.m_ItemID == itemWantAdd.m_ItemID)//스왑방지코드
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
	//safe
	
	if (Result)
	{
		m_OnSlotChanged.Broadcast(droppedIndex, m_ItemAry[droppedIndex]);
		m_OnSlotChanged.Broadcast(DragIndex, m_ItemAry[DragIndex]);
	}

	return Result;
}

bool UInventory::AddItemAuto(FItemInstance& item_instance)
{
	int Result =GetEmptyIndex();

	if(Result==-1)
	{
		return false;
	}

	return AddItem(Result,item_instance);
}

bool UInventory::SwapMove(FItemInstance &Drop, FItemInstance &Drag)
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

void UInventory::StackMove(FItemInstance &Drop, FItemInstance &Drag, IItemHolder* preItemHolder)
{
	int DiffStackCount = Drop.m_nMaxStack - Drop.m_nCurrentStack;

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

void UInventory::PrintInven()
{
	PRINTF("----InvenPrint----");

	for (FItemInstance& Item : m_ItemAry)
	{
		if (Item.m_ItemID==NAME_None)
		{
			continue;
		}
;
		PRINTF("ItemName: %s,Index: %d, Stack:%d", *UDiabloGameInstance::Get->GetItemData(Item.m_ItemID).m_ShowingName.ToString(), Item.m_nGridIndex, Item.m_nCurrentStack);
	}
}

void UInventory::GetInvenSize(int & x, int & y)
{
	x = m_nXGridCount;
	y = m_nYGridCount;
}

FItemInstance & UInventory::GetItemRef(int index)
{
	return m_ItemAry[index];
}

const TArray<FItemInstance>& UInventory::GetItemAry() const
{
	return m_ItemAry;	
}

void UInventory::SetItemAry(TArray<FItemInstance>& loadedAry)
{
	m_ItemAry=loadedAry;

	for(int i=0; i< m_ItemAry.Num();i++)
	{
		m_ItemAry[i].m_Holder=this;
		m_OnSlotChanged.Broadcast(i, m_ItemAry[i]);
	}
}

int UInventory::GetEmptyIndex()
{
	for(int i=0; i< m_ItemAry.Num();i++)
	{
		if(m_ItemAry[i].IsEmpty())
		{
			return i;
		}
	}
	return  -1;
}

