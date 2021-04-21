#include "ShopManager.h"
#include "Managers/DungeonManager.h"
#include "DiabloGameInstance.h"
#include "JsonSerializer.h"
#include "Widgets/GachaMenu/GachaPanel.h"

void UShopManager::SetShopDataFromServer(const FString iapJsonStr)
{
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(iapJsonStr);

	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;
	}

	m_PackagePurchased.Init(false, 5);

	m_PackagePurchased[0] = JsonObject->GetBoolField(TEXT("Package01"));
	m_PackagePurchased[1] = JsonObject->GetBoolField(TEXT("Package02"));
	m_PackagePurchased[2] = JsonObject->GetBoolField(TEXT("Package03"));
	m_PackagePurchased[3] = JsonObject->GetBoolField(TEXT("Package04"));
	m_PackagePurchased[4] = JsonObject->GetBoolField(TEXT("Package05"));
	//젬스톤 구매 골드가 플레이어 던전 최고레벨에 비례해서올라감
	UDiabloGameInstance::Get->m_DungeonManager->m_OnDungeonMaxUpdate.AddUObject(this, &UShopManager::UpdateGold);
}

#pragma region IAP

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

	m_OnUpdateShopGold.Broadcast(m_Gold01, m_Gold02, m_Gold03);
}

void UShopManager::RollGachaOneTime()
{
	// ERollItemType RollType = m_GachaPanel->m_GachaGridPanel->GetCurrentType();
	//
	// switch (RollType)
	// {
	// case RollWeapon: PurchaseGachaWeapon01();
	// 	break;
	// case RollSkin: PurchaseGachaSkin01();
	// 	break;
	// case RollPet: PurchaseGachaPet01();
	// 	break;
	// case RollAccessory: PurchaseGachaAccessory01();
	// 	break;
	// }
}

void UShopManager::RollGachaElevenTime()
{
	// ERollItemType RollType = m_GachaPanel->m_GachaGridPanel->GetCurrentType();
	//
	// switch (RollType)
	// {
	// case RollWeapon: PurchaseGachaWeapon11();
	// 	break;
	// case RollSkin: PurchaseGachaSkin11();
	// 	break;
	// case RollPet: PurchaseGachaPet11();
	// 	break;
	// case RollAccessory: PurchaseGachaAccessory11();
	// 	break;
	// }
}

void UShopManager::RollGachaFiftyTime()
{
	// ERollItemType RollType = m_GachaPanel->m_GachaGridPanel->GetCurrentType();
	//
	// switch (RollType)
	// {
	// case RollWeapon: PurchaseGachaWeapon55();
	// 	break;
	// case RollSkin: PurchaseGachaSkin55();
	// 	break;
	// case RollPet: PurchaseGachaPet55();
	// 	break;
	// case RollAccessory: PurchaseGachaAccessory55();
	// 	break;
	// }
}

void UShopManager::OnPurchasedGainItem(FString itemID,bool updateData)
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
		//UDiabloGameInstance::Get->m_GachaManager->RollGachaWeaponOneTime();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaweapon11"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaweapon55"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaskin01"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaskin11"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaskin55"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachapet01"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachapet11"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachapet55"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaaccessory01"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaaccessory11"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaaccessory55"))
	{
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}

	if(updateData)
	{
		UDiabloGameInstance::Get->m_PlayfabManager->UploadUserTitleData01();
	}
}

#pragma endregion IAP

bool UShopManager::GetPackagePurchased(int index)
{
	return m_PackagePurchased[index];
}

FString UShopManager::GetIAPDataStr()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetBoolField(TEXT("Package01"), m_PackagePurchased[0]);
	JsonObject->SetBoolField(TEXT("Package02"), m_PackagePurchased[1]);
	JsonObject->SetBoolField(TEXT("Package03"), m_PackagePurchased[2]);
	JsonObject->SetBoolField(TEXT("Package04"), m_PackagePurchased[3]);
	JsonObject->SetBoolField(TEXT("Package05"), m_PackagePurchased[4]);

	return PlayFab::FJsonKeeper(JsonObject).toJSONString();
}

void UShopManager::ShowTouchBan()
{
	UDiabloGameInstance::Get->GetHud()->ShowTouchBan(2.f);
}
