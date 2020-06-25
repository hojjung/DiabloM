#include "DiaInvenGridPanel.h"
#include "Components/GridSlot.h"
#include "Widgets/DefaultMenu.h"
#include "Objs/Inventory.h"
#include "Datas/ItemDataTable.h"



UDiaInvenGridPanel::UDiaInvenGridPanel(const FObjectInitializer& objInit):Super(objInit)
{
	m_ClassGridSlot = UDiaInvenGridSlot::StaticClass();
}

void UDiaInvenGridPanel::Init(Inventory* itemContainer )
{
	m_Inven = itemContainer;

	int X, Y;

	m_Inven->GetInvenSize(X,Y);
	m_Inven->m_OnSlotChanged.BindUObject(this, &UDiaInvenGridPanel::UpdateSlot);
	SetGrid(X, Y);
}

void UDiaInvenGridPanel::SetGrid(int x, int y)
{
	int InvenX = x;
	int InvenY = y;

	m_ArySlot.Reserve(InvenX*InvenY);

	int Index = 0;
	for (int X = 0; X < InvenX; X++)
	{
		for (int Y = 0; Y < InvenY; Y++)
		{
			m_SlotGridPanel->SetColumnFill(Y, 1);
			m_SlotGridPanel->SetRowFill(X, 1);
			UDiaInvenGridSlot* SlotCreated = CreateWidget<UDiaInvenGridSlot>(this, m_ClassGridSlot);
			UGridSlot* ChildSlot = m_SlotGridPanel->AddChildToGrid(SlotCreated);
			ChildSlot->SetColumn(Y);
			ChildSlot->SetRow(X);
			m_ArySlot.Add(SlotCreated);
			SlotCreated->InitSlot(Index,this);
			Index++;
		}
	}
}


void UDiaInvenGridPanel::UpdateSlot(int index, const FItemInstance& itemInst)
{
	m_ArySlot[index]->SetSlot(itemInst);
}


void UDiaInvenGridPanel::AddItem(int index, FItemInstance & itemWantAdd)
{
	m_Inven->AddItem(index, itemWantAdd);
}

void UDiaInvenGridPanel::AddItemStack(int index )
{
	m_Inven->AddItemStack(index);
}

void UDiaInvenGridPanel::RemoveItem(int index)
{
	m_Inven->RemoveItem(index);
}

void UDiaInvenGridPanel::RemoveItemStack(int index )
{
	m_Inven->RemoveItemStack(index );
}

bool UDiaInvenGridPanel::OnDropHeapedIndex(int dropIndex, int dragIndex)
{

	return m_Inven->OnDropIndexHeaped(dropIndex, dragIndex);

}
