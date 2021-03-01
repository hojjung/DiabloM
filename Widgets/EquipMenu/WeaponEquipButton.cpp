
#include "WeaponEquipButton.h"
#include "WingEquipButton.h"
#include "EquipmentPanel.h"
#include "Managers/DiabloGameInstance.h"

#define LOCTEXT_NAMESPACE "WeaponEquipButton"


void UWeaponEquipButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_FormatName= LOCTEXT("LevelName","{0}(Lv.{1})");//LOCTEXT("EquipText","Equipped!")
	m_FormatCombine= LOCTEXT("CombineText","Combine:{0}/5");//LOCTEXT("EquipText","Equipped!")
	m_nIndex=-1;
	m_WeaponSpec = nullptr;
}
void UWeaponEquipButton::Init(const FWeaponSpec& data, int index)
{
	m_WeaponSpec = &data;
	m_nIndex = index;
	
	m_BtnEquip->OnClicked.AddDynamic(this,&UWeaponEquipButton::TryEquip);
	m_BtnLvUp->OnClicked.AddDynamic(this,&UWeaponEquipButton::TryLvUp);
	m_BtnCombine->OnClicked.AddDynamic(this,&UWeaponEquipButton::TryCombine);
	m_ImgIcon->SetBrushFromTexture(data.m_EquipData->m_Icon);
	UpdateEquipWeapon();
}


void UWeaponEquipButton::SetLevelNameText(const FWeaponSpec& data)
{
	FFormatOrderedArguments Args;

	Args.Add(data.m_EquipData->m_ShowingText);
	Args.Add(data.m_nLv);

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
	SetCombineText(m_WeaponSpec->m_nStackCount);
	SetEquipped(m_WeaponSpec->m_nIsEquipped);
	SetCostText();
}


void UWeaponEquipButton::SetDescPreviewText(const FWeaponSpec& data)
{
	m_TextDesc->SetText(data.m_EquipData->GetFormatDescPreview(data.m_nLv));
}

void UWeaponEquipButton::SetEquipped(bool b)
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

#undef LOCTEXT_NAMESPACE
