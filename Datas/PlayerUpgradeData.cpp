#include "PlayerUpgradeData.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"

bool FSkillSpec::IsSkillUseable(ISkillUseCharacter* skillOwner)
{
	return m_fCurrentCD<=0.f && skillOwner->GetRage() >= m_SkillData->m_fRageCost;
}

float FSkillSpec::UseSkill(ISkillUseCharacter* skillOwner)
{
	FName SkillSection = m_SkillData->m_IDSkillMontageSection;
	
	if(SkillSection == NAME_None)
	{
		return 0.f;		
	}
	
	float SectionLen = skillOwner->PlaySkillMontageSection(SkillSection,m_SkillData->m_nSectionIndex,m_fCurrentCD,m_SkillData->m_fSkillCoolTime);

	return SectionLen;
}
