#include "PlayerUpgradeManager.h"

#include "DiabloGameInstance.h"

#include "Datas/PlayerUpgradeData.h"

void UPlayerUpgradeManager::SetUpgradeDataFromServer(UPlayfabManager* plMan)
{
	m_bIsPlAtkDmg01Able=true;
	m_bIsPlAtkCri01Able=true;
	m_bIsPlAtkCDmg01Able=true;
	//
	m_PlayfabManager = plMan;

	FName id = *UPlayfabManager::PlayerAtkDmg01Key;
	m_PlAtkDmg01Upgrade = UPlayerUpgradeData::GetPlUpgradeDataPtr(id); //
	id = *UPlayfabManager::PlayerAtkCri01Key;
	m_PlAtkCri01Upgrade = UPlayerUpgradeData::GetPlUpgradeDataPtr(id);
	id = *UPlayfabManager::PlayerAtkCDmg01Key;
	m_PlAtkCDmg01Upgrade = UPlayerUpgradeData::GetPlUpgradeDataPtr(id);

	//
	SetPlAtkDmg01();
	SetPlAtkCri01();
	SetPlAtkCDmg01();
	SetPlAtkDmg01Cost();
	SetPlAtkCri01Cost();
	SetPlAtkCDmgCost();
}


void UPlayerUpgradeManager::SetPlAtkDmg01()
{
	m_nPlAtkDmg01Lv = m_PlayfabManager->GetPlAtkDmg01Lv();
	m_PlAtkDmg01 = m_PlAtkDmg01Upgrade->GetValue(m_nPlAtkDmg01Lv);
}

void UPlayerUpgradeManager::SetPlAtkCri01()
{
	m_nPlAtkCri01Lv = m_PlayfabManager->GetPlAtkCri01Lv();
	m_PlAtkCri01 = m_PlAtkCri01Upgrade->GetValue(m_nPlAtkCri01Lv);
}

void UPlayerUpgradeManager::SetPlAtkCDmg01()
{
	m_nPlAtkCDmg01Lv = m_PlayfabManager->GetPlAtkCDmg01Lv();
	m_PlAtkCDmg01 = m_PlAtkCDmg01Upgrade->GetValue(m_nPlAtkCDmg01Lv);
}

void UPlayerUpgradeManager::SetPlAtkDmg01Cost()
{
	m_PlAtkDmg01Cost = m_PlAtkDmg01Upgrade->GetCost(m_nPlAtkDmg01Lv);
}

void UPlayerUpgradeManager::SetPlAtkCri01Cost()
{
	m_PlAtkCri01Cost = m_PlAtkCri01Upgrade->GetCost(m_nPlAtkCri01Lv);
}

void UPlayerUpgradeManager::SetPlAtkCDmgCost()
{
	m_PlAtkCDmg01Cost = m_PlAtkCDmg01Upgrade->GetCost(m_nPlAtkCDmg01Lv);
}


BigInt& UPlayerUpgradeManager::GetPlAtkDmg01()
{
	return m_PlAtkDmg01;
}

BigInt& UPlayerUpgradeManager::GetPlAtkCri01()
{
	return m_PlAtkCri01;
}

BigInt& UPlayerUpgradeManager::GetPlAtkCDmg01()
{
	return m_PlAtkCDmg01;
}

BigInt& UPlayerUpgradeManager::GetPlAtkDmg01Cost()
{
	return m_PlAtkDmg01Cost;
}

BigInt& UPlayerUpgradeManager::GetPlAtkCri01Cost()
{
	return m_PlAtkCri01Cost;
}

BigInt& UPlayerUpgradeManager::GetPlAtkCDmg01Cost()
{
	return m_PlAtkCDmg01Cost;
}

//

void UPlayerUpgradeManager::UpgradeAtkDmg01()
{
	if(!m_bIsPlAtkDmg01Able)
	{
		return;
	}
	
	if (m_nPlAtkDmg01Lv >= m_PlAtkDmg01Upgrade->m_nMaxLevel)
	{
		PRINTF("FAIL-UpgradeAtkDmg01-MaxLevel");
		return;
	}
	//
	m_bIsPlAtkDmg01Able=false;

	FGetUsrDataReq Req;
	Req.PlayFabId = m_PlayfabManager->m_PlayfabID;

	if (!m_PlayfabManager->GetClientAPI)
	{
		m_nPlAtkDmg01Lv++;

		PRINTF("UpgradeSuccess-PlAtk01DmgL%d", m_nPlAtkDmg01Lv);

		m_OnUpgradeChanged.Broadcast();
		return;
	}
	m_PlayfabManager->GetClientAPI->GetUserData(Req,
	                                            FGetUsrDataDele::CreateUObject(
		                                            this, &UPlayerUpgradeManager::OnGetPlAtk01DmgSuccess),
	                                            FFailDele::CreateUObject(
		                                            this, &UPlayerUpgradeManager::OnErrorPlayfabReq));


	// RequestUpgradeAtkDmg01(
	// 	FUpdateDele::CreateUObject(this,&UPlayerUpgradeManager::OnPlAtk01DmgSuccess),
	// 	FFailDele::CreateUObject(this,&UPlayerUpgradeManager::OnPlAtk01DmgFail));
}

void UPlayerUpgradeManager::OnGetPlAtk01DmgSuccess(const FGetUsrDataRSlt& result)
{
	SetPlAtkDmg01();
	PRINTF("Reset PlAtk01Dmg");
	if (m_nPlAtkDmg01Lv >= m_PlAtkDmg01Upgrade->m_nMaxLevel)
	{
		PRINTF("FAIL2-UpgradeAtkDmg01-MaxLevel-HeSeemsHack");
		return;
	}

	FString NewUpgradeLevel = FString::FromInt(m_nPlAtkDmg01Lv + 1);

	FUpdateReq Req;

	Req.Data.Add(UPlayfabManager::PlayerAtkDmg01Key, NewUpgradeLevel);

	m_PlayfabManager->GetClientAPI->UpdateUserData(Req,
	                                               FUpdateDele::CreateUObject(
		                                               this, &UPlayerUpgradeManager::OnUpdatePlAtk01DmgSuccess),
	                                               FFailDele::CreateUObject(
		                                               this, &UPlayerUpgradeManager::OnErrorPlayfabReq));
}

void UPlayerUpgradeManager::OnUpdatePlAtk01DmgSuccess(const FUpdateRslt& result)
{
	if (m_nPlAtkDmg01Lv >= m_PlAtkDmg01Upgrade->m_nMaxLevel)
	{
		PRINTF("FAIL3-UpgradeAtkDmg01-MaxLevel-HeSeemsHack");
		return;
	}
	
	m_PlayfabManager->m_nLoadedPlAtkDmg01++;
	
	SetPlAtkDmg01();

	PRINTF("UpgradeSuccess-PlAtk01DmgL%d", m_nPlAtkDmg01Lv);

	m_OnUpgradeChanged.Broadcast();

	m_bIsPlAtkDmg01Able = true;
}

void UPlayerUpgradeManager::OnErrorPlayfabReq(const FFailRslt& errorResult)
{	
	PRINTF("PlayfabRequest Error Name:%s", *errorResult.ErrorName);
	PRINTF("PlayfabRequest Error Message:%s", *errorResult.ErrorMessage);
	PRINTF("PlayfabRequest Error Code:%s", *UPlayFabUtilities::getErrorText(errorResult.ErrorCode));
	m_bIsPlAtkDmg01Able = true;
}