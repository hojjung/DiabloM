#include "AccessoryEquipButton.h"
#include "EquipmentPanel.h"
#include "Managers/DiabloGameInstance.h"

#define LOCTEXT_NAMESPACE "AccessoryEquipButton"

UAccessoryEquipButton::UAccessoryEquipButton(const FObjectInitializer& objInit): Super(objInit), m_ImgIcon(nullptr),
    m_TextName(nullptr),
    m_TextDesc(nullptr),
    m_TextCost(nullptr), 
    m_BtnCombine(nullptr)
{
	m_FormatName= LOCTEXT("LevelName","{0}(Lv.{1})");
	m_FormatCost = LOCTEXT("CombineText", "합성 업{0}/{1}"); //LOCTEXT("EquipText","Equipped!")
	m_nIndex = -1;
	m_AccessorySpec = nullptr;
}

void UAccessoryEquipButton::UpdateEquipAccessory()
{
	SetLevelNameText(*m_AccessorySpec);
	SetDescPreviewText(*m_AccessorySpec);
	SetCostText(m_AccessorySpec->m_nStackCount);
	//SetEquipped(m_AccessorySpec->m_nIsEquipped);

	m_ImgTierColor->SetBrushTintColor(m_AccessorySpec->m_AccessoryData->GetTier()->m_TierColor);
}

void UAccessoryEquipButton::SetLevelNameText(const FAccessorySpec& data)
{
	FFormatOrderedArguments Args;

	Args.Add(data.m_AccessoryData->m_ShowingName);
	Args.Add(data.m_nLv);

	FText tt = FText::Format(m_FormatName,Args);

	m_TextName->SetText(tt);
}

void UAccessoryEquipButton::Init(const FAccessorySpec& data, UEquipmentPanel* equipPanel,int index)
{
	m_ParentEquip = equipPanel;
	m_AccessorySpec = &data;
	m_TextName->SetText(m_AccessorySpec->m_AccessoryData->m_ShowingName); 
	m_nIndex = index;
	m_BtnCombine->OnClicked.AddDynamic(this,&UAccessoryEquipButton::TryCombineLvUp);
	m_ImgIcon->SetBrushFromTexture(data.m_AccessoryData->m_Icon);
	UpdateEquipAccessory();

	
}

void UAccessoryEquipButton::SetDescPreviewText(const FAccessorySpec& data)
{
	m_TextDesc->SetText(data.m_AccessoryData->GetFormatDescPreview(data.m_nLv));
}

void UAccessoryEquipButton::SetCostText(int stack)
{
	FFormatOrderedArguments Args;
	Args.Add(stack);
	Args.Add(m_AccessorySpec->m_LvlUpCost);

	FText tt = FText::Format(m_FormatCost,Args);
	
	m_TextCost->SetText(tt);
}


void UAccessoryEquipButton::TryCombineLvUp()
{
	if(m_AccessorySpec)
	{
		//if(m_ParentEquip->m_nAccessorySelector ==0)
		//{
		//	UDiabloGameInstance::Get->m_EquipManager->TryCombineLevelUpAccessory(m_nIndex);	
		//}
		//else if(m_ParentEquip->m_nAccessorySelector ==1)
		{
			UDiabloGameInstance::Get->m_EquipManager->TryCombineLevelUpAccessory(m_nIndex);
		}
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

#undef LOCTEXT_NAMESPACE
