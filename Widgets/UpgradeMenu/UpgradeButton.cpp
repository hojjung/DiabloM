#include "UpgradeButton.h"

#include "Datas/PlayerUpgradeData.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"

UUpgradeButton::UUpgradeButton(const FObjectInitializer& objInit):Super(objInit)
{
	m_FormatName = FText::FromString("{0}(Lv.{1})");
	m_nCurrentLevel=1;
	m_nMaxLevel=1;
	m_fDeltaCounter=0.f;
	m_bChargeUpgrade=false;
}

void UUpgradeButton::UpdateUpgradeable()
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
	//compare to enable
	//무겁지않을까
}

void UUpgradeButton::SetUpgradeVisual(const FUpgradeSpec& data)
{
	m_NameText = data.m_UpgradeData->m_UpgradeShowName;
	m_nMaxLevel = data.GetMaxLv();
	m_ImgIcon->SetBrushFromTexture(data.m_UpgradeData->m_UpgradeIcon);
	UpdateLevelText(data);
}

void UUpgradeButton::SetCostText(const BigInt& v)
{
	m_BigIntCost = v;
	FText TextWant = FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(m_BigIntCost,2));
	m_TextCost->SetText(TextWant);
}

void UUpgradeButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradeButton::ChargeStart);
	m_BtnLvUp->OnUnhovered.AddDynamic(this,&UUpgradeButton::ChargeEnd);
	m_BtnLvUp->OnReleased.AddDynamic(this,&UUpgradeButton::ChargeEnd);
}

void UUpgradeButton::SetDescPreviewText(const FUpgradeSpec& data)
{
	m_TextDesc->SetText(data.m_UpgradeData->GetFormatDescPreview(data.m_nLv));
}

void UUpgradeButton::UpdateLevelText(const FUpgradeSpec& data)
{
	m_nCurrentLevel= data.m_nLv;
	FFormatOrderedArguments Args;
	Args.Add(m_NameText);
	Args.Add(m_nCurrentLevel);

	FText tt = FText::Format(m_FormatName,Args);
	
	m_TextName->SetText(tt);
	
	SetDescPreviewText(data);
	SetCostText(data.m_Cost);
	UpdateUpgradeable();
}

void UUpgradeButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!m_bChargeUpgrade)
	{
		return;
	}

	m_fDeltaCounter+=InDeltaTime;

	if(m_fDeltaCounter>0.15f)
	{
		m_fDeltaCounter= 0.f;
		m_OnCharge.ExecuteIfBound();
		//upgradeTick
	}
}

void UUpgradeButton::ChargeStart()
{
	m_bChargeUpgrade = true;
	m_fDeltaCounter= 0.f;
}

void UUpgradeButton::ChargeEnd()
{
	m_bChargeUpgrade = false;
	m_fDeltaCounter= 0.f;
}
