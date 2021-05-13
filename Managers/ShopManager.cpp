#include "ShopManager.h"
#include "DiabloGameInstance.h"
#include "JsonSerializer.h"
#include "PlayFabJsonObject.h"
#include "Widgets/GachaMenu/GachaPanel.h"
#include "Widgets/HUD/MainCanvas.h"

void UShopManager::SetShopDataFromServer(const UPlayFabJsonObject* iapJsonStr)
{
	m_bUsingAutoSkill=false;
	
	m_PackagePurchased.Init(false, 5);
	m_PackagePurchased[0] = iapJsonStr->GetBoolField(TEXT("Package01"));
	m_PackagePurchased[1] = iapJsonStr->GetBoolField(TEXT("Package02"));
	m_PackagePurchased[2] = iapJsonStr->GetBoolField(TEXT("Package03"));
	m_PackagePurchased[3] = iapJsonStr->GetBoolField(TEXT("Package04"));
	m_PackagePurchased[4] = iapJsonStr->GetBoolField(TEXT("Package05"));
	
	UDiabloGameInstance::Get->m_NormalDgManager->m_OnDungeonMaxUpdate.AddUObject(this, &UShopManager::UpdateGold);
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
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientGemstone(30))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(30, TEXT("gold01"));
}

void UShopManager::PurchaseGold02()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientGemstone(300))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(300, TEXT("gold02"));
}

void UShopManager::PurchaseGold03()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientGemstone(3000))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(3000, TEXT("gold03"));
}

void UShopManager::PurchaseGachaWeapon01()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientGemstone(30))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(30, TEXT("gachaweapon01"));
}

void UShopManager::PurchaseGachaWeapon11()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientGemstone(300))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(300, TEXT("gachaweapon11"));
}

void UShopManager::PurchaseGachaWeapon55()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientGemstone(1500))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(1500, TEXT("gachaweapon55"));
}

void UShopManager::PurchaseGachaSkin01()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientGemstone(30))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(30, TEXT("gachaskin01"));
}

void UShopManager::PurchaseGachaSkin11()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientGemstone(300))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(300, TEXT("gachaskin11"));
}

void UShopManager::PurchaseGachaSkin55()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientGemstone(1500))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(1500, TEXT("gachaskin55"));
}

void UShopManager::PurchaseGachaPet01()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientPetTicket(1))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithTicket(1);
}

void UShopManager::PurchaseGachaPet11()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientPetTicket(11))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithTicket(11);
}

void UShopManager::PurchaseGachaPet55()
{
	
}

void UShopManager::PurchaseGachaAccessory01()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientGemstone(270))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(270, TEXT("gachaaccessory01"));
}

void UShopManager::PurchaseGachaAccessory11()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientGemstone(2700))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(2700, TEXT("gachaaccessory11"));
}

void UShopManager::PurchaseGachaAccessory55()
{
	if(!UDiabloGameInstance::Get->m_PlayfabManager->CheckClientGemstone(13500))
	{
		return;
	}
	
	ShowTouchBan();
	UDiabloGameInstance::Get->m_PlayfabManager->PurchaseWithGemStone(13500, TEXT("gachaaccessory55"));
}

void UShopManager::UpdateGold()
{
	BigInt Bounty = UDiabloGameInstance::Get->m_NormalDgManager->GetCurrentDungeonBounty();

	Bounty.Multiply(3500);

	m_Gold01 = Bounty;

	Bounty.Multiply(15);

	m_Gold02 = Bounty;

	Bounty.Multiply(20);

	m_Gold03 = Bounty;

	m_OnUpdateShopGold.Broadcast(m_Gold01, m_Gold02, m_Gold03);
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
		UDiabloGameInstance::Get->GetHud()->m_Canvas->m_PanelGacha->RollGachaWeaponOneTime();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaweapon11"))
	{
		UDiabloGameInstance::Get->GetHud()->m_Canvas->m_PanelGacha->RollGachaWeaponElevenTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaweapon55"))
	{
		UDiabloGameInstance::Get->GetHud()->m_Canvas->m_PanelGacha->RollGachaWeaponFiftyTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaskin01"))
	{
		UDiabloGameInstance::Get->GetHud()->m_Canvas->m_PanelGacha->RollGachaSkinOneTime();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaskin11"))
	{
		UDiabloGameInstance::Get->GetHud()->m_Canvas->m_PanelGacha->RollGachaSkinElevenTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaskin55"))
	{
		UDiabloGameInstance::Get->GetHud()->m_Canvas->m_PanelGacha->RollGachaSkinFiftyTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachapet01"))
	{
		UDiabloGameInstance::Get->GetHud()->m_Canvas->m_PanelGacha->RollGachaPetOneTime();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachapet11"))
	{
		UDiabloGameInstance::Get->GetHud()->m_Canvas->m_PanelGacha->RollGachaPetElevenTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachapet55"))
	{
		UDiabloGameInstance::Get->GetHud()->m_Canvas->m_PanelGacha->RollGachaPetFiftyTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaaccessory01"))
	{
		UDiabloGameInstance::Get->GetHud()->m_Canvas->m_PanelGacha->RollGachaAccessoryOneTime();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaaccessory11"))
	{
		UDiabloGameInstance::Get->GetHud()->m_Canvas->m_PanelGacha->RollGachaAccessoryElevenTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}
	else if (itemID == TEXT("gachaaccessory55"))
	{
		UDiabloGameInstance::Get->GetHud()->m_Canvas->m_PanelGacha->RollGachaAccessoryFiftyTimes();
		UDiabloGameInstance::Get->m_PlayfabManager->RequestGetInventory();
	}

	if(updateData)
	{
		UDiabloGameInstance::Get->m_PlayfabManager->UploadMainData();
	}
}

#pragma endregion IAP

bool UShopManager::GetPackagePurchased(int index)
{
	return m_PackagePurchased[index];
}

void UShopManager::SetIAPDataToJson(UPlayFabJsonObject* obj)
{
	UPlayFabJsonObject* JsonObj = UPlayFabJsonObject::ConstructJsonObject(this);

	JsonObj->SetBoolField(TEXT("Package01"), m_PackagePurchased[0]);
	JsonObj->SetBoolField(TEXT("Package02"), m_PackagePurchased[1]);
	JsonObj->SetBoolField(TEXT("Package03"), m_PackagePurchased[2]);
	JsonObj->SetBoolField(TEXT("Package04"), m_PackagePurchased[3]);
	JsonObj->SetBoolField(TEXT("Package05"), m_PackagePurchased[4]);
	//
	obj->SetObjectField(TEXT("IAP"),JsonObj);
}

void UShopManager::ShowTouchBan()
{
	UDiabloGameInstance::Get->GetHud()->ShowTouchBan(2.f);
}

bool UShopManager::IsAbleToUseAutoSkill()
{
	return m_PackagePurchased[0];
}

void UShopManager::SetUseAutoSkill(bool bUse)
{
	m_bUsingAutoSkill = bUse;
}
