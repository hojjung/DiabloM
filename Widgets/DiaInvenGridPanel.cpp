#include "DiaInvenGridPanel.h"
#include "Components/GridSlot.h"
#include "Widgets/DefaultMenu.h"
#include "Objs/Inventory.h"
#include "Datas/ItemDataTable.h"
#include "Components/CanvasPanelSlot.h"
#include "WidgetBlueprintLibrary.h"
#include "WidgetLayoutLibrary.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"

UDiaInvenGridPanel::UDiaInvenGridPanel(const FObjectInitializer& objInit):Super(objInit)
{
	m_nPopupSelectedIndex=-1;
	m_ClassGridSlot = UDiaInvenGridSlot::StaticClass();
}

void UDiaInvenGridPanel::Init(Inventory* itemContainer )
{
	m_Inven = itemContainer;

	int X, Y;

	m_Inven->GetInvenSize(X,Y);
	m_Inven->GetItemChangeCallback().AddUObject(this, &UDiaInvenGridPanel::UpdateSlot);
	SetGrid(X, Y);

	m_ItemPopup->GetUseButton()->SetVisibility(ESlateVisibility::Hidden);
	m_ItemPopup->GetEquipButton()->SetVisibility(ESlateVisibility::Hidden);

}



void UDiaInvenGridPanel::ShowItemInfo(const FGeometry & theInstigator,  FItemInstance & itemInst)
{
	if (m_ItemPopup->GetVisibility() == ESlateVisibility::SelfHitTestInvisible && m_nPopupSelectedIndex==itemInst.m_nGridIndex)
	{
		m_ItemPopup->PlayHideInfoAnim();
		return;
	}
	
	m_nPopupSelectedIndex=itemInst.m_nGridIndex;

	m_ItemPopup->ShowInfoPanel(itemInst);
	m_ItemPopup->SetPanelPosition(theInstigator);
}

void UDiaInvenGridPanel::HideItemInfo()
{
	m_ItemPopup->PlayHideInfoAnim();
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
			SlotCreated->InitSlot(Index);
			SlotCreated->m_OnDropIndex.BindUObject(this,&UDiaInvenGridPanel::AddItem);
			SlotCreated->m_OnClicked.AddUObject(this, &UDiaInvenGridPanel::ShowItemInfo);
			SlotCreated->m_OnDragDetect.BindUObject(this, &UDiaInvenGridPanel::HideItemInfo);
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

void UDiaInvenGridPanel::AddItemStack(int index )
{
	m_Inven->AddItemStack(index);
}

void UDiaInvenGridPanel::RemoveItem(int index)
{
	//m_Inven->RemoveItem(index);
}

void UDiaInvenGridPanel::RemoveItemStack(int index )
{
	//m_Inven->RemoveItemStack(index );
}

