#include "PlayerUpgradeManager.h"

#include "DiabloGameInstance.h"


void UPlayerUpgradeManager::SetUpgradeDataFromServer(const FString& stat,const FString& skill)
{
	m_PlayfabManager = plMan;

	FName id = *UPlayfabManager::PlayerAtkDmg01Key;
	m_UpgradeAtkDmg01.m_UpgradeData = UPlayerUpgradeData::GetPlUpgradeDataPtr(id); //
	id = *UPlayfabManager::PlayerAtkCri01Key;
	m_UpgradeAtkCri01.m_UpgradeData =  UPlayerUpgradeData::GetPlUpgradeDataPtr(id);
	id = *UPlayfabManager::PlayerAtkCDmg01Key;
	m_UpgradeAtkCDmg01.m_UpgradeData = UPlayerUpgradeData::GetPlUpgradeDataPtr(id);
	//
	id = *UPlayfabManager::PlayerSkill01Key;
	m_UpgradeSkill01.m_UpgradeData = UPlayerUpgradeData::GetSkillUpgradeDataPtr(id);
	id = *UPlayfabManager::PlayerSkill02Key;
	m_UpgradeSkill02.m_UpgradeData = UPlayerUpgradeData::GetSkillUpgradeDataPtr(id);
	id = *UPlayfabManager::PlayerSkill03Key;
	m_UpgradeSkill03.m_UpgradeData = UPlayerUpgradeData::GetSkillUpgradeDataPtr(id);
	//
	SetPlAtkDmg01();
	SetPlAtkCri01();
	SetPlAtkCDmg01();
}


void UPlayerUpgradeManager::SetPlAtkDmg01()
{
	m_UpgradeAtkDmg01.SetLevel(m_PlayfabManager->m_nLoadedPlAtkDmg01);
}

void UPlayerUpgradeManager::SetPlAtkCri01()
{
	m_UpgradeAtkCri01.SetLevel(m_PlayfabManager->m_nLoadedPlAtkCri01);
}

void UPlayerUpgradeManager::SetPlAtkCDmg01()
{
	m_UpgradeAtkCDmg01.SetLevel(m_PlayfabManager->m_nLoadedPlAtkCDmg01);
}

void UPlayerUpgradeManager::SetPlSkill01()
{
	m_UpgradeSkill01.SetLevel(m_PlayfabManager->m_nLoadedPlSkill01);
}

void UPlayerUpgradeManager::SetPlSkill02()
{
	m_UpgradeSkill02.SetLevel(m_PlayfabManager->m_nLoadedPlSkill02);
}

void UPlayerUpgradeManager::SetPlSkill03()
{
	m_UpgradeSkill03.SetLevel(m_PlayfabManager->m_nLoadedPlSkill03);
}


void UPlayerUpgradeManager::UpgradeAtkDmg01()
{
	if (m_UpgradeAtkDmg01.m_nLv >= m_UpgradeAtkDmg01.GetMaxLv())
	{
		PRINTF("FAIL-UpgradeAtkDmg01-MaxLevel");
		return;
	}
	//
	m_PlayfabManager->m_nLoadedPlAtkDmg01++;
	m_UpgradeAtkDmg01.SetLevel(m_PlayfabManager->m_nLoadedPlAtkDmg01);
	m_OnUpgradeChanged.Broadcast();
}

void UPlayerUpgradeManager::UpgradeAtkCri01()
{
	if (m_UpgradeAtkCri01.m_nLv >= m_UpgradeAtkCri01.GetMaxLv())
	{
		PRINTF("FAIL-UpgradeAtkCri01-MaxLevel");
		return;
	}
	//
	m_PlayfabManager->m_nLoadedPlAtkCri01++;
	m_UpgradeAtkCri01.SetLevel(m_PlayfabManager->m_nLoadedPlAtkCri01);
	m_OnUpgradeChanged.Broadcast();
}

void UPlayerUpgradeManager::UpgradeAtkCDmg01()
{
	if (m_UpgradeAtkCDmg01.m_nLv >= m_UpgradeAtkCDmg01.GetMaxLv())
	{
		PRINTF("FAIL-UpgradeAtkCDmg01-MaxLevel");
		return;
	}
	//
	m_PlayfabManager->m_nLoadedPlAtkCDmg01++;
	m_UpgradeAtkCDmg01.SetLevel(m_PlayfabManager->m_nLoadedPlAtkCDmg01);
	m_OnUpgradeChanged.Broadcast();
}

void UPlayerUpgradeManager::UpgradeSkill01()
{
	if (m_UpgradeSkill01.m_nLv >= m_UpgradeSkill01.GetMaxLv())
	{
		PRINTF("FAIL-UpgradeSkill01-MaxLevel");
		return;
	}
	//
	m_PlayfabManager->m_nLoadedPlSkill01++;
	m_UpgradeSkill01.SetLevel(m_PlayfabManager->m_nLoadedPlSkill01);
	m_OnUpgradeChanged.Broadcast();
}

void UPlayerUpgradeManager::UpgradeSkill02()
{
	if (m_UpgradeSkill02.m_nLv >= m_UpgradeSkill02.GetMaxLv())
	{
		PRINTF("FAIL-UpgradeSkill01-MaxLevel");
		return;
	}
	//
	m_PlayfabManager->m_nLoadedPlSkill02++;
	m_UpgradeSkill02.SetLevel(m_PlayfabManager->m_nLoadedPlSkill02);
	m_OnUpgradeChanged.Broadcast();
}

void UPlayerUpgradeManager::UpgradeSkill03()
{
	if (m_UpgradeSkill03.m_nLv >= m_UpgradeSkill03.GetMaxLv())
	{
		PRINTF("FAIL-UpgradeSkill03-MaxLevel");
		return;
	}
	//
	m_PlayfabManager->m_nLoadedPlSkill03++;
	m_UpgradeSkill03.SetLevel(m_PlayfabManager->m_nLoadedPlSkill03);
	m_OnUpgradeChanged.Broadcast();
}

