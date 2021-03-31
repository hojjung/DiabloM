#include "UpgradePanel.h"
#include "Managers/DiabloGameInstance.h"

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
	m_UpgradeAtkDmg01->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeAtkDmg01);
	m_UpgradeAtkDmg01->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeAtkDmg01);
	m_UpgradeAtkDmg01->SetUpgradeVisual(m_PlUpgrade->GetAtkUp(EAttackType::BaseAttack));
	//upgradeAtkCri01
	m_UpgradeAtkCri01->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeAtkCri01);
	m_UpgradeAtkCri01->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeAtkCri01);
	m_UpgradeAtkCri01->SetUpgradeVisual(m_PlUpgrade->GetAtkUp(EAttackType::Critical));
	//upgradeAtkCDmg01
	m_UpgradeAtkCDmg01->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeAtkCDmg01);
	m_UpgradeAtkCDmg01->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeAtkCDmg01);
	m_UpgradeAtkCDmg01->SetUpgradeVisual(m_PlUpgrade->GetAtkUp(EAttackType::CriticalDmg));
	//
	m_UpgradeAtkCri02->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeAtkCri02);
	m_UpgradeAtkCri02->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeAtkCri02);
	m_UpgradeAtkCri02->SetUpgradeVisual(m_PlUpgrade->GetAtkUp(EAttackType::SuperCritical));
	//
	m_UpgradeAtkCDmg02->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeAtkCDmg02);
	m_UpgradeAtkCDmg02->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeAtkCDmg02);
	m_UpgradeAtkCDmg02->SetUpgradeVisual(m_PlUpgrade->GetAtkUp(EAttackType::SuperCriticalDmg));
	//
	m_UpgradeAtkMagic01->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeAtkMagic01);
	m_UpgradeAtkMagic01->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeAtkMagic01);
	m_UpgradeAtkMagic01->SetUpgradeVisual(m_PlUpgrade->GetAtkUp(EAttackType::MagicBomb));
	//
	m_UpgradeAtkMDmg01->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeAtkMDmg01);
	m_UpgradeAtkMDmg01->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeAtkMDmg01);
	m_UpgradeAtkMDmg01->SetUpgradeVisual(m_PlUpgrade->GetAtkUp(EAttackType::MagicBombDmg));
	//
	m_UpgradeAtkMagic02->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeAtkMagic02);
	m_UpgradeAtkMagic02->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeAtkMagic02);
	m_UpgradeAtkMagic02->SetUpgradeVisual(m_PlUpgrade->GetAtkUp(EAttackType::SuperMagicBomb));
	//
	m_UpgradeAtkMDmg02->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeAtkMDmg02);
	m_UpgradeAtkMDmg02->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeAtkMDmg02);
	m_UpgradeAtkMDmg02->SetUpgradeVisual(m_PlUpgrade->GetAtkUp(EAttackType::SuperMagicBombDmg));
	//upgradeAtkCDmg01
	m_UpgradeSkill01->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeSkill01);
	m_UpgradeSkill01->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeSkill01);
	m_UpgradeSkill01->Init(m_PlUpgrade->GetSkillUp(ESkillType::DeathBlow));
	m_UpgradeSkill01->m_OnClicked.AddUObject(this,&UUpgradePanel::OnSkillEquipPressed);
	//upgradeAtkCDmg01
	m_UpgradeSkill02->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeSkill02);
	m_UpgradeSkill02->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeSkill02);
	m_UpgradeSkill02->Init(m_PlUpgrade->GetSkillUp(ESkillType::MagicBlade));
	m_UpgradeSkill02->m_OnClicked.AddUObject(this,&UUpgradePanel::OnSkillEquipPressed);
	//upgradeAtkCDmg01
	m_UpgradeSkill03->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeSkill03);
	m_UpgradeSkill03->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeSkill03);
	m_UpgradeSkill03->Init(m_PlUpgrade->GetSkillUp(ESkillType::WhirlWind));
	m_UpgradeSkill03->m_OnClicked.AddUObject(this,&UUpgradePanel::OnSkillEquipPressed);
	//
	m_UpgradeSkill04->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeSkill04);
	m_UpgradeSkill04->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeSkill04);
	m_UpgradeSkill04->Init(m_PlUpgrade->GetSkillUp(ESkillType::EarthQuake));
	m_UpgradeSkill04->m_OnClicked.AddUObject(this,&UUpgradePanel::OnSkillEquipPressed);
	//
	m_UpgradeSkill05->m_BtnLvUp->OnClicked.AddDynamic(this,&UUpgradePanel::UpgradeSkill05);
	m_UpgradeSkill05->m_OnCharge.BindUObject(this,&UUpgradePanel::UpgradeSkill05);
	m_UpgradeSkill05->Init(m_PlUpgrade->GetSkillUp(ESkillType::WindBlade));
	m_UpgradeSkill05->m_OnClicked.AddUObject(this,&UUpgradePanel::OnSkillEquipPressed);

	OnUpgradeChanged();
	CloseSkillHotkeyPanel();
	
}

