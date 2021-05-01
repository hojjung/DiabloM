#include "AutoSkillUse.h"

#include "Interface/SkillUseCharacter.h"

FSkillSpec* UAutoSkillUse::GetUsableSkill(ISkillUseCharacter* owner, TArray<FSkillSpec*>& skillSpecs,
                                          float gainRagePerSec)
{
	FSkillSpec* FutureWantUse = nullptr;

	FSkillSpec* WantUse = nullptr;

	FSkillSpec* LongestCooltime = nullptr;

	for (FSkillSpec* Skill : skillSpecs)
	{
		float RemainCooltime = Skill->m_fCurrentCD - Skill->m_SkillData->m_fSkillCoolTime;

		float FutureRage = (RemainCooltime * gainRagePerSec) + owner->GetRage();

		bool bIsUsableNow = Skill->IsSkillUseable(owner);

		bool bIsCooldownReady = Skill->IsCooldownReady();

		bool bIsEnoughRage = owner->GetRage() >= Skill->m_SkillData->m_fRageCost;

		bool bIsUsableForFuture = !bIsEnoughRage && Skill->m_SkillData->m_fRageCost <= FutureRage;

		if (bIsCooldownReady)
		{
			if (LongestCooltime) //이미 있으면
			{
				if (LongestCooltime->m_SkillData->m_fSkillCoolTime > Skill->m_SkillData->m_fSkillCoolTime)
				{
					continue; //새로운 스킬로 교체하고 싶은데 쿨타임이 더짧다면 걍 지나감
				}
			}
			LongestCooltime = Skill;
		}
	}

	if (LongestCooltime)
	{
		bool bIsEnoughRage = owner->GetRage() >= LongestCooltime->m_SkillData->m_fRageCost;

		if(bIsEnoughRage)
		{
			return LongestCooltime;
		}
	}

	return nullptr;
}
