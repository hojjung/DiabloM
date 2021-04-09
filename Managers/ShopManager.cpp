#include "ShopManager.h"
#include "Managers/DungeonManager.h"
#include "DiabloGameInstance.h"
#include "Widgets/GachaMenu/GachaPanel.h"

UShopManager::UShopManager()
{
	m_nOfflineHours=-1;
	m_nDDay = 0;
	m_bIsFirstTime = false;
}

void UShopManager::SetShopDataFromServer()
{
	const TArray<FString>& AryItemBought = UDiabloGameInstance::Get->m_PlayfabManager->m_AryIAPData;

	//0~6,2개제외
	m_PackagePurchased.Init(false, 5);
	m_PackagePurchased[0] = AryItemBought[1].ToBool();
	m_PackagePurchased[1] = AryItemBought[2].ToBool();
	m_PackagePurchased[2] = AryItemBought[3].ToBool();
	m_PackagePurchased[3] = AryItemBought[4].ToBool();
	m_PackagePurchased[4] = AryItemBought[5].ToBool();

	UDiabloGameInstance::Get->m_GachaManager->SetGachaLevel(AryItemBought[6], AryItemBought[7]);

	UDiabloGameInstance::Get->m_DungeonManager->m_OnDungeonMaxUpdate.AddUObject(this, &UShopManager::UpdateGold);
}

void UShopManager::SetOfflineHours(FDateTime& currentTime)
{
	m_bIsFirstTime = UDiabloGameInstance::Get->m_PlayfabManager->m_bIsNewCreatePlayer;
	
	const TArray<FString>& AryItemBought = UDiabloGameInstance::Get->m_PlayfabManager->m_AryIAPData;

	if (AryItemBought.Num() >= 10) //정상적인 업데이트 된사람
	{
		m_nDDay = FCString::Atoi(*AryItemBought[9]);
	}
	
	if (AryItemBought.Num() >= 11) //정상적인 업데이트 된사람
	{
		m_bIsFirstTime = false;
		
		FDateTime LastDailyClaimTime;
		
		if(FDateTime::Parse(AryItemBought[10],LastDailyClaimTime))
		{
			FTimespan DailyRewardTimeSpen = currentTime - LastDailyClaimTime;

			m_nOfflineHours = DailyRewardTimeSpen.GetTotalHours();
		}
	}
	else//데이터 없음 처음으로 인식
	{
		m_bIsFirstTime = true;
	}

	m_OnOfflineHoursSet.Broadcast();
}

void UShopManager::PurchasePacakge01()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP(TEXT("package_starter"), true);
}

void UShopManager::PurchasePacakge02()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP(TEXT("package_begginer"), true);
}

void UShopManager::PurchasePacakge03()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP(TEXT("package_rare"), true);
}

void UShopManager::PurchasePacakge04()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP(TEXT("package_hero"), true);
}

void UShopManager::PurchasePacakge05()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP(TEXT("package_legend"), true);
}

void UShopManager::PurchaseGemStone01()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP(TEXT("gemstone01"), true);
}

void UShopManager::PurchaseGemStone02()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP(TEXT("gemstone02"), true);
}

void UShopManager::PurchaseGemStone03()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP(TEXT("gemstone03"), true);
}

void UShopManager::PurchaseGemStone04()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP(TEXT("gemstone04"), true);
}

void UShopManager::PurchaseGemStone05()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP(TEXT("gemstone05"), true);
}

void UShopManager::PurchaseGemStone06()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->BuyIAP(TEXT("gemstone06"), true);
}

void UShopManager::PurchaseGold01()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(30, TEXT("gold01"));
}

void UShopManager::PurchaseGold02()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(300, TEXT("gold02"));
}

void UShopManager::PurchaseGold03()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(3000, TEXT("gold03"));
}

void UShopManager::PurchaseGachaWeapon01()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(30, TEXT("gachaweapon01"));
}

void UShopManager::PurchaseGachaWeapon11()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(300, TEXT("gachaweapon11"));
}

void UShopManager::PurchaseGachaWeapon55()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(1500, TEXT("gachaweapon55"));
}

void UShopManager::PurchaseGachaSkin01()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(30, TEXT("gachaskin01"));
}

void UShopManager::PurchaseGachaSkin11()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(300, TEXT("gachaskin11"));
}

void UShopManager::PurchaseGachaSkin55()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(1500, TEXT("gachaskin55"));
}

void UShopManager::PurchaseGachaPet01()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(90, TEXT("gachapet01"));
}

void UShopManager::PurchaseGachaPet11()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(900, TEXT("gachapet11"));
}

void UShopManager::PurchaseGachaPet55()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(4500, TEXT("gachapet55"));
}

void UShopManager::PurchaseGachaAccessory01()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(270, TEXT("gachaaccessory01"));
}

void UShopManager::PurchaseGachaAccessory11()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(2700, TEXT("gachaaccessory11"));
}

void UShopManager::PurchaseGachaAccessory55()
{
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(13500, TEXT("gachaaccessory55"));
}

void UShopManager::UpdateGold()
{
	BigInt Bounty = UDiabloGameInstance::Get->m_DungeonManager->GetCurrentDungeonBounty();

	Bounty.Multiply(3500);

	Bounty.Multiply(UDiabloGameInstance::Get->m_DungeonManager->GetLevelBonus());

	m_Gold01 = Bounty;

	Bounty.Multiply(15);

	m_Gold02 = Bounty;

	Bounty.Multiply(20);

	m_Gold03 = Bounty;

	m_OnUpdateGold.Broadcast(m_Gold01, m_Gold02, m_Gold03);
}

