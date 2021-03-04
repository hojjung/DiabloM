#include "SkillCastManager.h"

void USkillCastManager::StartUseSkill(USkillExecute* skillExe)
{
	if(m_CurrentCastingSkill)
	{
		return;
	}

	m_CurrentCastingSkill = skillExe;
	skillExe->CastStart(this);
	skillExe->m_OnSkillEnd.AddUObject(this,&USkillCastManager::OnEndCurrentSkill);
}

void USkillCastManager::OnEndCurrentSkill(USkillExecute* skillExe)
{
	skillExe->m_OnSkillEnd.RemoveAll(this);
	
	m_CurrentCastingSkill = nullptr;
}

void USkillCastManager::ForceEndCurrentSkill()
{
	if(!m_CurrentCastingSkill)
	{
		return;
	}

	m_CurrentCastingSkill->CastEnd();
}
