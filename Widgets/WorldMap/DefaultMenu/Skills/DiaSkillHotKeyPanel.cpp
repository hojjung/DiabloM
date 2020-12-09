// My First Hack n Slash


#include "DiaSkillHotKeyPanel.h"

void UDiaSkillHotKeyPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    m_AryButtons.Empty();
    m_AryButtons.Add(m_SkillBtn1);
    m_AryButtons.Add(m_SkillBtn2);
    m_AryButtons.Add(m_SkillBtn3);
    m_AryButtons.Add(m_SkillBtn4);
}
