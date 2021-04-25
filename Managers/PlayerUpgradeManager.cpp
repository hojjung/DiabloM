#include "PlayerUpgradeManager.h"
#include "DiabloGameInstance.h"
#include "JsonSerializer.h"
#include "PlayFabJsonObject.h"
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

void UPlayerUpgradeManager::SetUpgradeDataFromServer(const UPlayFabJsonObject* statJsonStr,const UPlayFabJsonObject* skillJsonStr)
{
	m_PlayfabManager = UDiabloGameInstance::Get->m_PlayfabManager;
	//
	m_AryBaseAtkUpgrade.Init(FUpgradeSpec(), (int)EAttackType::Length);
	m_AryBaseAtkUpgrade[(int)EAttackType::BaseAttack].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>(TEXT("AtkDmg01"), ""); //
	m_AryBaseAtkUpgrade[(int)EAttackType::Critical].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>(TEXT("AtkCri01"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::CriticalDmg].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>(TEXT("AtkCDmg01"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperCritical].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>(TEXT("AtkCri02"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperCriticalDmg].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>(TEXT("AtkCDmg02"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::MagicBomb].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>(TEXT("AtkMagic01"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::MagicBombDmg].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>(TEXT("AtkMDmg01"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBomb].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow>(TEXT("AtkMagic02"), "");
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBombDmg].m_UpgradeData = StatUpgradeTable->FindRow<FUpgradeDataRow	>(TEXT("AtkMDmg02"), "");
	m_AryUpgradeSkill.Init(FSkillSpec(), (int)ESkillType::Length);
	m_AryUpgradeSkill[(int)ESkillType::DeathBlow].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>(TEXT("Skill01"), "");
	m_AryUpgradeSkill[(int)ESkillType::MagicBlade].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>(TEXT("Skill02"), "");
	m_AryUpgradeSkill[(int)ESkillType::WhirlWind].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow	>(TEXT("Skill03"), "");
	m_AryUpgradeSkill[(int)ESkillType::EarthQuake].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>(TEXT("Skill04"), "");
	m_AryUpgradeSkill[(int)ESkillType::WindBlade].m_SkillData = SkillUpgradeTable->FindRow<FSkillUpgradeDataRow>(TEXT("Skill05"), "");
	//
	m_AryBaseAtkUpgrade[(int)EAttackType::BaseAttack].SetLevel(statJsonStr->GetNumberField(TEXT("BaseAttack")));
	m_AryBaseAtkUpgrade[(int)EAttackType::Critical].SetLevel(statJsonStr->GetNumberField(TEXT("Critical")));
	m_AryBaseAtkUpgrade[(int)EAttackType::CriticalDmg].SetLevel(statJsonStr->GetNumberField(TEXT("CriticalDmg")));
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperCritical].SetLevel(statJsonStr->GetNumberField(TEXT("SuperCritical")));
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperCriticalDmg].SetLevel(statJsonStr->GetNumberField(TEXT("SuperCriticalDmg")));
	m_AryBaseAtkUpgrade[(int)EAttackType::MagicBomb].SetLevel(statJsonStr->GetNumberField(TEXT("MagicBomb")));
	m_AryBaseAtkUpgrade[(int)EAttackType::MagicBombDmg].SetLevel(statJsonStr->GetNumberField(TEXT("MagicBombDmg")));
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBomb].SetLevel(statJsonStr->GetNumberField(TEXT("SuperMagicBomb")));
	m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBombDmg].SetLevel(statJsonStr->GetNumberField(TEXT("SuperMagicBombDmg")));
	//
	m_AryUpgradeSkill[(int)ESkillType::DeathBlow].InitSkillSpec(skillJsonStr->GetNumberField(TEXT("DeathBlow")),skillJsonStr->GetNumberField(TEXT("DeathBlowEquipSlot")));
	m_AryUpgradeSkill[(int)ESkillType::MagicBlade].InitSkillSpec(skillJsonStr->GetNumberField(TEXT("MagicBlade")),skillJsonStr->GetNumberField(TEXT("MagicBladeEquipSlot")));
	m_AryUpgradeSkill[(int)ESkillType::WhirlWind].InitSkillSpec (skillJsonStr->GetNumberField(TEXT("WhirlWind")),skillJsonStr->GetNumberField(TEXT("WhirlWindEquipSlot")));
	m_AryUpgradeSkill[(int)ESkillType::EarthQuake].InitSkillSpec(skillJsonStr->GetNumberField(TEXT("EarthQuake")),skillJsonStr->GetNumberField(TEXT("EarthQuakeEquipSlot")));
	m_AryUpgradeSkill[(int)ESkillType::WindBlade].InitSkillSpec (skillJsonStr->GetNumberField(TEXT("WindBlade")),skillJsonStr->GetNumberField(TEXT("WindBladeEquipSlot")));
	//
	m_AryEquippedSkillSpec.Init(nullptr, 4);

	for (FSkillSpec& Skill : m_AryUpgradeSkill)
	{
		if (Skill.m_nIndex > -1)
		{
			m_AryEquippedSkillSpec[Skill.m_nIndex] = &Skill;
		}
	}
}

FString UPlayerUpgradeManager::GetUpgradeDataStr()//9
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("BaseAttack"), m_AryBaseAtkUpgrade[(int)EAttackType::BaseAttack].m_nLv);
	JsonObject->SetNumberField(TEXT("Critical"), m_AryBaseAtkUpgrade[(int)EAttackType::Critical].m_nLv);
	JsonObject->SetNumberField(TEXT("CriticalDmg"), m_AryBaseAtkUpgrade[(int)EAttackType::CriticalDmg].m_nLv);
	JsonObject->SetNumberField(TEXT("SuperCritical"), m_AryBaseAtkUpgrade[(int)EAttackType::SuperCritical].m_nLv);
	JsonObject->SetNumberField(TEXT("SuperCriticalDmg"), m_AryBaseAtkUpgrade[(int)EAttackType::SuperCriticalDmg].m_nLv);
	JsonObject->SetNumberField(TEXT("MagicBomb"), m_AryBaseAtkUpgrade[(int)EAttackType::MagicBomb].m_nLv);
	JsonObject->SetNumberField(TEXT("MagicBombDmg"), m_AryBaseAtkUpgrade[(int)EAttackType::MagicBombDmg].m_nLv);
	JsonObject->SetNumberField(TEXT("SuperMagicBomb"), m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBomb].m_nLv);
	JsonObject->SetNumberField(TEXT("SuperMagicBombDmg"), m_AryBaseAtkUpgrade[(int)EAttackType::SuperMagicBombDmg].m_nLv);

	return PlayFab::FJsonKeeper(JsonObject).toJSONString();
}

