
#include "PetEquipButton.h"
#include "EquipmentPanel.h"
#include "Managers/DiabloGameInstance.h"

#define LOCTEXT_NAMESPACE "PetEquipButton"

UPetEquipButton::UPetEquipButton(const FObjectInitializer& objInit): Super(objInit), m_ImgIcon(nullptr),
    m_TextName(nullptr),
    m_TextDesc(nullptr),
    m_TextEquip(nullptr),
    m_TextCombine(nullptr), m_BtnEquip(nullptr),
    m_BtnCombine(nullptr)
{
	m_FormatName= LOCTEXT("LevelName","{0}(Lv.{1})");//LOCTEXT("EquipText","Equipped!")m_FormatCombine= LOCTEXT("CombineText","Combine:0/5");//LOCTEXT("EquipText","Equipped!")
	m_FormatCombine= LOCTEXT("CombineText","Combine:{0}/5");//LOCTEXT("EquipText","Equipped!")
	m_nIndex = -1;
	m_PetSpec = nullptr;
}

void UPetEquipButton::SetCostText()
{
	m_ImagTxtCost->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(m_PetSpec->m_LvlUpCost,2)));
}

void UPetEquipButton::UpdateEquipPet()
{
	SetLevelNameText(*m_PetSpec);
	SetDescPreviewText(*m_PetSpec);
	SetEquipped(m_PetSpec->m_nIsEquipped);
	SetCostText();
	SetCombineText(m_PetSpec->m_nStackCount);
	m_ImgTierColor->SetBrushTintColor(m_PetSpec->m_PetData->GetTier()->m_TierColor);
}

void UPetEquipButton::SetLevelNameText(const FPetSpec& data)
{
	FFormatOrderedArguments Args;

	Args.Add(data.m_PetData->m_ShowingName);
	Args.Add(data.m_nLv);

	FText tt = FText::Format(m_FormatName,Args);

	m_TextName->SetText(tt);
}

void UPetEquipButton::Init(const FPetSpec& data, int index)
{
	m_PetSpec = &data;
	m_TextName->SetText(m_PetSpec->m_PetData->m_ShowingName); 
	m_nIndex = index;
	m_BtnEquip->OnClicked.AddDynamic(this,&UPetEquipButton::TryEquip);
	m_BtnCombine->OnClicked.AddDynamic(this,&UPetEquipButton::TryCombine);
	m_BtnLvUp->OnClicked.AddDynamic(this,&UPetEquipButton::TryLvUp);
	m_ImgIcon->SetBrushFromTexture(data.m_PetData->m_Icon);
	UpdateEquipPet();
}

void UPetEquipButton::SetDescPreviewText(const FPetSpec& data)
{
	m_TextDesc->SetText(data.m_PetData->GetFormatDescPreview(data.m_nLv));
}

void UPetEquipButton::SetEquipped(bool b)
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

void UPetEquipButton::SetCombineText(int stack)
{
	FFormatOrderedArguments Args;
	Args.Add(stack);

	FText tt = FText::Format(m_FormatCombine,Args);
	
	m_TextCombine->SetText(tt);
}


void UPetEquipButton::TryEquip()
{
	if(m_PetSpec)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryEquipPet(m_nIndex);
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

void UPetEquipButton::TryCombine()
{
	if(m_PetSpec)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryCombinePet(m_nIndex);
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

void UPetEquipButton::TryLvUp()
{
	if(m_PetSpec)
	{
		UDiabloGameInstance::Get->m_EquipManager->TryLvUpPet(m_nIndex);
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

#undef LOCTEXT_NAMESPACE