void UShopManager::RollGachaOneTime()
{
	ERollItemType RollType = m_GachaPanel->m_GachaGridPanel->GetCurrentType();

	switch (RollType)
	{
	case RollWeapon: PurchaseGachaWeapon01();
		break;
	case RollSkin: PurchaseGachaSkin01();
		break;
	case RollPet: PurchaseGachaPet01();
		break;
	case RollAccessory: PurchaseGachaAccessory01();
		break;
	}
}

void UShopManager::RollGachaElevenTime()
{
	ERollItemType RollType = m_GachaPanel->m_GachaGridPanel->GetCurrentType();

	switch (RollType)
	{
	case RollWeapon: PurchaseGachaWeapon11();
		break;
	case RollSkin: PurchaseGachaSkin11();
		break;
	case RollPet: PurchaseGachaPet11();
		break;
	case RollAccessory: PurchaseGachaAccessory11();
		break;
	}
}

void UShopManager::RollGachaFiftyTime()
{
	ERollItemType RollType = m_GachaPanel->m_GachaGridPanel->GetCurrentType();

	switch (RollType)
	{
	case RollWeapon: PurchaseGachaWeapon55();
		break;
	case RollSkin: PurchaseGachaSkin55();
		break;
	case RollPet: PurchaseGachaPet55();
		break;
	case RollAccessory: PurchaseGachaAccessory55();
		break;
	}
}

void UShopManager::OnPurchasedGainItem(FString itemID)
{
	PRINTF("PurchasedItem:%s", *itemID);

	if (itemID == TEXT("package_starter"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(60000);
		m_PackagePurchased[0] = true;
		m_OnItemPurchased.Broadcast();
	}
	else if (itemID == TEXT("package_begginer"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(60000);
		m_PackagePurchased[1] = true;
		m_OnItemPurchased.Broadcast();
	}
	else if (itemID == TEXT("package_rare"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(60000);
		m_PackagePurchased[2] = true;
		m_OnItemPurchased.Broadcast();
	}
	else if (itemID == TEXT("package_hero"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(150000);
		m_PackagePurchased[3] = true;
		m_OnItemPurchased.Broadcast();
	}
	else if (itemID == TEXT("package_legend"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(150000);
		m_PackagePurchased[4] = true;
		m_OnItemPurchased.Broadcast();
	}
	else if (itemID == TEXT("gemstone01"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(320);
	}
	else if (itemID == TEXT("gemstone02"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(1000);
	}
	else if (itemID == TEXT("gemstone03"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(3300);
	}
	else if (itemID == TEXT("gemstone04"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(20000);
	}
	else if (itemID == TEXT("gemstone05"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(44000);
	}
	else if (itemID == TEXT("gemstone06"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->AddGemStone(89000);
	}
	else if (itemID == TEXT("gold01"))
	{
		UDiabloGameInstance::Get->m_GoldManager->AddGold(m_Gold01, false);
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gold02"))
	{
		UDiabloGameInstance::Get->m_GoldManager->AddGold(m_Gold02, false);
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gold03"))
	{
		UDiabloGameInstance::Get->m_GoldManager->AddGold(m_Gold03, false);
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaweapon01"))
	{
		m_GachaPanel->RollGachaWeaponOneTime();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaweapon11"))
	{
		m_GachaPanel->RollGachaWeaponElevenTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaweapon55"))
	{
		m_GachaPanel->RollGachaWeaponFiftyTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaskin01"))
	{
		m_GachaPanel->RollGachaSkinOneTime();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaskin11"))
	{
		m_GachaPanel->RollGachaSkinElevenTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaskin55"))
	{
		m_GachaPanel->RollGachaSkinFiftyTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachapet01"))
	{
		m_GachaPanel->RollGachaPetOneTime();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachapet11"))
	{
		m_GachaPanel->RollGachaPetElevenTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachapet55"))
	{
		m_GachaPanel->RollGachaPetFiftyTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaaccessory01"))
	{
		m_GachaPanel->RollGachaAccessoryOneTime();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaaccessory11"))
	{
		m_GachaPanel->RollGachaAccessoryElevenTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaaccessory55"))
	{
		m_GachaPanel->RollGachaAccessoryFiftyTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}

	UDiabloGameInstance::Get->m_PlayfabManager->UploadUserTitleData();
}

void UShopManager::ShowBannerAD(bool b)
{
	if (b && (!GetDefault<UPlayFabRuntimeSettings>()->bIsVIPGameVersion) && !m_PackagePurchased[0])
	{
		UKismetSystemLibrary::ShowAdBanner(0, false);
		m_OnShowAdBanner.Broadcast(true);
	}
	else
	{
		UKismetSystemLibrary::HideAdBanner();

		m_OnShowAdBanner.Broadcast(false);
	}
}

bool UShopManager::GetPackagePurchased(int index)
{
	return m_PackagePurchased[index];
}

FString UShopManager::GetIAPDataStr()
{
	FString Result = FString::Printf(TEXT("True/%s/%s/%s/%s/%s/"),
	                                 m_PackagePurchased[0] ? TEXT("True") : TEXT("False"),
	                                 m_PackagePurchased[1] ? TEXT("True") : TEXT("False"),
	                                 m_PackagePurchased[2] ? TEXT("True") : TEXT("False"),
	                                 m_PackagePurchased[3] ? TEXT("True") : TEXT("False"),
	                                 m_PackagePurchased[4] ? TEXT("True") : TEXT("False"));

	return Result;
}


void UShopManager::ShowTouchBan()
{
	UDiabloGameInstance::Get->GetHud()->ShowTouchBan(2.f);
}
