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
	m_ItemPopup->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_ItemPopup->SetInfoPanel(itemInst);
	//
	auto* PanelSlot = Cast<UCanvasPanelSlot>(m_ItemPopup->Slot);
	ClickedItemSlot = this->GetCachedGeometry().AbsoluteToLocal(theInstigator.GetAbsolutePosition()) + theInstigator.GetLocalSize() / 2.0f;
	ClickedItemSlot.X -= m_ItemPopup->GetDesiredSize().X / 2.0f;

	auto Geo= UWidgetLayoutLibrary::GetPlayerScreenWidgetGeometry(GetOwningPlayer());

	float ScreenY = Geo.GetAbsoluteSize().Y;
	float PopupSizeY = ((m_ItemPopup->GetDesiredSize().Y*Geo.Scale))/2.0f;
	float ScreenTopToItem = ClickedItemSlot.Y*Geo.Scale;
	float ScreenBottomToItem= ScreenY-ClickedItemSlot.Y*Geo.Scale;

	float ReverseScale = 1.f / Geo.Scale;

	if (ScreenTopToItem < PopupSizeY)
	{
		float Diff = PopupSizeY - ScreenTopToItem;

		ClickedItemSlot.Y += Diff * ReverseScale;
		ClickedItemSlot.Y += 50.f;
	}
	else if (ScreenBottomToItem < PopupSizeY)
	{
		float Diff = PopupSizeY - FMath::Abs(ScreenBottomToItem);

		ClickedItemSlot.Y -= Diff * ReverseScale;
		ClickedItemSlot.Y -= 50.f ;
	}

	PanelSlot->SetPosition(ClickedItemSlot);
}

void UDiaInvenGridPanel::HideItemInfo()
{
	m_ItemPopup->SetVisibility(ESlateVisibility::Hidden);
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
			SlotCreated->m_OnClicked.BindUObject(this, &UDiaInvenGridPanel::ShowItemInfo);
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

void UDiaInvenGridPanel::BoxDraw2(FPaintContext & cont, FVector2D point, float widthheight, FLinearColor color) const
{
	float newH = widthheight / 2.0f;

	FVector2D p1 = point;

	p1.X -= newH;
	p1.Y += newH;

	FVector2D p2 = p1;

	p2.X += widthheight;

	UWidgetBlueprintLibrary::DrawLine(cont, p1, p2, color);

	FVector2D p3 = p1;
	FVector2D p4 = p2;

	p3.Y -= widthheight;
	p4.Y -= widthheight;

	UWidgetBlueprintLibrary::DrawLine(cont, p3, p4, color);

	UWidgetBlueprintLibrary::DrawLine(cont, p1, p3, color);

	UWidgetBlueprintLibrary::DrawLine(cont, p2, p4, color);
}

int32 UDiaInvenGridPanel::NativePaint(const FPaintArgs & Args, const FGeometry & AllottedGeometry, const FSlateRect & MyCullingRect, FSlateWindowElementList & OutDrawElements, int32 LayerId, const FWidgetStyle & InWidgetStyle, bool bParentEnabled) const
{
	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	BoxDraw2(Context, ClickedItemSlot, 40.0f, FLinearColor::Red);
	BoxDraw2(Context, B, 40.0f, FLinearColor::Blue);

	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

