#include "AccessoryEquipButton.h"
#include "EquipmentPanel.h"
#include "Managers/DiabloGameInstance.h"

#define LOCTEXT_NAMESPACE "AccessoryEquipButton"

UAccessoryEquipButton::UAccessoryEquipButton(const FObjectInitializer& objInit): Super(objInit), m_ImgIcon(nullptr),
    m_TextName(nullptr),
    m_TextDesc(nullptr),
    m_TextEquip(nullptr),
    m_TextCost(nullptr), m_BtnEquip(nullptr),
    m_BtnCombine(nullptr)
{
	m_FormatName= LOCTEXT("LevelName","{0}(Lv.{1})");
	m_FormatCost = LOCTEXT("CombineText", "{0}/{1}"); //LOCTEXT("EquipText","Equipped!")
	m_nIndex = -1;
	m_AccessorySpec = nullptr;
}

void UAccessoryEquipButton::UpdateEquipAccessory()
{
	SetLevelNameText(*m_AccessorySpec);
	SetDescPreviewText(*m_AccessorySpec);
	SetCostText(m_AccessorySpec->m_nStackCount);
	SetEquipped(m_AccessorySpec->m_nIsEquipped);
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
	m_BtnEquip->OnClicked.AddDynamic(this,&UAccessoryEquipButton::TryEquip);
	m_ImgIcon->SetBrushFromTexture(data.m_AccessoryData->m_Icon);
	UpdateEquipAccessory();
}

void UAccessoryEquipButton::SetDescPreviewText(const FAccessorySpec& data)
{
	m_TextDesc->SetText(data.m_AccessoryData->GetFormatDescPreview(data.m_nLv));
}

void UAccessoryEquipButton::SetEquipped(bool b)
{
	if(b)
	{
		if(m_ParentEquip->m_nAccessorySelector == 0)
		{
			m_TextEquip->SetText(LOCTEXT("EquipSuccessText1","Equipped-1"));
		}
		else if(m_ParentEquip->m_nAccessorySelector == 1)
		{
			m_TextEquip->SetText(LOCTEXT("EquipSuccessText2","Equipped-2"));
		}
	}
	else
	{
		m_TextEquip->SetText(LOCTEXT("EquipableText","Equip"));
	}
}

void UAccessoryEquipButton::SetCostText(int stack)
{
	FFormatOrderedArguments Args;
	Args.Add(stack);
	Args.Add(m_AccessorySpec->m_LvlUpCost);

	FText tt = FText::Format(m_FormatCost,Args);
	
	m_TextCost->SetText(tt);
}

void UAccessoryEquipButton::TryEquip()
{
	if(m_AccessorySpec)
	{
		if(m_ParentEquip->m_nAccessorySelector ==0)
		{
			UDiabloGameInstance::Get->m_EquipManager->TryEquipAccessory1(m_nIndex);	
		}
		else if(m_ParentEquip->m_nAccessorySelector ==1)
		{
			UDiabloGameInstance::Get->m_EquipManager->TryEquipAccessory2(m_nIndex);
		}
	}
	else
	{
		PRINTF("EqBtn-NoData");
	}
}

#undef LOCTEXT_NAMESPACE
