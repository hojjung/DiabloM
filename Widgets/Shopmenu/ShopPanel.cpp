// My First Hack n Slash


#include "ShopPanel.h"

#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"

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
	m_BtnPurchaseGold01->OnClicked.AddDynamic(this,&UShopPanel::PurchaseGold01);
	m_BtnPurchaseGold02->OnClicked.AddDynamic(this,&UShopPanel::PurchaseGold02);
	m_BtnPurchaseGold03->OnClicked.AddDynamic(this,&UShopPanel::PurchaseGold03);

	m_ShopManager = UDiabloGameInstance::Get->m_ShopManager; 
	
	m_ShopManager->m_OnUpdateGold.AddUObject(this,&UShopPanel::OnUpdateGoldShop);
	m_ShopManager->UpdateGold();
	//
	m_PackageItem01->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	m_PackageItem02->SetVisibility(ESlateVisibility::Collapsed);
	m_PackageItem03->SetVisibility(ESlateVisibility::Collapsed);
	m_PackageItem04->SetVisibility(ESlateVisibility::Collapsed);
	m_PackageItem05->SetVisibility(ESlateVisibility::Collapsed);
	//
	m_ShopManager->m_OnItemPurchased.AddUObject(this,&UShopPanel::UpdateShowPackage);
	UpdateShowPackage();
}

void UShopPanel::PurchasePacakge01()
{
	m_ShopManager->PurchasePacakge01();
}

void UShopPanel::PurchasePacakge02()
{
	m_ShopManager->PurchasePacakge02();
}

void UShopPanel::PurchasePacakge03()
{
	m_ShopManager->PurchasePacakge03();
}

void UShopPanel::PurchasePacakge04()
{
	m_ShopManager->PurchasePacakge04();
}

void UShopPanel::PurchasePacakge05()
{
	m_ShopManager->PurchasePacakge05();
}

void UShopPanel::PurchaseGemStone01()
{
	m_ShopManager->PurchaseGemStone01();
}

void UShopPanel::PurchaseGemStone02()
{
	m_ShopManager->PurchaseGemStone02();
}

void UShopPanel::PurchaseGemStone03()
{
	m_ShopManager->PurchaseGemStone03();
}

void UShopPanel::PurchaseGemStone04()
{
	m_ShopManager->PurchaseGemStone04();
}

void UShopPanel::PurchaseGemStone05()
{
	m_ShopManager->PurchaseGemStone05();
}

void UShopPanel::PurchaseGemStone06()
{
	m_ShopManager->PurchaseGemStone06();
}

void UShopPanel::PurchaseGold01()
{
	m_ShopManager->PurchaseGold01();
}

void UShopPanel::PurchaseGold02()
{
	m_ShopManager->PurchaseGold02();
}

void UShopPanel::PurchaseGold03()
{
	m_ShopManager->PurchaseGold03();
}

void UShopPanel::ClosePanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
	UDiabloGameInstance::Get->m_ShopManager->ShowBannerAD(true);
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

void UShopPanel::OnUpdateGoldShop(const BigInt& small,const BigInt& midium,const BigInt& large)
{
	m_TextGoldSmallAmount->SetText(FText::FromString(FString::Printf(TEXT("골드 %s 획득"),*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(small,2))));
	m_TextGoldMidiumAmount->SetText(FText::FromString(FString::Printf(TEXT("골드 %s 획득"),*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(midium,2))));
	m_TextGoldLargeAmount->SetText(FText::FromString(FString::Printf(TEXT("골드 %s 획득"),*UDiaBlueprintFunctionLibrary::GetAlphabetTextBigInt(large,2))));
}

void UShopPanel::UpdateShowPackage()
{
	//다 꺼놓고 1개씩만 나오게
	if(UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(0))
	{
		m_PackageItem01->SetVisibility(ESlateVisibility::Collapsed);
		m_PackageItem02->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if(UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(1))
	{
		m_PackageItem02->SetVisibility(ESlateVisibility::Collapsed);
		m_PackageItem03->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if(UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(2))
	{
		m_PackageItem03->SetVisibility(ESlateVisibility::Collapsed);
		m_PackageItem04->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if(UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(3))
	{
		m_PackageItem04->SetVisibility(ESlateVisibility::Collapsed);
		m_PackageItem05->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else if(UDiabloGameInstance::Get->m_ShopManager->GetPackagePurchased(4))
	{
		m_PackageItem05->SetVisibility(ESlateVisibility::Collapsed);
	}
}
