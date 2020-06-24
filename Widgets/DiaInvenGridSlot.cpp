#include "DiaInvenGridSlot.h"
#include "Datas/ItemDataTable.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widgets/DiaDragDrop.h"


UDiaDragDrop* UDiaInvenGridSlot::GetDDOInst = nullptr;


void UDiaInvenGridSlot::InitSlot(int indexFromGrid)
{
	m_nIndex = indexFromGrid;
	ClearSlot();
}

void UDiaInvenGridSlot::SetSlot(const FItemInstance & itemInstance)
{
	UpdateText(itemInstance);

	UpdateEffectBG(itemInstance);

	UpdateItemVisual(itemInstance);


	m_CopiedItemData = itemInstance;
}

void UDiaInvenGridSlot::ClearSlot()
{
	m_TextItemStackCount->SetVisibility(ESlateVisibility::Hidden);
	m_ImgItemEffectBG->SetVisibility(ESlateVisibility::Hidden);
	m_ImgItemVisual->SetVisibility(ESlateVisibility::Hidden);
}

void UDiaInvenGridSlot::UpdateText(const FItemInstance& itemInstance)
{
	if (itemInstance.GetItemData().m_bStackable)
	{
		m_TextItemStackCount->SetVisibility(ESlateVisibility::Visible);
		m_TextItemStackCount->SetText(FText::AsNumber(itemInstance.GetCurrentStack()));
	}
	else
	{
		m_TextItemStackCount->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UDiaInvenGridSlot::UpdateEffectBG(const FItemInstance& itemInstance)
{
	m_ImgItemEffectBG->SetVisibility(ESlateVisibility::Visible);
	FLinearColor ColorW = itemInstance.GetItemData().m_Color;
	m_ImgItemEffectBG->SetColorAndOpacity(ColorW);
	m_ImgItemEffectBG->SetBrushTintColor(FSlateColor(ColorW));
}

void UDiaInvenGridSlot::UpdateItemVisual(const FItemInstance& itemInstance)
{
	m_ImgItemVisual->SetVisibility(ESlateVisibility::Visible);
	FSlateBrush ItemIcon = FSlateBrush();
	ItemIcon.SetResourceObject(itemInstance.GetItemData().m_ItemIcon);
	m_ImgItemVisual->SetBrush(ItemIcon);
}

void UDiaInvenGridSlot::SetIconOpacity(float opacityMaxOne)
{
	m_ImgItemVisual->SetOpacity(opacityMaxOne);
}

void UDiaInvenGridSlot::SetSlotFocus()
{
	//m_ImgSlotBG->SetBrushTintColor
}

void UDiaInvenGridSlot::ClearSlotFocus()
{

}


UDiaDragDrop * UDiaInvenGridSlot::CreateDDO(const FItemInstance & itemInst)
{
	auto* DDO = Cast<UDiaDragDrop>(UWidgetBlueprintLibrary::CreateDragDropOperation(m_ClassDDO));
	DDO->SetDDO(itemInst);
	UDiaInvenGridSlot::GetDDOInst = DDO;
	return DDO;
}


FReply UDiaInvenGridSlot::NativeOnMouseButtonDown(const FGeometry & MyGeometry, const FPointerEvent & MouseEvent)
{
	FReply Repl = Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent,this,EKeys::LeftMouseButton).NativeReply;
}

FReply UDiaInvenGridSlot::NativeOnMouseButtonUp(const FGeometry & MyGeometry, const FPointerEvent & MouseEvent)
{
	FReply Repl = Super::NativeOnMouseButtonUp(MyGeometry, MouseEvent);

	return Repl;
}

void UDiaInvenGridSlot::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry,InMouseEvent,OutOperation);

	CreateDDO();
}

void UDiaInvenGridSlot::NativeOnDragEnter(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	PRINTF("Enter");
}

void UDiaInvenGridSlot::NativeOnDragLeave(const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent,InOperation);
	PRINTF("Leave");
}

bool UDiaInvenGridSlot::NativeOnDragOver(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDragOver(InGeometry,InDragDropEvent,InOperation);
	PRINTF("over");
}

bool UDiaInvenGridSlot::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDrop(InGeometry,InDragDropEvent,InOperation);
	//Cast<UDiaDragDrop>(InOperation)->Drop();
	//끼울수 없는 슬롯이면 캔슬
	//빈 슬롯이면 끼워주고 이전 슬롯 클리어,아이템 새로 추가 및 삭제
	//
}

void UDiaInvenGridSlot::NativeOnDragCancelled(const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent,InOperation);
	//Cast<UDiaDragDrop>(InOperation)->DragCancel();
}
