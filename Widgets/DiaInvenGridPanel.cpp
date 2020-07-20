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
}



void UDiaInvenGridPanel::ShowItemInfo(const FGeometry & theInstigator, const FItemInstance & itemInst)
{
	if (m_ItemPopup->GetVisibility() == ESlateVisibility::SelfHitTestInvisible && m_nPopupSelectedIndex==itemInst.m_nGridIndex)
	{
		m_ItemPopup->PlayHideInfoAnim();
		return;
	}
	
	m_nPopupSelectedIndex=itemInst.m_nGridIndex;
	
	m_ItemPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_ItemPopup->ShowInfoPanel(itemInst);
	//
	auto Geo= UWidgetLayoutLibrary::GetPlayerScreenWidgetGeometry(GetOwningPlayer());
	auto* PanelSlot = Cast<UCanvasPanelSlot>(m_ItemPopup->Slot);
	auto ClickedItemSlot = this->GetCachedGeometry().AbsoluteToLocal(theInstigator.GetAbsolutePosition()) + theInstigator.GetLocalSize() / 2.0f;

	ClickedItemSlot.X -= (m_ItemPopup->GetDesiredSize().X / 2.0f) + (theInstigator.GetLocalSize().X / 2.0f);

	float ScreenY = Geo.GetAbsoluteSize().Y;
	float PopupSizeY = (m_ItemPopup->GetDesiredSize().Y*Geo.Scale)/2.0f;
	float ScreenTopToItem = ClickedItemSlot.Y*Geo.Scale;
	float ScreenBottomToItem= ScreenY-ClickedItemSlot.Y*Geo.Scale;

	float ReverseScale = 1.f / Geo.Scale;

	if (ScreenTopToItem < PopupSizeY)
	{
		float Diff = PopupSizeY - ScreenTopToItem;

		ClickedItemSlot.Y += Diff * ReverseScale;
	}
	else if (ScreenBottomToItem < PopupSizeY)
	{
		float Diff = PopupSizeY - FMath::Abs(ScreenBottomToItem);

		ClickedItemSlot.Y -= Diff * ReverseScale;
	}

	PanelSlot->SetPosition(ClickedItemSlot);
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
			SlotCreated->m_OnClicked.AddDynamic(this, &UDiaInvenGridPanel::ShowItemInfo);
			SlotCreated->m_OnDragDetect.BindUObject(this, &UDiaInvenGridPanel::HideItemInfo);
			Index++;
		}
	}
}


void UDiaInvenGridPanel::UpdateSlot(int index, const FItemInstance& itemInst)
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

