
#include "WeaponEquipButton.h"
#include "WingEquipButton.h"
#include "EquipmentPanel.h"
#include "Managers/DiabloGameInstance.h"

#define LOCTEXT_NAMESPACE "WeaponEquipButton"

UWeaponEquipButton::UWeaponEquipButton(const FObjectInitializer& objInit):Super(objInit)
{
	m_FormatName= LOCTEXT("LevelName","{0}(Lv.{1})");//LOCTEXT("EquipText","Equipped!")
	m_FormatCombine= LOCTEXT("CombineText","Combine:0/5");//LOCTEXT("EquipText","Equipped!")
	m_nIndex=-1;
	m_WeaponSpec = nullptr;
}

void UWeaponEquipButton::SetLevelNameText(const FWeaponSpec& data)
{
	FFormatOrderedArguments Args;

	Args.Add(data.m_EquipData->m_ShowingText);
	Args.Add(data.m_nLv);

	FText tt = FText::Format(m_FormatCombine,Args);

	m_TextName->SetText(tt);
}

void UWeaponEquipButton::UpdateEquipWeapon()
{
	SetLevelNameText(*m_WeaponSpec);
	SetDescPreviewText(*m_WeaponSpec);
	SetCombineText(m_WeaponSpec->m_nStackCount);
	SetEquipped(m_WeaponSpec->m_nIsEquipped);
}

void UWeaponEquipButton::Init(const FWeaponSpec& data, int index)
{
	m_WeaponSpec = &data;
	m_nIndex = index;
	
	m_BtnEquip->OnClicked.AddDynamic(this,&UWeaponEquipButton::TryEquip);
	m_ImgIcon->SetBrushFromTexture(data.m_EquipData->m_Icon);
	UpdateEquipWeapon();
}

void UWeaponEquipButton::SetDescPreviewText(const FWeaponSpec& data)
{
	m_TextDesc->SetText(data.m_EquipData->GetFormatDescPreview(data.m_nLv));
}

void UWeaponEquipButton::SetEquipped(bool b)
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

#undef LOCTEXT_NAMESPACE
