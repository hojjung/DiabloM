#include "PlayerUpgradeManager.h"
#include "DiabloGameInstance.h"
#include "Characters/PlayerDiabloCharacter.h"

#define LOCTEXT_NAMESPACE "PlayerUpgradeManager"

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

void UPlayerUpgradeManager::SetUpgradeDataFromServer(const FString& stat, const FString& skill)
{
	m_PlayfabManager = UDiabloGameInstance::Get->m_PlayfabManager;
	//
	TArray<FString> AryStat;
	stat.ParseIntoArray(AryStat,TEXT(":"));

	TArray<FString> ArySkill;
	skill.ParseIntoArray(ArySkill,TEXT("/"));
	//
	m_AryBaseAtkUpgrade.Init(FUpgradeSpec(),(int)EAttackType::Length);
	//	
	m_AryBaseAtkUpgrade[(int)EAttackType::BaseAttack].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>("AtkDmg01", ""); //
	m_AryBaseAtkUpgrade[(int)EAttackType::BaseAttack].SetLevel(FCString::Atoi(*AryStat[(int)EAttackType::BaseAttack]));

	m_AryBaseAtkUpgrade[(int)EAttackType::Critical].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>("AtkCri01", "");
	m_AryBaseAtkUpgrade[(int)EAttackType::Critical].SetLevel(FCString::Atoi(*AryStat[(int)EAttackType::Critical]));

	m_AryBaseAtkUpgrade[(int)EAttackType::CriticalDmg].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>("AtkCDmg01", "");
	m_AryBaseAtkUpgrade[(int)EAttackType::CriticalDmg].SetLevel(FCString::Atoi(*AryStat[(int)EAttackType::CriticalDmg]));

	m_AryBaseAtkUpgrade[(int)EAttackType::SuperCritical].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>("AtkCri02", "");
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperCritical].SetLevel(FCString::Atoi(*AryStat[(int)EAttackType::SuperCritical]));

	m_AryBaseAtkUpgrade[(int)EAttackType::SuperCriticalDmg].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>("AtkCDmg02", "");
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperCriticalDmg].SetLevel(FCString::Atoi(*AryStat[(int)EAttackType::SuperCriticalDmg]));

	m_AryBaseAtkUpgrade[(int)EAttackType::MagicBomb].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>("AtkMagic01", "");
	m_AryBaseAtkUpgrade[(int)EAttackType::MagicBomb].SetLevel(FCString::Atoi(*AryStat[(int)EAttackType::MagicBomb]));
	
	m_AryBaseAtkUpgrade[(int)EAttackType::MagicBombDmg].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>("AtkMDmg01", "");
	m_AryBaseAtkUpgrade[(int)EAttackType::MagicBombDmg].SetLevel(FCString::Atoi(*AryStat[(int)EAttackType::MagicBombDmg]));
	
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBomb].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>("AtkMagic02", "");
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBomb].SetLevel(FCString::Atoi(*AryStat[(int)EAttackType::MagicBomb]));
	
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBombDmg].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>("AtkMDmg02", "");
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBombDmg].SetLevel(FCString::Atoi(*AryStat[(int)EAttackType::SuperMagicBombDmg]));
	//
	m_AryUpgradeSkill.Init(FSkillSpec(), (int)ESkillType::Length);

	m_AryUpgradeSkill[(int)ESkillType::MiniSlash].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>("Skill01", "");
	m_AryUpgradeSkill[(int)ESkillType::MiniSlash].ParseFromString(ArySkill[(int)ESkillType::MiniSlash]);

	m_AryUpgradeSkill[(int)ESkillType::MagicBlade].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>("Skill02", "");
	m_AryUpgradeSkill[(int)ESkillType::MagicBlade].ParseFromString(ArySkill[(int)ESkillType::MagicBlade]);

	m_AryUpgradeSkill[(int)ESkillType::WhirlWind].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>("Skill03", "");
	m_AryUpgradeSkill[(int)ESkillType::WhirlWind].ParseFromString(ArySkill[(int)ESkillType::WhirlWind]);

	m_AryUpgradeSkill[(int)ESkillType::DeathBlow].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>("Skill04", "");
	m_AryUpgradeSkill[(int)ESkillType::DeathBlow].ParseFromString(ArySkill[(int)ESkillType::DeathBlow]);

	m_AryUpgradeSkill[(int)ESkillType::WindBlade].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>("Skill05", "");
	m_AryUpgradeSkill[(int)ESkillType::WindBlade].ParseFromString(ArySkill[(int)ESkillType::WindBlade]);
	//
	m_AryEquippedSkillSpec.Init(nullptr,4);

	for(FSkillSpec& Skill : m_AryUpgradeSkill)
	{
		if(Skill.m_nIndex>-1)
		{
			m_AryEquippedSkillSpec[Skill.m_nIndex] = &Skill;
		}
	}
	
}

