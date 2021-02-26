// My First Hack n Slash


#include "EquipmentPanel.h"

#include "Managers/DiabloGameInstance.h"

void UEquipmentPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_AryPlSkinBtn.Reset();

	FMargin MargW = FMargin(12.f,12.f,12.f,12.f);

	for(const FPlayerClassSpec& PlSpec : UDiabloGameInstance::Get->m_EquipManager->m_AryPlayerSkin)
	{
		UEquipmentButton* EqBtn = CreateWidget<UEquipmentButton>(this,m_ClassEquipBtn);

		EqBtn->SetUpgradeVisual(PlSpec);
		
		m_AryPlSkinBtn.Add(EqBtn);

		m_VertiSkin->AddChild(EqBtn);

		EqBtn->SetPadding(MargW);

		if(PlSpec.m_nEquippedSlot>0)
		{
			EqBtn->SetEquipped(true);
		}
		else
		{
			EqBtn->SetEquipped(false);
		}

		if(PlSpec.m_nLv<1)
		{
			//EqBtn->m_BtnEquip->SetIsEnabled(false);
		}
	}

	for(const FEquipmentSpec& EqSpec : UDiabloGameInstance::Get->m_EquipManager->m_AryWings)
	{
		UEquipmentButton* EqBtn = CreateWidget<UEquipmentButton>(this,m_ClassEquipBtn);

		EqBtn->SetUpgradeVisual(EqSpec);
		
		m_AryEqWingBtn.Add(EqBtn);

		m_VertiWing->AddChild(EqBtn);

		EqBtn->SetPadding(MargW);
	}

	for(const FEquipmentSpec& EqSpec : UDiabloGameInstance::Get->m_EquipManager->m_AryWeapons)
	{
		UEquipmentButton* EqBtn = CreateWidget<UEquipmentButton>(this,m_ClassEquipBtn);

		EqBtn->SetUpgradeVisual(EqSpec);
		
		m_AryEqWeaponBtn.Add(EqBtn);

		m_VertiWeapon->AddChild(EqBtn);

		EqBtn->SetPadding(MargW);
	}

	for(const FEquipmentSpec& EqSpec : UDiabloGameInstance::Get->m_EquipManager->m_AryPets)
	{
		UEquipmentButton* EqBtn = CreateWidget<UEquipmentButton>(this,m_ClassEquipBtn);
		EqBtn->SetUpgradeVisual(EqSpec);
		
		m_AryEqPetBtn.Add(EqBtn);

		m_VertiPet->AddChild(EqBtn);

		EqBtn->SetPadding(MargW);
	}
	
	for(const FEquipmentSpec& EqSpec : UDiabloGameInstance::Get->m_EquipManager->m_AryAcce)
	{
		UEquipmentButton* EqBtn = CreateWidget<UEquipmentButton>(this,m_ClassEquipBtn);

		EqBtn->SetUpgradeVisual(EqSpec);
		
		m_AryEqAccessoryBtn.Add(EqBtn);

		m_VertiAccessory->AddChild(EqBtn);

		EqBtn->SetPadding(MargW);
	}

	m_BtnClose->OnClicked.AddDynamic(this,&UEquipmentPanel::ClosePanel);

	m_BtnClassSkin->OnClicked.AddDynamic(this,&UEquipmentPanel::SetPanelSkin);
	m_BtnWeapon->OnClicked.AddDynamic(this,&UEquipmentPanel::UEquipmentPanel::SetPanelWeapon);
	m_BtnWing->OnClicked.AddDynamic(this,&UEquipmentPanel::UEquipmentPanel::SetPanelWing);
	m_BtnPet->OnClicked.AddDynamic(this,&UEquipmentPanel::UEquipmentPanel::SetPanelPet);
	m_BtnAccessory1->OnClicked.AddDynamic(this,&UEquipmentPanel::SetPanelAccessory);
	m_BtnAccessory2->OnClicked.AddDynamic(this,&UEquipmentPanel::SetPanelAccessory);
}

void UEquipmentPanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UEquipmentPanel::SetPanelSkin()
{
	m_SwitcherPanel->SetActiveWidget(m_OverlaySkin);
}

void UEquipmentPanel::SetPanelWing()
{
	m_SwitcherPanel->SetActiveWidget(m_OverlayWing);
}

void UEquipmentPanel::SetPanelPet()
{
	m_SwitcherPanel->SetActiveWidget(m_OverlayPet);
}

void UEquipmentPanel::SetPanelWeapon()
{
	m_SwitcherPanel->SetActiveWidget(m_OverlayWeapon);
}

void UEquipmentPanel::SetPanelAccessory()
{
	m_SwitcherPanel->SetActiveWidget(m_OverlayAccessory);
}

void UEquipmentPanel::OnUpgradeChanged()
{
	//m_UpgradeAtkDmg01->UpdateLevelText(m_PlUpgrade->m_UpgradeAtkDmg01);
	//m_UpgradeAtkCri01->UpdateLevelText(m_PlUpgrade->m_UpgradeAtkCri01);
	//m_UpgradeAtkCDmg01->UpdateLevelText(m_PlUpgrade->m_UpgradeAtkCDmg01);
	////
	//m_UpgradeSkill01->UpdateLevelText(m_PlUpgrade->m_UpgradeSkill01);
	//m_UpgradeSkill02->UpdateLevelText(m_PlUpgrade->m_UpgradeSkill02);
	//m_UpgradeSkill03->UpdateLevelText(m_PlUpgrade->m_UpgradeSkill03);
	////
	//m_UpgradeAtkDmg01->UpdateUpgradeable();
	//m_UpgradeAtkCri01->UpdateUpgradeable();
	//m_UpgradeAtkCDmg01->UpdateUpgradeable();
	//m_UpgradeSkill01->UpdateUpgradeable();
	//m_UpgradeSkill02->UpdateUpgradeable();
	//m_UpgradeSkill03->UpdateUpgradeable();
}

void UEquipmentPanel::OnEquipPlayerSkin()
{
	//해당 버튼이 인스턴스를 알아야한다
	//누굴 끼운건데?
}
