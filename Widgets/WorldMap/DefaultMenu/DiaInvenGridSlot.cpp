#include "DiaInvenGridSlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Managers/ItemManagerOld.h"
#include "Widgets/WorldMap/DefaultMenu/DiaDragDrop.h"

UDiaDragDrop* UDiaInvenGridSlot::GetDDOInst = nullptr;


void UDiaInvenGridSlot::InitSlot(int indexFromGrid)
{
	m_nIndex = indexFromGrid;
	
	ClearSlot();
}

void UDiaInvenGridSlot::SetSlot(const FItemInstance & itemInstance)
{
	if (itemInstance.m_ItemID==NAME_None)
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
	SetVisualColorTint(FColor::White);
	ClearSlotFocus();
}

void UDiaInvenGridSlot::UpdateText(const FItemInstance& itemInstance)
{
	if (itemInstance.m_bStackable)
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

	FLinearColor ColorW =itemInstance.m_ItemData->GetItemTier().m_TierColor;
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

void UDiaInvenGridSlot::SetVisualColorTint(FLinearColor colorW)
{
	m_ImgItemVisual->SetBrushTintColor(FSlateColor(colorW));
	
}

void UDiaInvenGridSlot::SetHighlightColorTint(FLinearColor colorW)
{
	m_ImgSlotHighlight->SetBrushTintColor(FSlateColor(colorW));
}

bool UDiaInvenGridSlot::IsSlotEmpty()
{
	return m_CopiedItemData.IsEmpty();
}

void UDiaInvenGridSlot::SetSlotFocus(UDiaDragDrop* ddo)
{
	if (m_OnDragIndex.IsBound())
	{
		if (m_OnDragIndex.Execute(m_nIndex, UDiaInvenGridSlot::GetDDOInst->m_DraggedItem))
		{
			SetHighlightColorTint(FLinearColor::White);
		}
		else
		{
			SetHighlightColorTint(FLinearColor::Red);
		}
	}

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


FReply UDiaInvenGridSlot::NativeOnMouseButtonUp(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	FReply Repl = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (!IsSlotEmpty())
		m_OnClicked.Broadcast(InGeometry, m_CopiedItemData);

	return Repl;
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
	m_OnDragDetect.ExecuteIfBound();
	OutOperation = CreateDDO(m_CopiedItemData);
}

void UDiaInvenGridSlot::NativeOnDragEnter(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	SetSlotFocus(UDiaInvenGridSlot::GetDDOInst);
}

void UDiaInvenGridSlot::NativeOnDragLeave(const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent,InOperation);
	ClearSlotFocus();
}

bool UDiaInvenGridSlot::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	bool Result=Super::NativeOnDrop(InGeometry,InDragDropEvent,InOperation);

	ClearSlotFocus();

	UDiaInvenGridSlot::GetDDOInst->m_PreSlot->SetIconOpacity(1.f);

	Result= m_OnDropIndex.Execute(m_nIndex, UDiaInvenGridSlot::GetDDOInst->m_DraggedItem);

	return Result;
}

