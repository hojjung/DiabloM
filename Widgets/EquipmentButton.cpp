#include "EquipmentButton.h"

#include "Managers/DiabloGameInstance.h"

#define LOCTEXT_NAMESPACE "EquipButton"

UEquipmentButton::UEquipmentButton(const FObjectInitializer& objInit):Super(objInit)
{
	m_FormatName = FText::FromString("{0}(Lv.{1})");
	m_FormatCombine= LOCTEXT("CombineText","Combine:0/5");//LOCTEXT("EquipText","Equipped!")
	m_nCurrentLevel=1;
	m_nMaxLevel=1;
	m_PlSkinData = nullptr;
	m_EquipData = nullptr;
}

void UEquipmentButton::UpdateUpgradeable()
{
	const BigInt& CurrentPlayerGold = UDiabloGameInstance::Get->m_GoldManager->GetCurrentGold();

	if(CurrentPlayerGold<m_BigIntCost || m_nCurrentLevel >= m_nMaxLevel)
	{
		//m_BtnLvUp->SetIsEnabled(false);
	}
	else
	{
		m_BtnLvUp->SetIsEnabled(true);
	}
}

void UEquipmentButton::SetCostText(const BigInt& v)
{
	m_BigIntCost = v;
	FText TextWant = FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(m_BigIntCost,2));
	m_TextCost->SetString(TextWant);
}

void UEquipmentButton::SetDescPreviewText(const FPlayerClassSpec& data)
{
	m_TextDesc->SetText(data.m_PlayerData->GetFormatDescPreview(data.m_nLv));
}

void UEquipmentButton::SetDescPreviewText(const FEquipmentSpec& data)
{
	m_TextDesc->SetText(data.m_EquipData->GetFormatDescPreview(data.m_nLv));
}

void UEquipmentButton::SetUpgradeVisual(const FEquipmentSpec& data)
{
	m_EquipData = &data;
	m_BtnEquip->OnClicked.AddDynamic(this,&UEquipmentButton::TryEquip);
	m_NameText = data.m_EquipData->m_UpgradeShowName;
	m_nMaxLevel = data.GetMaxLv();
	m_ImgIcon->SetBrushFromTexture(data.m_EquipData->m_UpgradeIcon);
	UpdateLevelText(data);
}

void UEquipmentButton::SetUpgradeVisual(const FPlayerClassSpec& data)
{
	m_PlSkinData = &data;
	m_BtnEquip->OnClicked.AddDynamic(this,&UEquipmentButton::TryEquip);
	m_NameText = data.m_PlayerData->m_UpgradeShowName;
	m_nMaxLevel = data.GetMaxLv();
	m_ImgIcon->SetBrushFromTexture(data.m_PlayerData->m_UpgradeIcon);
	UpdateLevelText(data);
}

void UEquipmentButton::UpdateLevelText(const FEquipmentSpec& data)
{
	m_nCurrentLevel= data.m_nLv;
	FFormatOrderedArguments Args;
	Args.Add(m_NameText);
	Args.Add(m_nCurrentLevel);

	FText tt = FText::Format(m_FormatName,Args);
	
	m_TextName->SetText(tt);
	
	SetDescPreviewText(data);
	SetCostText(data.m_LvlUpCost);
	UpdateUpgradeable();
	SetCombineText(data.m_nStackCount);
}

void UEquipmentButton::UpdateLevelText(const FPlayerClassSpec& data)
{
	m_nCurrentLevel= data.m_nLv;
	FFormatOrderedArguments Args;
	Args.Add(m_NameText);
	Args.Add(m_nCurrentLevel);

	FText tt = FText::Format(m_FormatName,Args);
	
	m_TextName->SetText(tt);
	
	SetDescPreviewText(data);
	SetCostText(data.m_LvlUpCost);
	UpdateUpgradeable();
	SetCombineText(data.m_nStackCount);
}

void UEquipmentButton::SetEquipped(bool b)
{
	if(b)
	{
		m_TextEquip->SetText(LOCTEXT("EquipText","Equipped!"));
	}
	else
	{
		m_TextEquip->SetText(LOCTEXT("EquipText","Equip"));
	}
}

void UEquipmentButton::SetCombineText(int stack)
{
	FFormatOrderedArguments Args;
	Args.Add(stack);

	FText tt = FText::Format(m_FormatCombine,Args);
	
	m_TextCombine->SetText(tt);
}

void UEquipmentButton::TryEquip()
{
	if(m_PlSkinData)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryEquipSkin(m_PlSkinData);
	}
	else if(m_EquipData)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryEquipEquipment(m_EquipData);
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

#undef LOCTEXT_NAMESPACE
