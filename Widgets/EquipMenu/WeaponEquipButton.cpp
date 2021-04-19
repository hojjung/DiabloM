
#include "WeaponEquipButton.h"
#include "WingEquipButton.h"
#include "EquipmentPanel.h"
#include "Managers/DiabloGameInstance.h"

#define LOCTEXT_NAMESPACE "WeaponEquipButton"


void UWeaponEquipButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_FormatName= LOCTEXT("LevelName","{0}(Lv.{1})");//LOCTEXT("EquipText","Equipped!")
	m_FormatCombine= LOCTEXT("CombineText","합성:{0}/5");//LOCTEXT("EquipText","Equipped!")
	m_nIndex=-1;
	m_WeaponSpec = nullptr;
}
void UWeaponEquipButton::Init(const FWeaponSpec& data, int index)
{
	m_WeaponSpec = &data;
	m_nIndex = index;

	
	m_BtnEquip->OnClicked.AddDynamic(this,&UWeaponEquipButton::TryEquip);
	m_BtnCombine->OnClicked.AddDynamic(this,&UWeaponEquipButton::TryCombine);
	//
	m_BtnLvUp->OnClicked.AddDynamic(this,&UWeaponEquipButton::TryLvUp);
	m_BtnLvUp->OnClicked.AddDynamic(this, &UWeaponEquipButton::ChargeStart);
	m_BtnLvUp->OnHovered.AddDynamic(this, &UWeaponEquipButton::ChargeStart);
	m_BtnLvUp->OnUnhovered.AddDynamic(this, &UWeaponEquipButton::ChargeEnd);
	m_BtnLvUp->OnReleased.AddDynamic(this, &UWeaponEquipButton::ChargeEnd);
	//
	m_ImgIcon->SetBrushFromTexture(data.m_EquipData->m_Icon);
	UpdateEquipWeapon();
}


void UWeaponEquipButton::SetLevelNameText(const FWeaponSpec& data)
{
	FFormatOrderedArguments Args;

	Args.Add(data.m_EquipData->m_ShowingName);
	Args.Add(data.Level);

	FText tt = FText::Format(m_FormatName,Args);

	m_TextName->SetText(tt);
}

void UWeaponEquipButton::SetCostText()
{
	m_ImagTxtCost->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(m_WeaponSpec->m_LvlUpCost,2)));
}

void UWeaponEquipButton::UpdateEquipWeapon()
{
	SetLevelNameText(*m_WeaponSpec);
	SetDescPreviewText(*m_WeaponSpec);
	SetCombineText(m_WeaponSpec->StackCount);
	SetEquipped(m_WeaponSpec->IsEquipped);
	SetCostText();

	m_ImgTierColor->SetBrushTintColor(m_WeaponSpec->m_EquipData->GetTier()->m_TierColor);

	m_BtnEquip->SetIsEnabled(m_WeaponSpec->Level>0);
	m_BtnLvUp->SetIsEnabled(m_WeaponSpec->Level>0);
}


void UWeaponEquipButton::SetDescPreviewText(const FWeaponSpec& data)
{
	m_TextDesc->SetText(data.m_EquipData->GetFormatDescPreview(data.Level));
}

void UWeaponEquipButton::SetEquipped(bool b)
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

void UWeaponEquipButton::SetCombineText(int stack)
{
	FFormatOrderedArguments Args;
	Args.Add(stack);

	FText tt = FText::Format(m_FormatCombine,Args);
	
	m_TextCombine->SetText(tt);
}

void UWeaponEquipButton::TryEquip()
{
	if(m_WeaponSpec)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryEquipWeapon(m_nIndex);
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

void UWeaponEquipButton::TryCombine()
{
	if(m_WeaponSpec)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryCombineWeapon(m_nIndex);
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

void UWeaponEquipButton::TryLvUp()
{
	if(m_WeaponSpec)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryLvUpWeapon(m_nIndex);
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

void UWeaponEquipButton::ChargeStart()
{
	m_bChargeUpgrade = true;
	m_fDeltaCounter = 0.f;
}

void UWeaponEquipButton::ChargeEnd()
{
	m_bChargeUpgrade = false;
	m_fDeltaCounter = 0.f;
}

void UWeaponEquipButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!m_bChargeUpgrade)
	{
		return;
	}

	m_fDeltaCounter += InDeltaTime;

	if (m_fDeltaCounter > 0.1f)
	{
		TryLvUp();
		m_fDeltaCounter = 0.f;
		//upgradeTick
	}
}

#undef LOCTEXT_NAMESPACE
