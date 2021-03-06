// My First Hack n Slash


#include "SkillHotkeyPanel.h"

#include "Managers/DiabloGameInstance.h"

void USkillHotkeyPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	m_BtnSkillUse1->Init(0);
	m_BtnSkillUse2->Init(1);
	m_BtnSkillUse3->Init(2);
	m_BtnSkillUse4->Init(3);

	m_ArySkillUse.Add(m_BtnSkillUse1);
	m_ArySkillUse.Add(m_BtnSkillUse2);
	m_ArySkillUse.Add(m_BtnSkillUse3);
	m_ArySkillUse.Add(m_BtnSkillUse4);

	UDiabloGameInstance::Get->m_PlayerUpgradeManager->m_OnSkillChanged.AddUObject(this,&USkillHotkeyPanel::OnUpdateSkill);
	UDiabloGameInstance::Get->m_PlayerUpgradeManager->m_OnSkillUse.AddUObject(this,&USkillHotkeyPanel::OnCooldownStart);

	int i =0;
	for(FSkillSpec* SkillSpec : UDiabloGameInstance::Get->m_PlayerUpgradeManager->GetAryEquippedSkill())
	{
		OnUpdateSkill(i,SkillSpec);

		i++;
	}
}


void USkillHotkeyPanel::PlayBlink()
{
	PlayAnimation(m_FlashAnim);
}

void USkillHotkeyPanel::OnUpdateSkill(int index, FSkillSpec* skill_spec)
{
	m_ArySkillUse[index]->SetSkillSpec(skill_spec);
}
void USkillHotkeyPanel::OnCooldownStart(int index, FSkillSpec* skill_spec)
{
	m_ArySkillUse[index]->TryStartCooldown();
}