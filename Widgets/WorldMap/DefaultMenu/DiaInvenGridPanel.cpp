#include "DiaInvenGridPanel.h"
#include "Widgets/WorldMap/DefaultMenu/DefaultMenu.h"
#include "Item/Inventory.h"
#include "Datas/ItemDataTable.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"

UDiaInvenGridPanel* UDiaInvenGridPanel::GetInvenWidgetInst = nullptr;

UDiaInvenGridPanel::UDiaInvenGridPanel(const FObjectInitializer& objInit):Super(objInit)
{
	m_nPopupSelectedIndex=-1;
	m_ClassGridSlot = UDiaInvenGridSlot::StaticClass();
}

void UDiaInvenGridPanel::Init(UInventory* itemContainer )
{
	m_Inven = itemContainer;

	int X, Y;

	m_Inven->GetInvenSize(X,Y);
	m_Inven->GetItemChangeCallback().AddUObject(this, &UDiaInvenGridPanel::UpdateSlot);
	SetGrid(X, Y);

	UDiaInvenGridPanel::GetInvenWidgetInst=this;

	for(int i=0; i<m_Inven->GetItemAry().Num();i++)
	{
		UpdateSlot(i,m_Inven->GetItemRef(i));
	}

	UpdateGold(ADiabloPlayerController::Get->GetPlayerPawn()->GetGold());
}




void UDiaInvenGridPanel::SetGrid(int x, int y)
{
	int InvenX = x;
	int InvenY = y;

	m_ArySlot.Reserve(InvenX*InvenY);

	int Index = 0;
	for (int Y = 0; Y < InvenY; Y++)
	{
		for (int X = 0; X < InvenX; X++)
		{
				m_SlotGridPanel->SetColumnFill(X, 1);
				m_SlotGridPanel->SetRowFill(Y, 1);
				UDiaInvenGridSlot* SlotCreated = CreateWidget<UDiaInvenGridSlot>(this, m_ClassGridSlot);
				UGridSlot* ChildSlot = m_SlotGridPanel->AddChildToGrid(SlotCreated);
				ChildSlot->SetColumn(X);
				ChildSlot->SetRow(Y);
				m_ArySlot.Add(SlotCreated);
				SlotCreated->InitSlot(Index);
				SlotCreated->m_OnDropIndex.BindUObject(this,&UDiaInvenGridPanel::AddItem);
				Index++;
		}
	}
}


void UDiaInvenGridPanel::UpdateSlot(int index, FItemInstance& itemInst)
{
	m_ArySlot[index]->SetSlot(itemInst);

}


bool UDiaInvenGridPanel::AddItem(int index, FItemInstance & itemWantAdd)
{
	return m_Inven->AddItem(index, itemWantAdd);
}

bool UDiaInvenGridPanel::AddItemAuto(FItemInstance& itemWantAdd)
{
	return m_Inven->AddItemAuto(itemWantAdd);
}

void UDiaInvenGridPanel::AddItemStack(int index )
{
	m_Inven->AddItemStack(index);
}


void UDiaInvenGridPanel::UpdateGold(float v)
{
	if(v>100000000.f)
	{
		m_TextGold->SetText(UDiaBlueprintFunctionLibrary::GetAlphabetText(v));	
	}
	else
	{
		m_TextGold->SetText(FText::AsNumber(v));
	}
}

