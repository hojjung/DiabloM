#include "GachaPanel.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/EquipManager.h"
#include "Managers/ShopManager.h"

void UGachaPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_BtnClose->OnClicked.AddDynamic(this, &UGachaPanel::ClosePanel);
	//
	m_GachaInfoPanel->SetVisibility(ESlateVisibility::Collapsed);
	m_GachaGridPanel->SetVisibility(ESlateVisibility::Collapsed);

	m_GachaManager = UDiabloGameInstance::Get->m_GachaManager;

	m_ShopManager = UDiabloGameInstance::Get->m_ShopManager;

	m_BtnViewAds->OnClicked.AddDynamic(this, &UGachaPanel::ShowAds);
	
	m_BtnShowWeaponGachaRate->OnClicked.AddDynamic(this, &UGachaPanel::ShowWeaponGachaInfo);
	m_BtnShowSkinGachaRate->OnClicked.AddDynamic(this, &UGachaPanel::ShowSkinGachaInfo);
	m_BtnShowPetGachaRate->OnClicked.AddDynamic(this, &UGachaPanel::ShowPetGachaInfo);
	m_BtnShowWingGachaRate->OnClicked.AddDynamic(this, &UGachaPanel::ShowWingGachaInfo);
	//m_BtnRollGachaWingOneTime->OnClicked.AddDynamic(m_ShopManager.Get(),&UShopManager::PurchaseGachaPet01);
	//m_BtnRollGachaWingElevenTime->OnClicked.AddDynamic(this,&UGachaPanel::RollGachaWingElevenTimes);
	m_BtnShowAccesoryGachaRate->OnClicked.AddDynamic(this, &UGachaPanel::ShowAccessoryGachaInfo);
	//
	m_BtnRollGachaWeaponOneTime->OnClicked.AddDynamic(m_ShopManager.Get(), &UShopManager::PurchaseGachaWeapon01);
	m_BtnRollGachaWeaponElevenTime->OnClicked.AddDynamic(m_ShopManager.Get(), &UShopManager::PurchaseGachaWeapon11);
	m_BtnRollGachaSkinOneTime->OnClicked.AddDynamic(m_ShopManager.Get(), &UShopManager::PurchaseGachaSkin01);
	m_BtnRollGachaSkinElevenTime->OnClicked.AddDynamic(m_ShopManager.Get(), &UShopManager::PurchaseGachaSkin11);
	m_BtnRollGachaPetOneTime->OnClicked.AddDynamic(m_ShopManager.Get(), &UShopManager::PurchaseGachaPet01);
	m_BtnRollGachaPetElevenTime->OnClicked.AddDynamic(m_ShopManager.Get(), &UShopManager::PurchaseGachaPet11);
	m_BtnRollGachaAccesoryOneTime->OnClicked.AddDynamic(m_ShopManager.Get(), &UShopManager::PurchaseGachaAccessory01);
	m_BtnRollGachaAccesoryElevenTime->OnClicked.
	                                  AddDynamic(m_ShopManager.Get(), &UShopManager::PurchaseGachaAccessory11);
	//
	m_GachaManager->m_OnGachaRollSkin.AddUObject(this, &UGachaPanel::UpdateGachaSkinLevelCount);
	m_GachaManager->m_OnGachaRollWeapon.AddUObject(this, &UGachaPanel::UpdateGachaWeaponLevelCount);

	UpdateGachaSkinLevelCount(m_GachaManager->m_nGachaSkinCount, m_GachaManager->GetGachaSkinMaxCount(),
	                          m_GachaManager->m_nCurrentSkinIndex);
	UpdateGachaWeaponLevelCount(m_GachaManager->m_nGachaWeaponCount, m_GachaManager->GetGachaWeaponMaxCount(),
	                            m_GachaManager->m_nCurrentWeaponIndex);
}

void UGachaPanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
	UDiabloGameInstance::Get->m_AdverManager->ShowBannerAD(true);
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

void UGachaPanel::RollGachaWeaponFiftyTimes()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollWeapon);
	m_GachaGridPanel->RollGachaFiftyTime();
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

void UGachaPanel::RollGachaSkinFiftyTimes()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollSkin);
	m_GachaGridPanel->RollGachaFiftyTime();
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

void UGachaPanel::RollGachaPetFiftyTimes()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollPet);
	m_GachaGridPanel->RollGachaFiftyTime();
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

void UGachaPanel::RollGachaWingFiftyTimes()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollWing);
	m_GachaGridPanel->RollGachaFiftyTime();
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

void UGachaPanel::RollGachaAccessoryFiftyTimes()
{
	m_GachaGridPanel->SetRollGachaData(ERollItemType::RollAccessory);
	m_GachaGridPanel->RollGachaFiftyTime();
}

void UGachaPanel::ShowAds()
{
	UDiabloGameInstance::Get->m_AdverManager->ShowRewardAds();
}

void UGachaPanel::UpdateGachaWeaponLevelCount(int c, int m, int lv)
{
	m_TextGachaWeaponNameLevel->SetText(FText::FromString(FString::Printf(TEXT("무기 가챠:Lv.%d"), lv + 1)));

	if (lv == 7)
	{
		m_TextGachaLevelExpBarWeapon->SetText(FText::FromString(TEXT("MAX")));
		m_BarGachaLevelExpWeapon->SetProgressValue(1.f);
		return;
	}

	m_TextGachaLevelExpBarWeapon->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), c, m)));

	float Current = c;

	float Max = m;

	float Gauge = Current / Max;

	m_BarGachaLevelExpWeapon->SetProgressValue(Gauge);
}

void UGachaPanel::UpdateGachaSkinLevelCount(int c, int m, int lv)
{
	m_TextGachaSkinNameLevel->SetText(FText::FromString(FString::Printf(TEXT("변신 가챠:Lv.%d"), lv + 1)));

	if (lv == 7)
	{
		m_TextGachaLevelExpBarSkin->SetText(FText::FromString(TEXT("MAX")));
		m_BarGachaLevelExpSkin->SetProgressValue(1.f);
		return;
	}

	m_TextGachaLevelExpBarSkin->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), c, m)));

	float Current = c;

	float Max = m;

	float Gauge = Current / Max;

	m_BarGachaLevelExpSkin->SetProgressValue(Gauge);
}

FReply UGachaPanel::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	return FReply::Handled();
}

FReply UGachaPanel::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	return FReply::Handled();
}

FReply UGachaPanel::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Re = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	return FReply::Handled();
}
