// My First Hack n Slash


#include "EquipmentPanel.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/EquipManager.h"

void UEquipmentPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_AryPlSkinBtn.Reset();

	FMargin MargW = FMargin(12.f,12.f,12.f,12.f);

	int Index = 0;

	for(const FPlayerClassSpec& PlSpec : UDiabloGameInstance::Get->m_EquipManager->m_AryPlayerSkin)
	{
		UPlayerSkinEquipButton* EqBtn = CreateWidget<UPlayerSkinEquipButton>(this,m_ClassPlayerEquipBtn);

		EqBtn->Init(PlSpec,Index++);
		
		m_AryPlSkinBtn.Add(EqBtn);

		m_VertiSkin->AddChild(EqBtn);

		EqBtn->SetPadding(MargW);

		if(PlSpec.m_nIsUnlocked<1)
		{
			//EqBtn->m_BtnEquip->SetIsEnabled(false);
		}
	}

	Index = 0;

	for(const FWingSpec& EqSpec : UDiabloGameInstance::Get->m_EquipManager->m_AryWings)
	{
		UWingEquipButton* EqBtn = CreateWidget<UWingEquipButton>(this,m_ClassWingEquipBtn);

		EqBtn->Init(EqSpec,Index++);
		
		m_AryEqWingBtn.Add(EqBtn);

		m_VertiWing->AddChild(EqBtn);

		EqBtn->SetPadding(MargW);

		if(EqSpec.m_nIsUnlocked<1)
		{
			//EqBtn->m_BtnEquip->SetIsEnabled(false);
		}
	}

	Index++;

	for(const FWeaponSpec& EqSpec : UDiabloGameInstance::Get->m_EquipManager->m_AryWeapons)
	{
		UWeaponEquipButton* EqBtn = CreateWidget<UWeaponEquipButton>(this,m_ClassWeaponEquipBtn);

		EqBtn->Init(EqSpec,Index);
		
		m_AryEqWeaponBtn.Add(EqBtn);

		m_VertiWeapon->AddChild(EqBtn);

		EqBtn->SetPadding(MargW);

		if(EqSpec.m_nLv<1)
		{
			//EqBtn->m_BtnEquip->SetIsEnabled(false);
		}
	}

	Index++;

	for(const FPetSpec& EqSpec : UDiabloGameInstance::Get->m_EquipManager->m_AryPets)
	{
		UPetEquipButton* EqBtn = CreateWidget<UPetEquipButton>(this,m_ClassPetEquipBtn);
		
		EqBtn->Init(EqSpec,Index);
		
		m_AryEqPetBtn.Add(EqBtn);

		m_VertiPet->AddChild(EqBtn);

		EqBtn->SetPadding(MargW);

		if(EqSpec.m_nLv<1)
		{
			//EqBtn->m_BtnEquip->SetIsEnabled(false);
		}
	}

	Index = 0;
	
	for(const FAccessorySpec& EqSpec : UDiabloGameInstance::Get->m_EquipManager->m_AryAcce)
	{
		UAccessoryEquipButton* EqBtn = CreateWidget<UAccessoryEquipButton>(this,m_ClassAccessoryEquipBtn);

		EqBtn->Init(EqSpec,Index++);
		
		m_AryEqAccessoryBtn.Add(EqBtn);

		m_VertiAccessory->AddChild(EqBtn);

		EqBtn->SetPadding(MargW);

		if(EqSpec.m_nLv<1)
		{
			//EqBtn->m_BtnEquip->SetIsEnabled(false);
		}
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


