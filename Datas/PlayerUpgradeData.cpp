#include "PlayerUpgradeData.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"

bool FSkillSpec::IsSkillUseable()
{
	return m_fCurrentCD<=0.f && UDiabloGameInstance::Get->GetPlChar()->GetRage() >= m_SkillData->m_fRageCost;
}

float FSkillSpec::UseSkill()
{
	FName SkillSection = m_SkillData->m_IDSkillMontageSection;
	
	if(SkillSection == NAME_None)
	{
		PRINTF("SkillData MongrageSectionNull");
		
		return 0.f;		
	}
	
	float SectionLen = UDiabloGameInstance::Get->GetPlChar()->PlaySkillMontageSection(SkillSection,m_SkillData->m_nSectionIndex,m_fCurrentCD,m_SkillData->m_fSkillCoolTime);

	return SectionLen;
}
