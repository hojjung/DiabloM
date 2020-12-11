#include "SaveTalent.h"


void USaveTalent::SetSaveTalent(ESaveVersion saveVersion, int remainPoint, int spentPoint,
    TArray<FTalentDataSpec>& talent1, TArray<FTalentDataSpec>& talent2)
{
    m_SaveVersion=saveVersion;
    m_nRemainSkillPoint=remainPoint;
    m_nSpentSkillPoint=spentPoint;
    m_AryTalentData1=talent1;
    m_AryTalentData2=talent2;
}