FString UPlayerUpgradeManager::GetSkillDataStr()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("DeathBlow"),m_AryUpgradeSkill[(int)ESkillType::DeathBlow].m_nLv);
	JsonObject->SetNumberField(TEXT("DeathBlowEquipSlot"), m_AryUpgradeSkill[(int)ESkillType::DeathBlow].m_nIndex);
	JsonObject->SetNumberField(TEXT("MagicBlade"), m_AryUpgradeSkill[(int)ESkillType::MagicBlade].m_nLv);
	JsonObject->SetNumberField(TEXT("MagicBladeEquipSlot"), m_AryUpgradeSkill[(int)ESkillType::MagicBlade].m_nIndex);
	JsonObject->SetNumberField(TEXT("WhirlWind"), m_AryUpgradeSkill[(int)ESkillType::WhirlWind].m_nLv);
	JsonObject->SetNumberField(TEXT("WhirlWindEquipSlot"), m_AryUpgradeSkill[(int)ESkillType::WhirlWind].m_nIndex);
	JsonObject->SetNumberField(TEXT("EarthQuake"), m_AryUpgradeSkill[(int)ESkillType::EarthQuake].m_nLv);
	JsonObject->SetNumberField(TEXT("EarthQuakeEquipSlot"), m_AryUpgradeSkill[(int)ESkillType::EarthQuake].m_nIndex);
	JsonObject->SetNumberField(TEXT("WindBlade"), m_AryUpgradeSkill[(int)ESkillType::WindBlade].m_nLv);
	JsonObject->SetNumberField(TEXT("WindBladeEquipSlot"), m_AryUpgradeSkill[(int)ESkillType::WindBlade].m_nIndex);

	return PlayFab::FJsonKeeper(JsonObject).toJSONString();
}

