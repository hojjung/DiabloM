#include "DiaInvenGridSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widgets/DiaDragDrop.h"


UDiaDragDrop* UDiaInvenGridSlot::GetDDOInst = nullptr;


void UDiaInvenGridSlot::InitSlot(int indexFromGrid, UDiaInvenGridPanel* parentGrid)
{
	m_ParentGrid = nullptr;
	m_ParentGrid = parentGrid;

	m_nIndex = indexFromGrid;
	
	ClearSlot();
}

void UDiaInvenGridSlot::SetSlot(const FItemInstance & itemInstance)
{
	if (!itemInstance.m_ItemData)
	{
		ClearSlot();
		return;
	}

	UpdateText(itemInstance);

	UpdateEffectBG(itemInstance);

	UpdateItemVisual(itemInstance);

	m_CopiedItemData = itemInstance;
}

void UDiaInvenGridSlot::ClearSlot()
{
	m_ImgSlotHighlight->SetVisibility(ESlateVisibility::Hidden);
	m_TextItemStackCount->SetVisibility(ESlateVisibility::Hidden);
	m_ImgItemEffectBG->SetVisibility(ESlateVisibility::Hidden);
	m_ImgItemVisual->SetVisibility(ESlateVisibility::Hidden);
	SetIconOpacity(1.0f);
	m_CopiedItemData.ClearData();
}

void UDiaInvenGridSlot::UpdateText(const FItemInstance& itemInstance)
{
	if (itemInstance.m_ItemData->m_bStackable)
	{
		m_TextItemStackCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_TextItemStackCount->SetText(FText::AsNumber(itemInstance.m_nCurrentStack));
	}
	else
	{
		m_TextItemStackCount->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UDiaInvenGridSlot::UpdateEffectBG(const FItemInstance& itemInstance)
{
	m_ImgItemEffectBG->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FLinearColor ColorW = itemInstance.m_ItemData->m_Color;
	m_ImgItemEffectBG->SetColorAndOpacity(ColorW);
	m_ImgItemEffectBG->SetBrushTintColor(FSlateColor(ColorW));
}

void UDiaInvenGridSlot::UpdateItemVisual(const FItemInstance& itemInstance)
{
	m_ImgItemVisual->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FSlateBrush ItemIcon = FSlateBrush();
	ItemIcon.SetResourceObject(itemInstance.m_ItemData->m_ItemIcon);
	m_ImgItemVisual->SetBrush(ItemIcon);
}

void UDiaInvenGridSlot::SetIconOpacity(float opacityMaxOne)
{
	m_ImgItemVisual->SetOpacity(opacityMaxOne);
}

bool UDiaInvenGridSlot::IsSlotEmpty()
{
	return !m_CopiedItemData.m_ItemData;
}

void UDiaInvenGridSlot::SetSlotFocus()
{
	m_ImgSlotHighlight->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UDiaInvenGridSlot::ClearSlotFocus()
{
	m_ImgSlotHighlight->SetVisibility(ESlateVisibility::Hidden);
}


UDiaDragDrop * UDiaInvenGridSlot::CreateDDO(const FItemInstance & itemInst)
{
	auto* DDO = Cast<UDiaDragDrop>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDiaDragDrop::StaticClass()));
	DDO->SetDDO(itemInst);
	DDO->InitDrag(this);
	UDiaInvenGridSlot::GetDDOInst = DDO;
	return DDO;
}

bool UDiaInvenGridSlot::CanDrop(const FItemInstance & itemInst)
{//implemet require
	return true;
}


FReply UDiaInvenGridSlot::NativeOnMouseButtonDown(const FGeometry & MyGeometry, const FPointerEvent & MouseEvent)
{
	FReply Repl = Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent,this,EKeys::LeftMouseButton).NativeReply;
}

void UDiaInvenGridSlot::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry,InMouseEvent,OutOperation);

	if (IsSlotEmpty())
	{
		return;
	}
	OutOperation = CreateDDO(m_CopiedItemData);
}

void UDiaInvenGridSlot::NativeOnDragEnter(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	SetSlotFocus();
	//PRINTF("Enter");
}

void UDiaInvenGridSlot::NativeOnDragLeave(const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent,InOperation);
	ClearSlotFocus();
	//PRINTF("Leave");
}

bool UDiaInvenGridSlot::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	bool Result=Super::NativeOnDrop(InGeometry,InDragDropEvent,InOperation);
	ClearSlotFocus();
	PRINTF("Dropped Slot Index:%d",m_nIndex);
	UDiaInvenGridSlot::GetDDOInst->m_PreSlot->SetIconOpacity(1.f);

	if (!CanDrop(UDiaInvenGridSlot::GetDDOInst->m_DraggedItem))
	{
		return false;
	}

	int DropIndex = m_nIndex;
	int DragIndex = UDiaInvenGridSlot::GetDDOInst->m_PreSlot->m_nIndex;

	Result=m_ParentGrid->OnDropHeapedIndex(DropIndex, DragIndex);

	return Result;
}

