#include "SaveSkill.h"
#include "SaveLoadManager.h"

void USaveSkill::SetSaveSkill(ESaveVersion saveVersion,int remainPoint,int spentPoint,TArray<FSkillDataSpec>& skill1, TArray<FSkillDataSpec>& skill2,
                              TArray<FSkillDataSpec>& skill3, TArray<FSkillDataSpec>& skill4, TArray<FSkillDataSpec>& skill5,
                              TArray<FSkillDataSpec>& skill6)
{
    m_SaveVersion=saveVersion;

    m_nRemainSkillPoint=remainPoint;

    m_nSpentSkillPoint=spentPoint;
    
    m_ArySkillDataSpec1=skill1;
    m_ArySkillDataSpec2=skill2;
    m_ArySkillDataSpec3=skill3;
    m_ArySkillDataSpec4=skill4;
    m_ArySkillDataSpec5=skill5;
    m_ArySkillDataSpec6=skill6;
    //
    ClearSkillAry(m_ArySkillDataSpec1);
    ClearSkillAry(m_ArySkillDataSpec2);
    ClearSkillAry(m_ArySkillDataSpec3);
    ClearSkillAry(m_ArySkillDataSpec4);
    ClearSkillAry(m_ArySkillDataSpec5);
    ClearSkillAry(m_ArySkillDataSpec6);
}

void USaveSkill::ClearSkillAry(TArray<FSkillDataSpec>& skill)
{
    for(FSkillDataSpec& Spec :skill)
    {
        Spec.m_LearnBtn=nullptr;
        Spec.m_SkillDataPtr=nullptr;
    }
}