void UPlayerUpgradeManager::UpgradeAtk(EAttackType type)
{
	if (!GetAtkUp(type).IsUpgradeAble())
	{
		return;
	}
	//
	BigInt Cost = GetAtkUp(type).m_Cost;
	
	if(!UDiabloGameInstance::Get->m_GoldManager->SubtractGold(Cost))
	{
		return;
	}
	
	GetAtkUp(type).IncreaseLevel();
	
	m_OnUpgradeChanged.Broadcast();

	switch (type)
	{
	case EAttackType::BaseAttack: UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::AttackLv);
		break;
	case EAttackType::Critical: UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::CriLv);
		break;
	case EAttackType::CriticalDmg: UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::CriDmgLv);
		break;
	case EAttackType::SuperCritical: UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::SCriLv);
		break;
	case EAttackType::SuperCriticalDmg: UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::SCriDmgLv);
		break;
	case EAttackType::MagicBomb: UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::MaLv);
		break;
	case EAttackType::MagicBombDmg: UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::MaDmgLv);
		break;
	case EAttackType::SuperMagicBomb: UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::SMaLv);
		break;
	case EAttackType::SuperMagicBombDmg: UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::SMaDmgLv);
		break;
	}
}

void UPlayerUpgradeManager::UpgradeSkill(ESkillType type)
{
	if (!GetSkillUp(type).IsUpgradeAble())
	{
		return;
	}
	
	BigInt Cost = GetSkillUp(type).m_Cost;
	
	if(!UDiabloGameInstance::Get->m_GoldManager->SubtractGold(Cost))
	{
		return;
	}
	//
	GetSkillUp(type).IncreaseLevel();
	m_OnUpgradeChanged.Broadcast();
}

void UPlayerUpgradeManager::EquipSkill(int index, FSkillSpec* skill_spec)
{
	if (m_AryEquippedSkillSpec[index] && !m_AryEquippedSkillSpec[index]->IsCooldownReady())
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("EquipSkillFail", "스킬 쿨타임 중 변경 불가"));
		return;
	}

	if (skill_spec && !skill_spec->IsCooldownReady())
	{
		UDiabloGameInstance::Get->RequestPopupText(LOCTEXT("EquipSkillFail2", "스킬 쿨타임 중 변경 불가"));
		return;
	}


	if (skill_spec->m_nIndex > -1)
	{
		UnequipSkill(skill_spec->m_nIndex);
	}

	UnequipSkill(index);

	m_AryEquippedSkillSpec[index] = skill_spec;
	m_AryEquippedSkillSpec[index]->m_nIndex = index;

	m_OnSkillChanged.Broadcast(index, skill_spec);
}

void UPlayerUpgradeManager::UnequipSkill(int index)
{
	if (!m_AryEquippedSkillSpec[index])
	{
		return;
	}

	m_AryEquippedSkillSpec[index]->m_nIndex = -1;
	m_AryEquippedSkillSpec[index] = nullptr;
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

	m_OnSkillUse.Broadcast(index, m_AryEquippedSkillSpec[index]);

	return true;
}

void UPlayerUpgradeManager::Tick(float deltaTime)
{
	for (int i = 0; i < 4; i++)
	{
		if (!m_AryEquippedSkillSpec[i])
		{
			continue;
		}

		m_AryEquippedSkillSpec[i]->Tick(deltaTime);
	}

	if (!m_CurrentCastingSkill)
	{
		return;
	}

	m_fCastTime -= deltaTime;

	if (m_fCastTime <= 0.f)
	{
		m_fCastTime = -1;
		m_CurrentCastingSkill = nullptr;
	}
}

bool UPlayerUpgradeManager::IsSkillCasting()
{
	return m_CurrentCastingSkill;
}

bool UPlayerUpgradeManager::IsRootmotionSkillCasting()
{
	return m_CurrentCastingSkill && m_CurrentCastingSkill->m_SkillData->m_bIsRootmotion;
}

void UPlayerUpgradeManager::ClearCooldownAllSkill()
{
	for (int i = 0; i < 4; i++)
	{
		if (!m_AryEquippedSkillSpec[i])
		{
			continue;
		}

		m_AryEquippedSkillSpec[i]->m_fCurrentCD = -1.f;
	}
}



//1,1
#undef LOCTEXT_NAMESPACE