void UUpgradePanel::SetPanel1()
{
	m_SwitcherUpgradePanel->SetActiveWidgetIndex(0);
	CloseSkillHotkeyPanel();
}

void UUpgradePanel::SetPanel2()
{
	m_SwitcherUpgradePanel->SetActiveWidgetIndex(1);
}

void UUpgradePanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
	
	CloseSkillHotkeyPanel();

	UDiabloGameInstance::Get->m_ShopManager->ShowBannerAD(true);

}

void UUpgradePanel::UpgradeAtkDmg01()
{
	m_PlUpgrade->UpgradeAtk(EAttackType::BaseAttack);
}

void UUpgradePanel::UpgradeAtkCri01()
{
	m_PlUpgrade->UpgradeAtk(EAttackType::Critical);
}

void UUpgradePanel::UpgradeAtkCDmg01()
{
	m_PlUpgrade->UpgradeAtk(EAttackType::CriticalDmg);
}

void UUpgradePanel::UpgradeAtkCri02()
{
	m_PlUpgrade->UpgradeAtk(EAttackType::SuperCritical);
}

void UUpgradePanel::UpgradeAtkCDmg02()
{
	m_PlUpgrade->UpgradeAtk(EAttackType::SuperCriticalDmg);
}

void UUpgradePanel::UpgradeAtkMagic01()
{
	m_PlUpgrade->UpgradeAtk(EAttackType::MagicBomb);
}

void UUpgradePanel::UpgradeAtkMDmg01()
{
	m_PlUpgrade->UpgradeAtk(EAttackType::MagicBombDmg);
}

void UUpgradePanel::UpgradeAtkMagic02()
{
	m_PlUpgrade->UpgradeAtk(EAttackType::SuperMagicBomb);
}

void UUpgradePanel::UpgradeAtkMDmg02()
{
	m_PlUpgrade->UpgradeAtk(EAttackType::SuperMagicBombDmg);
}

void UUpgradePanel::UpgradeSkill01()
{
	m_PlUpgrade->UpgradeSkill(ESkillType::DeathBlow);
}

void UUpgradePanel::UpgradeSkill02()
{
	m_PlUpgrade->UpgradeSkill(ESkillType::MagicBlade);
}

void UUpgradePanel::UpgradeSkill03()
{
	m_PlUpgrade->UpgradeSkill(ESkillType::WhirlWind);
}

void UUpgradePanel::UpgradeSkill04()
{
	m_PlUpgrade->UpgradeSkill(ESkillType::EarthQuake);
}

void UUpgradePanel::UpgradeSkill05()
{
	m_PlUpgrade->UpgradeSkill(ESkillType::WindBlade);
}

void UUpgradePanel::OnUpgradeChanged()
{
	m_UpgradeAtkDmg01->UpdateLevelText();
	m_UpgradeAtkCri01->UpdateLevelText();
	m_UpgradeAtkCDmg01->UpdateLevelText();
	m_UpgradeAtkCri02->UpdateLevelText();
	m_UpgradeAtkCDmg02->UpdateLevelText();
	m_UpgradeAtkMagic01->UpdateLevelText();
	m_UpgradeAtkMDmg01->UpdateLevelText();
	m_UpgradeAtkMagic02->UpdateLevelText();
	m_UpgradeAtkMDmg02->UpdateLevelText();
	//
	m_UpgradeSkill01->UpdateSkillButton();
	m_UpgradeSkill02->UpdateSkillButton();
	m_UpgradeSkill03->UpdateSkillButton();
	m_UpgradeSkill04->UpdateSkillButton();
	m_UpgradeSkill05->UpdateSkillButton();
}

void UUpgradePanel::CloseSkillHotkeyPanel()
{
	m_SkillEquipBtnProxy->SetVisibility(ESlateVisibility::Collapsed);
		
	if(USkillUpgradeButton::CurrentSelected)
	{
		USkillUpgradeButton::CurrentSelected->SetNormalImage();
		USkillUpgradeButton::CurrentSelected = nullptr;
	}
}

void UUpgradePanel::OnSkillEquipPressed(USkillUpgradeButton* btn)//EquipPress
{
	PRINTF("EquipSkill");
	if (USkillUpgradeButton::CurrentSelected == btn)
	{
		CloseSkillHotkeyPanel();
	}
	else
	{
		if(m_SkillEquipBtnProxy->GetVisibility()!=ESlateVisibility::SelfHitTestInvisible)
		{
			m_SkillEquipBtnProxy->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			m_SkillEquipBtnProxy->PlayBlink();
		}
		
		USkillUpgradeButton::CurrentSelected=btn;
		USkillUpgradeButton::CurrentSelected->SetHoverImage();
	}
	
}


