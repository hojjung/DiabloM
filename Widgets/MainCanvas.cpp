// My First Hack n Slash


#include "MainCanvas.h"

#include "Managers/DiabloGameInstance.h"

void UMainCanvas::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//
	m_BtnQuest->OnClicked.AddDynamic(this,&UMainCanvas::SetActiveQuestPanel);
	m_BtnUpgrade->OnClicked.AddDynamic(this,&UMainCanvas::SetActiveUpgradePanel);
	m_BtnEquipment->OnClicked.AddDynamic(this,&UMainCanvas::SetActiveEquipmentPanel);
	m_BtnGacha->OnClicked.AddDynamic(this,&UMainCanvas::SetActiveGachaPanel);
	m_BtnShop->OnClicked.AddDynamic(this,&UMainCanvas::SetActiveShopPanel);
	m_BtnMenu->OnClicked.AddDynamic(this,&UMainCanvas::SetActiveMenuPanel);

	m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);

	UDiabloGameInstance::Get->m_GoldManager->m_OnGoldChanged.AddUObject(this,&UMainCanvas::UpdateGoldUI);

	UpdateGoldUI();
}

void UMainCanvas::SetActiveQuestPanel()
{
	if(m_PanelQuest->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelQuest->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	}
	m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);	
}

void UMainCanvas::SetActiveUpgradePanel()
{
	m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	if(m_PanelUpgrade->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelUpgrade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	}
	m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainCanvas::SetActiveEquipmentPanel()
{
	m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	if(m_PanelEquipment->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelEquipment->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	}
	m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainCanvas::SetActiveGachaPanel()
{
	m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	if(m_PanelGacha->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelGacha->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	}
	m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainCanvas::SetActiveShopPanel()
{
	m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	if(m_PanelShop->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelShop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	}
	m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainCanvas::SetActiveMenuPanel()
{
	m_PanelQuest->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelUpgrade->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelEquipment->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelGacha->SetVisibility(ESlateVisibility::Collapsed);
	m_PanelShop->SetVisibility(ESlateVisibility::Collapsed);
	if(m_PanelMenu->Visibility != ESlateVisibility::SelfHitTestInvisible)
	{
		m_PanelMenu->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		m_PanelMenu->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainCanvas::UpdateGoldUI()
{
	m_TextGold->SetText(FText::FromString(UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(UDiabloGameInstance::Get->m_GoldManager->GetCurrentGold(),2)));
}