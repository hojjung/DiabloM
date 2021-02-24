// My First Hack n Slash


#include "UpgradePanel.h"


#include "Datas/PlayerUpgradeData.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/PlayfabManager.h"

UUpgradePanel::UUpgradePanel(const FObjectInitializer& objInit):Super(objInit)
{
	
}

void UUpgradePanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_PlUpgrade = UDiabloGameInstance::Get->m_PlayerUpgradeManager;
	//
	SetPanel1();
	//
	m_BtnDefaultStat->OnClicked.AddDynamic(this,&UUpgradePanel::SetPanel1);
	m_BtnSkill01->OnClicked.AddDynamic(this,&UUpgradePanel::SetPanel2);
	m_BtnClose->OnClicked.AddDynamic(this,&UUpgradePanel::ClosePanel);
	//
	m_UpgradeAtkDmg01->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeAtkDmg01);
	m_PlUpgrade->m_OnUpgradeChanged.AddUObject(this,&UUpgradePanel::OnUpgradeAtkDmg01);
	
	m_UpgradeAtkDmg01->SetUpgradeVisual(m_PlUpgrade->m_PlAtkDmg01Upgrade->m_UpgradeIcon,
		m_PlUpgrade->m_PlAtkDmg01Upgrade->m_UpgradeShowName,
		m_PlUpgrade->m_nPlAtkDmg01Lv,
		m_PlUpgrade->m_PlAtkDmg01Upgrade->m_nMaxLevel);
}

void UUpgradePanel::SetPanel1()
{
	m_SwitcherUpgradePanel->SetActiveWidgetIndex(0);
}

void UUpgradePanel::SetPanel2()
{
	m_SwitcherUpgradePanel->SetActiveWidgetIndex(1);
}

void UUpgradePanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUpgradePanel::UpgradeAtkDmg01()
{
	m_PlUpgrade->UpgradeAtkDmg01();
}

void UUpgradePanel::OnUpgradeAtkDmg01()
{
	m_UpgradeAtkDmg01->UpdateLevelText(m_PlUpgrade->m_nPlAtkDmg01Lv);
}
