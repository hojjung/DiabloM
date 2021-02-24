#include "UpgradeButton.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"

UUpgradeButton::UUpgradeButton(const FObjectInitializer& objInit):Super(objInit)
{
	m_FormatName = FText::FromString("{0}(Lv.{1})");
	m_nCurrentLevel=1;
	m_nMaxLevel=1;
}

void UUpgradeButton::UpdateUpgradeable()
{
	const BigInt& CurrentPlayerGold = UDiabloGameInstance::Get->m_GoldManager->GetCurrentGold();

	if(CurrentPlayerGold<m_BigIntCost || m_nCurrentLevel >= m_nMaxLevel)
	{
		m_BtnLvUp->SetIsEnabled(false);
	}
	else
	{
		m_BtnLvUp->SetIsEnabled(true);
	}
	//compare to enable
	//무겁지않을까
}

void UUpgradeButton::SetUpgradeVisual(UTexture2D* icon, const FText& tName, int currentLevel, int maxLevel)
{
	m_NameText = tName;
	m_nMaxLevel=maxLevel;
	m_ImgIcon->SetBrushFromTexture(icon);
	UpdateLevelText(currentLevel);
}

void UUpgradeButton::SetCostText(const BigInt& v)
{
	m_BigIntCost = v;
	FText TextWant = FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(m_BigIntCost,2));
	m_TextCost->SetString(TextWant);
}

void UUpgradeButton::SetDescPreviewText(const FText& tDesc)
{
	m_TextDesc->SetText(tDesc);
}

void UUpgradeButton::UpdateLevelText(int newV)
{
	m_nCurrentLevel= newV;
	FFormatOrderedArguments Args;
	Args.Add(m_NameText);
	Args.Add(m_nCurrentLevel);

	FText tt = FText::Format(m_FormatName,Args);
	
	m_TextName->SetText(tt);
}
