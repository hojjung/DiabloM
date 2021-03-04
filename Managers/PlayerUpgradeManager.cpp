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

	m_AryUpgradeSkill.Init(FSkillSpec(),4);

	m_AryUpgradeSkill[0].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>("Skill01","");
	m_AryUpgradeSkill[0].SetLevel(FCString::Atoi(*ArySkill[0]));
	
	m_AryUpgradeSkill[1].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>("Skill02","");
	m_AryUpgradeSkill[1].SetLevel(FCString::Atoi(*ArySkill[1]));
	
	m_AryUpgradeSkill[2].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>("Skill03","");
	m_AryUpgradeSkill[2].SetLevel(FCString::Atoi(*ArySkill[2]));
	//
	m_AryEquippedSkillSpec[0] = nullptr;
	m_AryEquippedSkillSpec[1] = nullptr;
	m_AryEquippedSkillSpec[2] = nullptr;
	m_AryEquippedSkillSpec[3] = nullptr;
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
	if (m_AryUpgradeSkill[0].m_nLv >= m_AryUpgradeSkill[0].GetMaxLv())
	{
		PRINTF("FAIL-UpgradeSkill01-MaxLevel");
		return;
	}
	//
	m_AryUpgradeSkill[0].IncreaseLevel();
	m_OnUpgradeChanged.Broadcast();
}

void UPlayerUpgradeManager::UpgradeSkill02()
{
	if (m_AryUpgradeSkill[1].m_nLv >= m_AryUpgradeSkill[1].GetMaxLv())
	{
		PRINTF("FAIL-UpgradeSkill01-MaxLevel");
		return;
	}
	//
	m_AryUpgradeSkill[1].IncreaseLevel();
	m_OnUpgradeChanged.Broadcast();
}

void UPlayerUpgradeManager::UpgradeSkill03()
{
	if (m_AryUpgradeSkill[2].m_nLv >= m_AryUpgradeSkill[2].GetMaxLv())
	{
		PRINTF("FAIL-UpgradeSkill03-MaxLevel");
		return;
	}
	//
	m_AryUpgradeSkill[2].IncreaseLevel();
	m_OnUpgradeChanged.Broadcast();
}

void UPlayerUpgradeManager::EquipSkill(int index, FSkillSpec* skill_spec)
{
	if(skill_spec->m_nIndex>-1)
	{
		int Index=skill_spec->m_nIndex; 
		m_OnSkillChanged.Broadcast(Index,nullptr);
		skill_spec->m_nIndex = -1;
	}
	m_AryEquippedSkillSpec[index] = skill_spec;
	
	m_OnSkillChanged.Broadcast(index,skill_spec);
}

void UPlayerUpgradeManager::UnequipSkill(int index, FSkillSpec* skill_spec)
{
	m_AryEquippedSkillSpec[index] = nullptr;
	skill_spec->m_nIndex = -1;
	m_OnSkillChanged.Broadcast(index,nullptr);
}