void UPlayerUpgradeManager::UpgradeAtk(EAttackType type)
{
	if (!GetAtkUp(type).IsUpgradeAble())
	{
		return;
	}
	//
	GetAtkUp(type).IncreaseLevel();
	m_OnUpgradeChanged.Broadcast();
	//
	
}

void UPlayerUpgradeManager::UpgradeSkill(ESkillType type)
{
	if (!GetSkillUp(type).IsUpgradeAble())
	{
		return;
	}
	//
	GetSkillUp(type).IncreaseLevel();
	m_OnUpgradeChanged.Broadcast();
}

void UPlayerUpgradeManager::EquipSkill(int index, FSkillSpec* skill_spec)
{
	if(m_AryEquippedSkillSpec[index]&&	!m_AryEquippedSkillSpec[index]->IsCooldownReady())
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("EquipSkillFail","Cooldown Skill Cant Change1!"));
		return;
	}

	if( skill_spec&&	!skill_spec->IsCooldownReady())
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("EquipSkillFail2","Cooldown Skill Cant Change2!"));
		return;
	}

	
	if (skill_spec->m_nIndex > -1)
	{
		int Index=skill_spec->m_nIndex;
		m_AryEquippedSkillSpec[Index] = nullptr;
		m_OnSkillChanged.Broadcast(Index,nullptr);
		skill_spec->m_nIndex = -1;
	}
	
	m_AryEquippedSkillSpec[index] = skill_spec;
	m_AryEquippedSkillSpec[index]->m_nIndex = index;

	m_OnSkillChanged.Broadcast(index, skill_spec);
}

void UPlayerUpgradeManager::UnequipSkill(int index, FSkillSpec* skill_spec)
{
	m_AryEquippedSkillSpec[index] = nullptr;
	skill_spec->m_nIndex = -1;
	m_OnSkillChanged.Broadcast(index, nullptr);
}

bool UPlayerUpgradeManager::UseSkill(int index)
{
	if (!m_AryEquippedSkillSpec[index])
	{
		return false;
	}

	if (!m_AryEquippedSkillSpec[index]->IsSkillUseable())
	{
		return false;
	}
	
	m_fCastTime = m_AryEquippedSkillSpec[index]->UseSkill();

	m_CurrentCastingSkill = m_AryEquippedSkillSpec[index];

	UDiabloGameInstance::Get->GetPlChar()->SpendRagePoint(m_CurrentCastingSkill->m_SkillData->m_fRageCost);

	m_OnSkillUse.Broadcast(index,m_AryEquippedSkillSpec[index]);

	return true;
}

void UPlayerUpgradeManager::Tick(float deltaTime)
{
	for (int i = 0; i < 4; i++)
	{
		if(!m_AryEquippedSkillSpec[i])
		{
			continue;
		}
		
		m_AryEquippedSkillSpec[i]->Tick(deltaTime);

		//m_OnSkillTick.Broadcast(i, m_AryEquippedSkillSpec[i]);
	}

	if(!m_CurrentCastingSkill)
	{
		return;
	}

	m_fCastTime-=deltaTime;

	if(m_fCastTime<=0.f)
	{
		m_fCastTime = -1;
		m_CurrentCastingSkill = nullptr;
	}
}

bool UPlayerUpgradeManager::IsSkillCasting()
{
	//for prevent move and base attack
	return m_CurrentCastingSkill;
}
#undef LOCTEXT_NAMESPACE
