#include "ItemPopupInfo.h"


void UItemPopupInfo::NativePreConstruct()
{
	Super::NativePreConstruct();
	m_AryOptions.Reset(13);
	//m_AryOptions.Emplace(m_MainOptionAttack);
	//m_AryOptions.Emplace(m_MainOptionDefense);
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
		Options->SetVisibility(ESlateVisibility::Hidden);
	}
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

void UItemPopupInfo::SetFlavorText(const FItemInstance & itemInst)
{
	m_TextFlavor->SetText(itemInst.m_ItemData->m_FlavorText);
}

void UItemPopupInfo::SetInfoPanel(const FItemInstance & itemInst)
{
	HideAllSubOptions();
	//이후로도 장비템이냐 장비템이 아니냐로 숨겨주고 보여주고 조절해야함
	//또한 현재 장착된 아이템보다 얼마나 좋은지
	SetIcon(itemInst);
	SetColorTier(itemInst);
	SetItemText(itemInst);
	SetFlavorText(itemInst);
}
