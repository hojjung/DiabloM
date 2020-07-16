#include "ItemPopupInfo.h"
#include "Components/CanvasPanelSlot.h"
#include "WidgetLayoutLibrary.h"
#include "Components/VerticalBoxSlot.h"
#include "Animation/UMGSequencePlayer.h"


void UItemPopupInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_AryOptions.Reset(13);
	m_AryOptions.Emplace(m_SubOption1);
	m_AryOptions.Emplace(m_SubOption2);
	m_AryOptions.Emplace(m_SubOption3);
	m_AryOptions.Emplace(m_SubOption4);
	m_AryOptions.Emplace(m_SubOption5);
	m_AryOptions.Emplace(m_SubOption6);
	m_AryOptions.Emplace(m_SubOption7);
	m_AryOptions.Emplace(m_SubOption8);
	m_AryOptions.Emplace(m_SubOption9);
	m_AryOptions.Emplace(m_SubOption10);
	m_AryOptions.Emplace(m_SubOption11);

	FString ItemTypeName = "EItemType";

	m_ItemTypeString = FindObject<UEnum>(ANY_PACKAGE, *ItemTypeName);

	m_InitSize = UWidgetLayoutLibrary::SlotAsCanvasSlot(this)->GetSize();
	PRINTF("InitSize:%s", *m_InitSize.ToString());

	//NativeOnMouseButtonDoubleClick()
}

FReply UItemPopupInfo::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	auto Rep=Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	PlayHideInfoAnim();
	
	return Rep;
	
}
//NativeOnInitialized
void UItemPopupInfo::NativePreConstruct()
{
	//m_AryOptions.Emplace(m_MainOptionAttack);
	//m_AryOptions.Emplace(m_MainOptionDefense);
	Super::NativePreConstruct();
}

FText UItemPopupInfo::GetItemTypeTxt(EItemType typeV) const
{
	return  FText::FromString( m_ItemTypeString->GetNameStringByIndex(static_cast<uint8>(typeV)));
}

void UItemPopupInfo::SetIcon(const FItemInstance & itemInst)
{
	FSlateBrush BrushWant;

	BrushWant.SetImageSize(FVector2D(64.f,64.f));
	BrushWant.SetResourceObject(itemInst.m_ItemData->m_ItemIcon);

	m_ImageItemVisualIcon->SetBrush(BrushWant);
}

void UItemPopupInfo::SetColorTier(const FItemInstance & itemInst)
{
	const auto ColorW = itemInst.m_ItemData->GetItemTier().m_TierColor;
	m_ImageItemTierColorSmall->SetColorAndOpacity(ColorW);
	m_ImageItemTierColorLarge->SetColorAndOpacity(ColorW);
	m_TextItemName->SetColorAndOpacity(ColorW);
	m_TextItemTierAndType->SetColorAndOpacity(ColorW);

}

void UItemPopupInfo::HideAllSubOptions()
{
	for (auto* Options : m_AryOptions)
	{
		Options->SetString(FText());
		Options->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UItemPopupInfo::HideFlavorText()
{
	m_TextFlavor->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemPopupInfo::SetItemText(const FItemInstance & itemInst)
{
	m_TextItemName->SetText(itemInst.m_ItemData->m_ShowingName);

	FText  ItemNameT = itemInst.m_ItemData->GetItemTier().m_ShowingName;
	
	FText ItemTypeT= GetItemTypeTxt(itemInst.m_ItemData->m_ItemType);

	FFormatOrderedArguments Args;
	Args.Add(ItemNameT);
	Args.Add(ItemTypeT);

	FTextFormat FormatT = FText::FromString("{0} {1}");

	m_TextItemTierAndType->SetText(FText::Format(FormatT, Args));
}

float UItemPopupInfo::SetFlavorText(const FItemInstance & itemInst)
{
	//m_TextFlavor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_TextFlavor->SetText(itemInst.m_ItemData->m_FlavorText);
	m_TextFlavor->ForceLayoutPrepass();
	return m_TextFlavor->GetDesiredSize().Y;
}

float UItemPopupInfo::SetOptionTexts(const FItemInstance & itemInst)
{
	float OptionSizeY = 0.f;

	int OptionCount = itemInst.m_AryOptions.Num();

	int i = 0;

	while (i<OptionCount)
	{
		m_AryOptions[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_AryOptions[i]->SetString(itemInst.m_ItemData->GetOption(i).GetOptionFormat(itemInst.m_AryOptions[i].m_fValue));
		m_AryOptions[i]->ForceLayoutPrepass();
		OptionSizeY+= m_AryOptions[i]->GetDesiredSize().Y;
		i++;
	}

	return OptionSizeY;
}

void UItemPopupInfo::ShowInfoPanel(const FItemInstance & itemInst)
{
	PlayAnimation(m_FadeAnimation);
	HideAllSubOptions();

	UWidgetLayoutLibrary::SlotAsCanvasSlot(this)->SetSize(m_InitSize);

	FVector2D NewSize = m_InitSize;

	SetIcon(itemInst);
	SetColorTier(itemInst);
	SetItemText(itemInst);

	NewSize.Y += SetOptionTexts(itemInst);
	NewSize.Y += SetFlavorText(itemInst);
	//
	UWidgetLayoutLibrary::SlotAsCanvasSlot(this)->SetSize(NewSize);
}

void UItemPopupInfo::PlayHideInfoAnim()
{
	PlayAnimationReverse(m_FadeAnimation);	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UItemPopupInfo::HideInfoPanel, m_FadeAnimation->GetEndTime(), false);
}

void UItemPopupInfo::HideInfoPanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

