// My First Hack n Slash


#include "ShopPanel.h"

#include "Managers/DiabloGameInstance.h"

void UShopPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnPackage->OnClicked.AddDynamic(this,&UShopPanel::ShowPackagePanel);
	m_BtnGemStone->OnClicked.AddDynamic(this,&UShopPanel::ShowGemStonePanel);
	m_BtnGold->OnClicked.AddDynamic(this,&UShopPanel::ShowGoldPanel);
	m_BtnClose->OnClicked.AddDynamic(this,&UShopPanel::ClosePanel);
	m_BtnPurchasePackage01->OnClicked.AddDynamic(this,&UShopPanel::PurchasePacakge01);
	m_BtnPurchasePackage02->OnClicked.AddDynamic(this,&UShopPanel::PurchasePacakge02);
	m_BtnPurchasePackage03->OnClicked.AddDynamic(this,&UShopPanel::PurchasePacakge03);
	m_BtnPurchasePackage04->OnClicked.AddDynamic(this,&UShopPanel::PurchasePacakge04);
	m_BtnPurchasePackage05->OnClicked.AddDynamic(this,&UShopPanel::PurchasePacakge05);
	m_BtnPurchaseGemStone01->OnClicked.AddDynamic(this,&UShopPanel::PurchaseGemStone01);
	m_BtnPurchaseGemStone02->OnClicked.AddDynamic(this,&UShopPanel::PurchaseGemStone02);
	m_BtnPurchaseGemStone03->OnClicked.AddDynamic(this,&UShopPanel::PurchaseGemStone03);
	m_BtnPurchaseGemStone04->OnClicked.AddDynamic(this,&UShopPanel::PurchaseGemStone04);
	m_BtnPurchaseGemStone05->OnClicked.AddDynamic(this,&UShopPanel::PurchaseGemStone05);
	m_BtnPurchaseGemStone06->OnClicked.AddDynamic(this,&UShopPanel::PurchaseGemStone06);
}

void UShopPanel::PurchasePacakge01()
{
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP("package_starter",false);
}

void UShopPanel::PurchasePacakge02()
{
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP("package_begginer",false);
}

void UShopPanel::PurchasePacakge03()
{
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP("package_rare",false);
}

void UShopPanel::PurchasePacakge04()
{
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP("package_hero",false);
}

void UShopPanel::PurchasePacakge05()
{
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP("package_legend",false);
}

void UShopPanel::PurchaseGemStone01()
{
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP("gemstone01",true);
}

void UShopPanel::PurchaseGemStone02()
{
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP("gemstone02",true);
}

void UShopPanel::PurchaseGemStone03()
{
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP("gemstone03",true);
}

void UShopPanel::PurchaseGemStone04()
{
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP("gemstone04",true);
}

void UShopPanel::PurchaseGemStone05()
{
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP("gemstone05",true);
}

void UShopPanel::PurchaseGemStone06()
{
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP("gemstone06",true);
}

void UShopPanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
	
}

void UShopPanel::ShowPackagePanel()
{
	m_SwitcherPanel->SetActiveWidgetIndex(0);
}

void UShopPanel::ShowGemStonePanel()
{
	m_SwitcherPanel->SetActiveWidgetIndex(1);
}

void UShopPanel::ShowGoldPanel()
{
	m_SwitcherPanel->SetActiveWidgetIndex(2);
}
