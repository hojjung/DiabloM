#include "GachaPanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/EquipManager.h"

void UGachaPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_BtnClose->OnClicked.AddDynamic(this,&UGachaPanel::ClosePanel);
	//
	m_GachaInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_GachaGridPanel->SetVisibility(ESlateVisibility::Collapsed);

	m_GachaManager = UDiabloGameInstance::Get->m_GachaManager;

	//m_BtnViewAds->SetVisibility(ESlateVisibility::Collapsed);
	m_BtnShowWeaponLevelInfo->OnClicked.AddDynamic(this,&UGachaPanel::ShowWeaponLevelInfo);
	m_BtnShowWeaponGachaRate->OnClicked.AddDynamic(this,&UGachaPanel::ShowWeaponGachaInfo);
	m_BtnRollGachaWeaponOneTime->OnClicked.AddDynamic(this,&UGachaPanel::RollGachaWeaponOneTime);
	m_BtnRollGachaWeaponElevenTime->OnClicked.AddDynamic(this,&UGachaPanel::RollGachaWeaponElevenTimes);
	m_BtnShowSkinLevelInfo->OnClicked.AddDynamic(this,&UGachaPanel::ShowSkinLevelInfo);
	m_BtnShowSkinGachaRate->OnClicked.AddDynamic(this,&UGachaPanel::UGachaPanel::ShowSkinGachaInfo);
	m_BtnRollGachaSkinOneTime->OnClicked.AddDynamic(this,&UGachaPanel::RollGachaSkinOneTime);
	m_BtnRollGachaSkinElevenTime->OnClicked.AddDynamic(this,&UGachaPanel::RollGachaSkinElevenTimes);
	m_BtnShowPetGachaRate->OnClicked.AddDynamic(this,&UGachaPanel::ShowPetGachaInfo);
	m_BtnRollGachaPetOneTime->OnClicked.AddDynamic(this,&UGachaPanel::RollGachaPetOneTime);
	m_BtnRollGachaPetElevenTime->OnClicked.AddDynamic(this,&UGachaPanel::RollGachaPetElevenTimes);
	m_BtnShowWingGachaRate->OnClicked.AddDynamic(this,&UGachaPanel::ShowWingGachaInfo);
	m_BtnRollGachaWingOneTime->OnClicked.AddDynamic(this,&UGachaPanel::RollGachaWingOneTime);
	m_BtnRollGachaWingElevenTime->OnClicked.AddDynamic(this,&UGachaPanel::RollGachaWingElevenTimes);
	m_BtnShowAccesoryGachaRate->OnClicked.AddDynamic(this,&UGachaPanel::ShowAccessoryGachaInfo);
	m_BtnRollGachaAccesoryOneTime->OnClicked.AddDynamic(this,&UGachaPanel::RollGachaAccessoryOneTime);
	m_BtnRollGachaAccesoryElevenTime->OnClicked.AddDynamic(this,&UGachaPanel::RollGachaAccessoryElevenTimes);
}

void UGachaPanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
	UDiabloGameInstance::Get->m_PlayfabManager->ShowBannerAd(true);
}


void UGachaPanel::ShowWeaponLevelInfo()
{
	
}

void UGachaPanel::ShowWeaponGachaInfo()
{
	m_GachaInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_GachaInfoPanel->OpenGachaInfo(m_GachaManager->GetCurrentLevelWeaponTable(),
		m_GachaManager->GetCurrentLevelWeaponTotalPercent());
}

void UGachaPanel::RollGachaWeaponOneTime()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollWeapon);
	m_GachaGridPanel->RollGachaOneTime();
}

void UGachaPanel::RollGachaWeaponElevenTimes()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollWeapon);
	m_GachaGridPanel->RollGachaElevenTime();
}

void UGachaPanel::ShowSkinLevelInfo()
{
	
}

void UGachaPanel::ShowSkinGachaInfo()
{
	m_GachaInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_GachaInfoPanel->OpenGachaInfo(m_GachaManager->GetCurrentLevelSkinTable(),
        m_GachaManager->GetCurrentLevelSkinTotalPercent());
}

void UGachaPanel::RollGachaSkinOneTime()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollSkin);
	m_GachaGridPanel->RollGachaOneTime();
}

void UGachaPanel::RollGachaSkinElevenTimes()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollSkin);
	m_GachaGridPanel->RollGachaElevenTime();
}

void UGachaPanel::ShowPetGachaInfo()
{
	m_GachaInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_GachaInfoPanel->OpenGachaInfo(m_GachaManager->m_AryGachaPet,
        m_GachaManager->m_fTotalPetGacha);
}

void UGachaPanel::RollGachaPetOneTime()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollPet);
	m_GachaGridPanel->RollGachaOneTime();
}

void UGachaPanel::RollGachaPetElevenTimes()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollPet);
	m_GachaGridPanel->RollGachaElevenTime();
}

void UGachaPanel::ShowWingGachaInfo()
{
	m_GachaInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	
	m_GachaInfoPanel->OpenGachaInfo(m_GachaManager->m_AryGachaWing,
        m_GachaManager->m_fTotalWingGacha);
}

void UGachaPanel::RollGachaWingOneTime()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollWing);
	m_GachaGridPanel->RollGachaOneTime();
}

void UGachaPanel::RollGachaWingElevenTimes()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollWing);
	m_GachaGridPanel->RollGachaOneTime();
}

void UGachaPanel::ShowAccessoryGachaInfo()
{
	m_GachaInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    	
    m_GachaInfoPanel->OpenGachaInfo(m_GachaManager->m_AryGachaAccessory,
		m_GachaManager->m_fTotalAccessoryGacha);
}

void UGachaPanel::RollGachaAccessoryOneTime()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollAccessory);
	m_GachaGridPanel->RollGachaOneTime();
}

void UGachaPanel::RollGachaAccessoryElevenTimes()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollAccessory);
	m_GachaGridPanel->RollGachaElevenTime();
}
