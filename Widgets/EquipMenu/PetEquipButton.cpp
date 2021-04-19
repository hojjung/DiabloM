
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
	m_FormatCombine= LOCTEXT("CombineText","합성:{0}/5");//LOCTEXT("EquipText","Equipped!")
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
	SetEquipped(m_PetSpec->IsEquipped);
	SetCostText();
	SetCombineText(m_PetSpec->StackCount);
	m_ImgTierColor->SetBrushTintColor(m_PetSpec->m_PetData->GetTier()->m_TierColor);

	m_BtnEquip->SetIsEnabled(m_PetSpec->Level>0);
	m_BtnLvUp->SetIsEnabled(m_PetSpec->Level>0);
}

void UPetEquipButton::SetLevelNameText(const FPetSpec& data)
{
	FFormatOrderedArguments Args;

	Args.Add(data.m_PetData->m_ShowingName);
	Args.Add(data.Level);

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
	//
	m_BtnLvUp->OnClicked.AddDynamic(this,&UPetEquipButton::TryLvUp);
	m_BtnLvUp->OnClicked.AddDynamic(this, &UPetEquipButton::ChargeStart);
	m_BtnLvUp->OnHovered.AddDynamic(this, &UPetEquipButton::ChargeStart);
	m_BtnLvUp->OnUnhovered.AddDynamic(this, &UPetEquipButton::ChargeEnd);
	m_BtnLvUp->OnReleased.AddDynamic(this, &UPetEquipButton::ChargeEnd);
	m_ImgIcon->SetBrushFromTexture(data.m_PetData->m_Icon);
	UpdateEquipPet();
}

void UPetEquipButton::SetDescPreviewText(const FPetSpec& data)
{
	m_TextDesc->SetText(data.m_PetData->GetFormatDescPreview(data.Level));
}

void UPetEquipButton::SetEquipped(bool b)
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

void UPetEquipButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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

void UPetEquipButton::ChargeStart()
{m_bChargeUpgrade = true;
	m_fDeltaCounter = 0.f;
}

void UPetEquipButton::ChargeEnd()
{m_bChargeUpgrade = false;
	m_fDeltaCounter = 0.f;
}

#undef LOCTEXT_NAMESPACE
