#include "UpgradePanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Datas/PlayerUpgradeData.h"
#include "Managers/PlayfabManager.h"

UUpgradePanel::UUpgradePanel(const FObjectInitializer& objInit):Super(objInit)
{
	
}

void UUpgradePanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_PlUpgrade = UDiabloGameInstance::Get->m_PlayerUpgradeManager;
	m_PlUpgrade->m_OnUpgradeChanged.AddUObject(this,&UUpgradePanel::OnUpgradeChanged);
	//
	SetPanel1();
	//toggle tab
	m_BtnDefaultStat->OnClicked.AddDynamic(this,&UUpgradePanel::SetPanel1);
	m_BtnSkill01->OnClicked.AddDynamic(this,&UUpgradePanel::SetPanel2);
	m_BtnClose->OnClicked.AddDynamic(this,&UUpgradePanel::ClosePanel);
	//upgradeAtkDmg01
	m_UpgradeAtkDmg01->m_BtnLvUp->OnPressed.AddDynamic(this,&UUpgradePanel::UpgradeAtkDmg01);
	m_UpgradeAtkDmg01->SetUpgradeVisual(m_PlUpgrade->m_UpgradeAtkDmg01);
	//upgradeAtkCri01
	m_UpgradeAtkCri01->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeAtkCri01);
	m_UpgradeAtkCri01->SetUpgradeVisual(m_PlUpgrade->m_UpgradeAtkCri01);
	//upgradeAtkCDmg01
	m_UpgradeAtkCDmg01->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeAtkCDmg01);
	m_UpgradeAtkCDmg01->SetUpgradeVisual(m_PlUpgrade->m_UpgradeAtkCDmg01);
	//upgradeAtkCDmg01
	m_UpgradeSkill01->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeSkill01);
	m_UpgradeSkill01->SetUpgradeVisual(m_PlUpgrade->m_UpgradeSkill01);
	//upgradeAtkCDmg01
	m_UpgradeSkill02->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeSkill02);
	m_UpgradeSkill02->SetUpgradeVisual(m_PlUpgrade->m_UpgradeSkill02);
	//upgradeAtkCDmg01
	m_UpgradeSkill03->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeSkill03);
	m_UpgradeSkill03->SetUpgradeVisual(m_PlUpgrade->m_UpgradeSkill03);

	OnUpgradeChanged();
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

void UUpgradePanel::UpgradeAtkCri01()
{
	m_PlUpgrade->UpgradeAtkCri01();
}

void UUpgradePanel::UpgradeAtkCDmg01()
{
	m_PlUpgrade->UpgradeAtkCDmg01();
}

void UUpgradePanel::UpgradeSkill01()
{
	m_PlUpgrade->UpgradeSkill01();
}

void UUpgradePanel::UpgradeSkill02()
{
	m_PlUpgrade->UpgradeSkill02();
}

void UUpgradePanel::UpgradeSkill03()
{
	m_PlUpgrade->UpgradeSkill03();
}

void UUpgradePanel::OnUpgradeChanged()
{
	m_UpgradeAtkDmg01->UpdateLevelText(m_PlUpgrade->m_UpgradeAtkDmg01);
	m_UpgradeAtkCri01->UpdateLevelText(m_PlUpgrade->m_UpgradeAtkCri01);
	m_UpgradeAtkCDmg01->UpdateLevelText(m_PlUpgrade->m_UpgradeAtkCDmg01);
	//
	m_UpgradeSkill01->UpdateLevelText(m_PlUpgrade->m_UpgradeSkill01);
	m_UpgradeSkill02->UpdateLevelText(m_PlUpgrade->m_UpgradeSkill02);
	m_UpgradeSkill03->UpdateLevelText(m_PlUpgrade->m_UpgradeSkill03);
}
