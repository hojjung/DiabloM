#include "SkillUpgradeButton.h"
#include "UpgradeButton.h"
#include "Datas/PlayerUpgradeData.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"
#include "Managers/DiabloGameInstance.h"

#define LOCTEXT_NAMESPACE "SkillUpgradeButton"

USkillUpgradeButton* USkillUpgradeButton::CurrentSelected = nullptr;

USkillUpgradeButton::USkillUpgradeButton(const FObjectInitializer& objInit): Super(objInit)
{
	m_FormatName = FText::FromString("{0}(Lv.{1})");
	m_nCurrentLevel = 1;
	m_nMaxLevel = 1;
	m_fDeltaCounter = 0.f;
	m_bChargeUpgrade = false;
	CurrentSelected = nullptr;
}

void USkillUpgradeButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_BtnLvUp->OnClicked.AddDynamic(this, &USkillUpgradeButton::OnClickButton);
	m_BtnLvUp->OnClicked.AddDynamic(this, &USkillUpgradeButton::ChargeStart);
	m_BtnLvUp->OnHovered.AddDynamic(this, &USkillUpgradeButton::ChargeStart);
	m_BtnLvUp->OnUnhovered.AddDynamic(this, &USkillUpgradeButton::ChargeEnd);
	m_BtnLvUp->OnReleased.AddDynamic(this, &USkillUpgradeButton::ChargeEnd);
	m_BtnStyle = m_BtnEquip->WidgetStyle;
	m_BtnEquip->OnClicked.AddDynamic(this,&USkillUpgradeButton::OnClick);
}


void USkillUpgradeButton::UpdateUpgradeable()
{
	const BigInt& CurrentPlayerGold = UDiabloGameInstance::Get->m_GoldManager->GetCurrentGold();

	if (CurrentPlayerGold < m_BigIntCost || m_nCurrentLevel >= m_nMaxLevel)
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

void USkillUpgradeButton::Init(FSkillSpec& data)
{
	m_EquippedSkillSpec = &(data);
	m_NameText = m_EquippedSkillSpec->m_SkillData->m_SkillShowName;
	m_nMaxLevel = m_EquippedSkillSpec->GetMaxLv();
	m_ImgIcon->SetBrushFromTexture(m_EquippedSkillSpec->m_SkillData->m_SkillIcon);
	UpdateSkillButton();
}

void USkillUpgradeButton::SetCostText(const BigInt& v)
{
	m_BigIntCost = v;
	FText TextWant = FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(m_BigIntCost, 2));
	m_TextCost->SetText(TextWant);
}


void USkillUpgradeButton::SetDescPreviewText(const FSkillSpec& data)
{
	m_TextDesc->SetText(data.m_SkillData->GetFormatDescPreview(data.m_nLv));
}

void USkillUpgradeButton::UpdateSkillButton()
{
	m_nCurrentLevel = m_EquippedSkillSpec->m_nLv;
	FFormatOrderedArguments Args;
	Args.Add(m_NameText);
	Args.Add(m_nCurrentLevel);

	FText tt = FText::Format(m_FormatName, Args);

	m_TextName->SetText(tt);

	SetDescPreviewText(*m_EquippedSkillSpec);
	SetCostText(m_EquippedSkillSpec->m_Cost);
	UpdateUpgradeable();
}

void USkillUpgradeButton::OnClickButton()
{
	m_OnCharge.ExecuteIfBound();
}

void USkillUpgradeButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!m_bChargeUpgrade)
	{
		return;
	}

	m_fDeltaCounter += InDeltaTime;

	if (m_fDeltaCounter > 0.1f)
	{
		m_fDeltaCounter = 0.f;
		OnClickButton();
		//upgradeTick
	}
}


void USkillUpgradeButton::ChargeStart()
{
	m_bChargeUpgrade = true;
	m_fDeltaCounter = 0.f;
}

void USkillUpgradeButton::ChargeEnd()
{
	m_bChargeUpgrade = false;
	m_fDeltaCounter = 0.f;
}

void USkillUpgradeButton::OnClick()
{
	if(m_EquippedSkillSpec->m_nLv<1)
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("OnClickFail", "배우지 않은 스킬입니다."));
		return;
	}
	m_OnClicked.Broadcast(this);
}

void USkillUpgradeButton::SetHoverImage()
{
	auto& Style = m_BtnEquip->WidgetStyle;
	
	m_CachedNormal = Style.Normal; 
	
	Style.Normal = Style.Hovered;

	m_BtnEquip->SetStyle(Style);
}

void USkillUpgradeButton::SetNormalImage()
{
	m_BtnEquip->WidgetStyle.Normal = m_CachedNormal;
	
	m_BtnEquip->SetStyle(m_BtnEquip->WidgetStyle);
}

#undef LOCTEXT_NAMESPACE