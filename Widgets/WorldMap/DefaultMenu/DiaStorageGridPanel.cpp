#include "DiaStorageGridPanel.h"
#include "DiaInvenGridSlot.h"
#include "Widgets/WorldMap/DefaultMenu/DefaultMenu.h"
#include "Item/Inventory.h"
#include "Datas/ItemDataTable.h"



UDiaStorageGridPanel* UDiaStorageGridPanel::GetStorageWidgetInst = nullptr;

UDiaStorageGridPanel::UDiaStorageGridPanel(const FObjectInitializer& objInit):Super(objInit)
{
	m_nPopupSelectedIndex=-1;
	m_ClassGridSlot = UDiaInvenGridSlot::StaticClass();
	m_nCurrentSelectedPanelIndex=0;
}

void UDiaStorageGridPanel::Init(TArray<UInventory*>* itemContainer)
{
	m_AryGridPanels.Reset();
	m_AryGridPanels.Emplace(m_SlotGridPanel1);
	m_AryGridPanels.Emplace(m_SlotGridPanel2);
	m_AryGridPanels.Emplace(m_SlotGridPanel3);
	m_AryGridPanels.Emplace(m_SlotGridPanel4);
	m_AryGridPanels.Emplace(m_SlotGridPanel5);

	m_AryArySlots.Reset();
	m_AryArySlots.Emplace(&m_ArySlot1);
	m_AryArySlots.Emplace(&m_ArySlot2);
	m_AryArySlots.Emplace(&m_ArySlot3);
	m_AryArySlots.Emplace(&m_ArySlot4);
	m_AryArySlots.Emplace(&m_ArySlot5);
	
	m_AryStorages = itemContainer;
	UDiaStorageGridPanel::GetStorageWidgetInst=this;

	int X=0;
	int Y=0;
	
	int MaxIter=m_AryStorages->Num();
	int Iter=0;
	
	while (Iter<MaxIter)
	{
		UInventory* CurrentInven = (*m_AryStorages)[Iter];
		
		CurrentInven->GetInvenSize(X,Y);
		
		CurrentInven->GetItemChangeCallback().AddUObject(this, &UDiaStorageGridPanel::UpdateSlot);

		SetGrid(Iter,X,Y);

		m_nCurrentSelectedPanelIndex=Iter;
		
		for(int i=0; i<CurrentInven->GetItemAry().Num();i++)
		{
			UpdateSlot(i,CurrentInven->GetItemRef(i));
		}
		
		Iter++;
	}
	
	m_nCurrentSelectedPanelIndex=0;
}

void UDiaStorageGridPanel::SetGrid(int indexPanel,int x, int y)
{
	int InvenX = x;
	int InvenY = y;

	m_AryArySlots[indexPanel]->Reserve(InvenX*InvenY);

	int Index = 0;
	for (int X = 0; X < InvenX; X++)
	{
		for (int Y = 0; Y < InvenY; Y++)
		{
			m_AryGridPanels[indexPanel]->SetColumnFill(X, 1);
			m_AryGridPanels[indexPanel]->SetRowFill(Y, 1);
			
			UDiaInvenGridSlot* SlotCreated = CreateWidget<UDiaInvenGridSlot>(this, m_ClassGridSlot);
			UGridSlot* ChildSlot = m_AryGridPanels[indexPanel]->AddChildToGrid(SlotCreated);
			
			ChildSlot->SetColumn(X);
			ChildSlot->SetRow(Y);
			
			SlotCreated->InitSlot(Index);
			SlotCreated->m_OnDropIndex.BindUObject(this,&UDiaStorageGridPanel::AddItem);
			
			m_AryArySlots[indexPanel]->Add(SlotCreated);
			
			Index++;
		}
	}
}


void UDiaStorageGridPanel::UpdateSlot(int index, FItemInstance& itemInst)
{
	(*m_AryArySlots[m_nCurrentSelectedPanelIndex])[index]->SetSlot(itemInst);
}

bool UDiaStorageGridPanel::AddItem(int index, FItemInstance & itemWantAdd)
{
	return (*m_AryStorages)[m_nCurrentSelectedPanelIndex]->AddItem(index, itemWantAdd);
}

bool UDiaStorageGridPanel::AddItemAuto(FItemInstance& itemWantAdd)
{
	return (*m_AryStorages)[m_nCurrentSelectedPanelIndex]->AddItemAuto(itemWantAdd);
}

void UDiaStorageGridPanel::AddItemStack(int index )
{
	(*m_AryStorages)[m_nCurrentSelectedPanelIndex]->AddItemStack(index);
}

