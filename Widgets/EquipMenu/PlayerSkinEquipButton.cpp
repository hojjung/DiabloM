#include "PlayerSkinEquipButton.h"
#include "EquipmentPanel.h"
#include "Managers/DiabloGameInstance.h"

#define LOCTEXT_NAMESPACE "SkinEquipButton"

UPlayerSkinEquipButton::UPlayerSkinEquipButton(const FObjectInitializer& objInit): Super(objInit), m_ImgIcon(nullptr),
	m_TextName(nullptr),
	m_TextDesc(nullptr),
	m_TextEquip(nullptr),
	m_TextCombine(nullptr), m_BtnEquip(nullptr),
	m_BtnCombine(nullptr)
{
	m_FormatCombine = LOCTEXT("CombineText", "Combine:{0}/5"); //LOCTEXT("EquipText","Equipped!")
	m_nIndex = -1;
	m_SkinSpec = nullptr;
}

void UPlayerSkinEquipButton::UpdateEquipSkin()
{
	SetDescPreviewText(*m_SkinSpec);
	SetCombineText(m_SkinSpec->m_nStackCount);
	PRINTF("ASD : %d",m_SkinSpec->m_nIsEquipped);
	SetEquipped(m_SkinSpec->m_nIsEquipped);
}

void UPlayerSkinEquipButton::Init(const FPlayerClassSpec& data, int index)
{
	m_SkinSpec = &data;
	m_TextName->SetText(m_SkinSpec->m_PlayerData->m_ShowingName); 
	m_nIndex = index;
	m_BtnEquip->OnClicked.AddDynamic(this,&UPlayerSkinEquipButton::TryEquip);
	m_BtnCombine->OnClicked.AddDynamic(this,&UPlayerSkinEquipButton::TryCombine);
	m_ImgIcon->SetBrushFromTexture(data.m_PlayerData->m_Icon);
	UpdateEquipSkin();
}

void UPlayerSkinEquipButton::SetDescPreviewText(const FPlayerClassSpec& data)
{
	m_TextDesc->SetText(data.m_PlayerData->GetFormatDescPreview());
}

void UPlayerSkinEquipButton::SetEquipped(bool b)
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

void UPlayerSkinEquipButton::SetCombineText(int stack)
{
	FFormatOrderedArguments Args;
	Args.Add(stack);

	FText tt = FText::Format(m_FormatCombine,Args);
	
	m_TextCombine->SetText(tt);
}

void UPlayerSkinEquipButton::TryEquip()
{
	if(m_SkinSpec)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryEquipSkin(m_nIndex);
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

void UPlayerSkinEquipButton::TryCombine()
{
	if(m_SkinSpec)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryCombineSkin(m_nIndex);
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

#undef LOCTEXT_NAMESPACE
