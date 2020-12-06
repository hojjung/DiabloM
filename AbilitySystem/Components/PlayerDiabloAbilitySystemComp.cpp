#include "PlayerDiabloAbilitySystemComp.h"

UPlayerDiabloAbilitySystemComp::UPlayerDiabloAbilitySystemComp()
{
	m_SkillDataTableRow=nullptr;
	m_AryBaseSkill.Reset();
	m_AryPowerSkill.Reset();
	m_AryDefensvieSkill.Reset();
	m_ArySpecialSkill.Reset();
	m_AryMasterySkill.Reset();
	m_AryUltimateSkill.Reset();
	m_nSkillPoints=10;//Test
	m_nTotalSkillPointSpents=m_nSkillPoints;
}

void UPlayerDiabloAbilitySystemComp::CreateClassSkillSpecs(const FSkillDataHandle& skillDataHandle)
{
	m_SkillDataTableRow = skillDataHandle.GetRow<FSkillDataRow>("SkillDataNotFound-PlayerGASComp");

	check(m_SkillDataTableRow);

	for(const FSkillData& SkillData : m_SkillDataTableRow->m_AryBaseSkillBelt)
	{
		m_AryBaseSkill.Emplace(FSkillDataSpec(0,&SkillData));
	}

	for(const FSkillData& SkillData : m_SkillDataTableRow->m_AryDefensiveSkillBelt)
	{
		m_AryDefensvieSkill.Emplace(FSkillDataSpec(0,&SkillData));
	}

	for(const FSkillData& SkillData : m_SkillDataTableRow->m_AryMasterySkillBelt)
	{
		m_AryMasterySkill.Emplace(FSkillDataSpec(0,&SkillData));
	}
	for(const FSkillData& SkillData : m_SkillDataTableRow->m_AryPowerSkillBelt)
	{
		m_AryPowerSkill.Emplace(FSkillDataSpec(0,&SkillData));
	}

	for(const FSkillData& SkillData : m_SkillDataTableRow->m_ArySpecialSkillBelt)
	{
		m_ArySpecialSkill.Emplace(FSkillDataSpec(0,&SkillData));
	}

	for(const FSkillData& SkillData : m_SkillDataTableRow->m_AryUltimateSkillBelt)
	{
		m_AryUltimateSkill.Emplace(FSkillDataSpec(0,&SkillData));
	}
	
}

int UPlayerDiabloAbilitySystemComp::GetSkillPoints()
{
	return m_nSkillPoints;
}

void UPlayerDiabloAbilitySystemComp::LevelupSkill(FSkillDataSpec* skillSpec)
{
	PRINTF("Skill Learn Pressed");
	//스킬포인트 체크
	//실제 레벨업
	//현재 장착된 스킬 업데이트
	//위젯 업데이트
	//저장
}
