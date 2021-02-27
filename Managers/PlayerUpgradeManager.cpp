#include "PlayerUpgradeManager.h"
#include "DiabloGameInstance.h"

UDataTable* UPlayerUpgradeManager::StatUpgradeTable = nullptr;

UDataTable* UPlayerUpgradeManager::SkillUpgradeTable = nullptr;

UPlayerUpgradeManager::UPlayerUpgradeManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundSkillTable(
              TEXT("DataTable'/Game/DataTables/Upgrade/PlayerDefaultSkillTable.PlayerDefaultSkillTable'"));
	SkillUpgradeTable = FoundSkillTable.Object;
	
	 	static ConstructorHelpers::FObjectFinder<UDataTable> FoundStatTable(
	              TEXT("DataTable'/Game/DataTables/Upgrade/PlayerDefaultUpgradeTable.PlayerDefaultUpgradeTable'"));
	StatUpgradeTable = FoundStatTable.Object;
}

void UPlayerUpgradeManager::SetUpgradeDataFromServer(const FString& stat,const FString& skill)
{
	m_PlayfabManager = UDiabloGameInstance::Get->m_PlayfabManager;
	//
	TArray<FString> AryStat;
	stat.ParseIntoArray(AryStat,TEXT(":"));

	TArray<FString> ArySkill;
	skill.ParseIntoArray(ArySkill,TEXT(":"));
	//	
	m_UpgradeAtkDmg01.m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>("AtkDmg01",""); //
	m_UpgradeAtkDmg01.SetLevel(FCString::Atoi(*AryStat[0]));
	
	m_UpgradeAtkCri01.m_UpgradeData =  StatUpgradeTable->FindRow<FUpgradeDataRow>("AtkCri01","");
	m_UpgradeAtkCri01.SetLevel(FCString::Atoi(*AryStat[1]));
	
	m_UpgradeAtkCDmg01.m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>("AtkCDmg01","");
	m_UpgradeAtkCDmg01.SetLevel(FCString::Atoi(*AryStat[2]));

	m_UpgradeSkill01.m_UpgradeData = SkillUpgradeTable->FindRow<FUpgradeDataRow>("Skill01","");
	m_UpgradeSkill01.SetLevel(FCString::Atoi(*ArySkill[0]));
	
	m_UpgradeSkill02.m_UpgradeData = SkillUpgradeTable->FindRow<FUpgradeDataRow>("Skill02","");
	m_UpgradeSkill02.SetLevel(FCString::Atoi(*ArySkill[1]));
	
	m_UpgradeSkill03.m_UpgradeData = SkillUpgradeTable->FindRow<FUpgradeDataRow>("Skill03","");
	m_UpgradeSkill03.SetLevel(FCString::Atoi(*ArySkill[2]));
	//
}

void UPlayerUpgradeManager::UpgradeAtkDmg01()
{
	if (m_UpgradeAtkDmg01.m_nLv >= m_UpgradeAtkDmg01.GetMaxLv())
	{
		PRINTF("FAIL-UpgradeAtkDmg01-MaxLevel");
		return;
	}
	//
	m_UpgradeAtkDmg01.IncreaseLevel();
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
	m_UpgradeAtkCri01.IncreaseLevel();
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
	m_UpgradeAtkCDmg01.IncreaseLevel();
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
	m_UpgradeSkill01.IncreaseLevel();
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
	m_UpgradeSkill02.IncreaseLevel();
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
	m_UpgradeSkill03.IncreaseLevel();
	m_OnUpgradeChanged.Broadcast();
}

