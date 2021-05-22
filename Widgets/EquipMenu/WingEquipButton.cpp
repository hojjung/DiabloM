#include "WingEquipButton.h"
#include "EquipmentPanel.h"
#include "Managers/DiabloGameInstance.h"

#define LOCTEXT_NAMESPACE "WingEquipButton"

UWingEquipButton::UWingEquipButton(const FObjectInitializer& objInit):Super(objInit)
{
	m_FormatCombine= LOCTEXT("CombineText","합성:{0}/5");//LOCTEXT("EquipText","Equipped!")
	m_nIndex=-1;
	m_WingSpec = nullptr;
}

void UWingEquipButton::UpdateEquipWing()
{
	SetDescPreviewText(*m_WingSpec);
	SetEquipped(m_WingSpec->IsEquipped);
	m_ImgTierColor->SetBrushTintColor(m_WingSpec->m_WingData->GetTier()->m_TierColor);

	m_BtnEquip->SetIsEnabled(m_WingSpec->IsUnlocked);

	if(m_WingSpec->IsUnlocked)
	{
		m_BtnBuy->SetVisibility(ESlateVisibility::Collapsed);
	}

	int Cost = m_WingSpec->m_WingData->m_nCost; 
	m_ImgTxtCost->SetText(FText::AsNumber(Cost));
}

void UWingEquipButton::Init(const FWingSpec& data, int index)
{
	m_WingSpec = &data;
	m_TextName->SetText(m_WingSpec->m_WingData->m_ShowingName); 
	m_nIndex = index;
	m_BtnEquip->OnClicked.AddDynamic(this,&UWingEquipButton::TryEquip);
	m_BtnBuy->OnClicked.AddDynamic(this,&UWingEquipButton::TryBuy);
	m_ImgIcon->SetBrushFromTexture(data.m_WingData->m_Icon);
	UpdateEquipWing();
}

void UWingEquipButton::SetDescPreviewText(const FWingSpec& data)
{
	m_TextDesc->SetText(data.m_WingData->GetFormatDescPreview());
}

void UWingEquipButton::SetEquipped(bool b)
{
	if(b)
	{
		m_TextEquip->SetText(LOCTEXT("EquipSuccessText","장착 됨!"));
	}
	else
	{
		m_TextEquip->SetText(LOCTEXT("EquipableText","장착"));
	}
}

void UWingEquipButton::TryEquip()
{
	if(m_WingSpec)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryEquipWing(m_nIndex);
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

void UWingEquipButton::TryBuy()
{
	if(m_WingSpec&&!m_WingSpec->IsUnlocked)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryUnlockWing(m_nIndex);
	}
}

#undef LOCTEXT_NAMESPACE
