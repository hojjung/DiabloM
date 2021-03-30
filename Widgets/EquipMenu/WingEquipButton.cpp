#include "WingEquipButton.h"
#include "EquipmentPanel.h"
#include "Managers/DiabloGameInstance.h"

#define LOCTEXT_NAMESPACE "WingEquipButton"

UWingEquipButton::UWingEquipButton(const FObjectInitializer& objInit):Super(objInit)
{
	m_FormatCombine= LOCTEXT("CombineText","Combine:{0}/5");//LOCTEXT("EquipText","Equipped!")
	m_nIndex=-1;
	m_WingSpec = nullptr;
}

void UWingEquipButton::UpdateEquipWing()
{
	SetDescPreviewText(*m_WingSpec);
	SetCombineText(m_WingSpec->m_nStackCount);
	SetEquipped(m_WingSpec->m_nIsEquipped);
	m_ImgTierColor->SetBrushTintColor(m_WingSpec->m_WingData->GetTier()->m_TierColor);

	m_BtnEquip->SetIsEnabled(m_WingSpec->m_nIsUnlocked>0);
}

void UWingEquipButton::Init(const FWingSpec& data, int index)
{
	m_WingSpec = &data;
	m_TextName->SetText(m_WingSpec->m_WingData->m_ShowingName); 
	m_nIndex = index;
	m_BtnEquip->OnClicked.AddDynamic(this,&UWingEquipButton::TryEquip);
	m_BtnCombine->OnClicked.AddDynamic(this,&UWingEquipButton::TryCombine);
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
		m_TextEquip->SetText(LOCTEXT("EquipSuccessText","Equipped!"));
	}
	else
	{
		m_TextEquip->SetText(LOCTEXT("EquipableText","Equip"));
	}
}

void UWingEquipButton::SetCombineText(int stack)
{
	FFormatOrderedArguments Args;
	Args.Add(stack);

	FText tt = FText::Format(m_FormatCombine,Args);
	
	m_TextCombine->SetText(tt);
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

void UWingEquipButton::TryCombine()
{
	if(m_WingSpec)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryCombineWing(m_nIndex);
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

#undef LOCTEXT_NAMESPACE
