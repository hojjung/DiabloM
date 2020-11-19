// My First Hack n Slash


#include "DiaShopGridSlot.h"

#include "DiaInvenGridSlot.h"
#include "Item/ItemManager.h"
#include "Widgets/WorldMap/DefaultMenu/DiaDragDrop.h"


void UDiaShopGridSlot::InitSlot(int indexFromGrid)
{
	m_nIndex = indexFromGrid;
	m_LevelFormat=FText::FromString("Lv {0}");
	ClearSlot();
}

void UDiaShopGridSlot::SetSlot(const FItemInstance & itemInstance)
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

void UDiaShopGridSlot::ClearSlot()
{
	m_ImgSlotHighlight->SetVisibility(ESlateVisibility::Hidden);
	m_TextItemStackCount->SetVisibility(ESlateVisibility::Hidden);
	m_ImgItemEffectBG->SetVisibility(ESlateVisibility::Hidden);
	m_ImgItemVisual->SetVisibility(ESlateVisibility::Hidden);
	SetIconOpacity(1.0f);
	m_CopiedItemData.ClearData();
	SetVisualColorTint(FColor::White);
	ClearSlotFocus();
	m_TextItemLevel->SetVisibility(ESlateVisibility::Hidden);
}

void UDiaShopGridSlot::UpdateText(const FItemInstance& itemInstance)
{
	if (itemInstance.GetIsStackable())
	{
		m_TextItemStackCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_TextItemStackCount->SetText(FText::AsNumber(itemInstance.m_nCurrentStack));
	}
	else
	{
		m_TextItemStackCount->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UDiaShopGridSlot::UpdateEffectBG(const FItemInstance& itemInstance)
{
	m_ImgItemEffectBG->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FLinearColor ColorW =itemInstance.m_ItemTier->m_TierColor;
	m_ImgItemEffectBG->SetColorAndOpacity(ColorW);
	m_ImgItemEffectBG->SetBrushTintColor(FSlateColor(ColorW));
}

void UDiaShopGridSlot::SetLevelText(int itemLevel)
{
	if(itemLevel>0)
	{
		m_TextItemLevel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		FFormatOrderedArguments Args;

		Args.Add(itemLevel);
		
		m_TextItemLevel->SetText(FText::Format(m_LevelFormat,Args));
	}
	else
	{
		m_TextItemLevel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UDiaShopGridSlot::UpdateItemVisual(const FItemInstance& itemInstance)
{
	m_ImgItemVisual->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FSlateBrush ItemIcon = FSlateBrush();
	ItemIcon.SetImageSize(FVector2D(128.f, 128.f));
	ItemIcon.SetResourceObject(itemInstance.m_ItemData->m_ItemIcon);
	m_ImgItemVisual->SetBrush(ItemIcon);

	SetLevelText(itemInstance.m_nItemLevel);
}

void UDiaShopGridSlot::SetIconOpacity(float opacityMaxOne)
{
	m_ImgItemVisual->SetOpacity(opacityMaxOne);
}

void UDiaShopGridSlot::SetVisualColorTint(FLinearColor colorW)
{
	m_ImgItemVisual->SetBrushTintColor(FSlateColor(colorW));
	
}

void UDiaShopGridSlot::SetHighlightColorTint(FLinearColor colorW)
{
	m_ImgSlotHighlight->SetBrushTintColor(FSlateColor(colorW));
}

bool UDiaShopGridSlot::IsSlotEmpty()
{
	return m_CopiedItemData.IsEmpty();
}

void UDiaShopGridSlot::SetSlotFocus(UDiaDragDrop* ddo)
{
	if (m_OnDragIndex.IsBound())
	{
		if (m_OnDragIndex.Execute(m_nIndex, UDiaDragDrop::GetDDOInst->m_DraggedItem))
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

void UDiaShopGridSlot::ClearSlotFocus()
{
	m_ImgSlotHighlight->SetVisibility(ESlateVisibility::Hidden);
}


UDiaDragDrop * UDiaShopGridSlot::CreateDDO(const FItemInstance & itemInst)
{
	auto* DDO = Cast<UDiaDragDrop>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDiaDragDrop::StaticClass()));
	DDO->SetDDO(itemInst);
	DDO->InitDrag(m_ImgItemVisual);
	UDiaDragDrop::GetDDOInst = DDO;
	return DDO;
}


FReply UDiaShopGridSlot::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	FReply Repl = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (!IsSlotEmpty())
		m_OnClicked.Broadcast(InGeometry, m_CopiedItemData);

	return Repl;
}


void UDiaShopGridSlot::NativeOnDragEnter(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	SetSlotFocus(UDiaDragDrop::GetDDOInst);
}

void UDiaShopGridSlot::NativeOnDragLeave(const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent,InOperation);
	ClearSlotFocus();
}

bool UDiaShopGridSlot::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	bool Result=Super::NativeOnDrop(InGeometry,InDragDropEvent,InOperation);

	ClearSlotFocus();

	UDiaDragDrop::GetDDOInst->m_PreSlot->SetOpacity(1.f);

	if(m_OnDropIndex.IsBound())
	{
		Result= m_OnDropIndex.Execute(m_nIndex, UDiaDragDrop::GetDDOInst->m_DraggedItem);
	}

	return Result;
}

