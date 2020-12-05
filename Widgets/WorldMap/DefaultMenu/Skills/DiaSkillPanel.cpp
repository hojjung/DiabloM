#include "DiaSkillPanel.h"

void UDiaSkillPanel::Init(UPlayerDiabloAbilitySystemComp* playerSkillComp)
{
	m_FormatSkillPoint = FText::FromString("Points Available:{0}");
	
	m_FormatTotalSkillPoint = FText::FromString("Total Points Spent:{0}");
	
	m_PlayerSkillComp=playerSkillComp;

	m_TextBaseSkillName->SetText(m_PlayerSkillComp->GetSkillDataTableRow()->m_BaseSkillBeltName);
	m_TextPowerSkillName->SetText(m_PlayerSkillComp->GetSkillDataTableRow()->m_PowerSkillBeltName);
	m_TextDefensiveSkillName->SetText(m_PlayerSkillComp->GetSkillDataTableRow()->m_DefensiveSkillBeltName);
	m_TextSpecialSkillName->SetText(m_PlayerSkillComp->GetSkillDataTableRow()->m_SpecialSkillBeltName);
	m_TextMasterySkillName->SetText(m_PlayerSkillComp->GetSkillDataTableRow()->m_MasterySkillBeltName);
	m_TextUltimateSkillName->SetText(m_PlayerSkillComp->GetSkillDataTableRow()->m_UltimateSkillBeltName);

	m_ResetButton->OnClicked.AddDynamic(this,&UDiaSkillPanel::ResetSkillPoint);

	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryBaseSkill)
	{
		m_BaseSkillBelt->AddChildToHorizontalBox(CreateSkillButton(SkillSpec));
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryPowerSkill)
	{
		m_PowerSkillBelt->AddChildToHorizontalBox(CreateSkillButton(SkillSpec));
	}
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryDefensvieSkill)
	{
		m_DefenseSkillBelt->AddChildToHorizontalBox(CreateSkillButton(SkillSpec));
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_ArySpecialSkill)
	{
		m_SpeicalSkillBelt->AddChildToHorizontalBox(CreateSkillButton(SkillSpec));
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryMasterySkill)
	{
		m_MasterySkillBelt->AddChildToHorizontalBox(CreateSkillButton(SkillSpec));
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryUltimateSkill)
	{
		m_UltimateSkillBelt->AddChildToHorizontalBox(CreateSkillButton(SkillSpec));
	}
	//
	UpdateAvailablePoint();
	UpdateTotalPoint();
}

USkillLearnButton* UDiaSkillPanel::CreateSkillButton(FSkillDataSpec& skillSpec)
{
	USkillLearnButton* SkillButtonCreated = CreateWidget<USkillLearnButton>(this, m_ClassSkillLearnButton);

	SkillButtonCreated->InitSkillButton(skillSpec);

	return SkillButtonCreated;
}

void UDiaSkillPanel::ResetSkillPoint()
{
	int LearnedLevel=0;
	//need limit here
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryBaseSkill)
	{
		LearnedLevel+=SkillSpec.m_nCurrentLevel;
		SkillSpec.m_nCurrentLevel=0;
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryPowerSkill)
	{
		LearnedLevel+=SkillSpec.m_nCurrentLevel;
		SkillSpec.m_nCurrentLevel=0;
	}
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryDefensvieSkill)
	{
		LearnedLevel+=SkillSpec.m_nCurrentLevel;
		SkillSpec.m_nCurrentLevel=0;
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_ArySpecialSkill)
	{
		LearnedLevel+=SkillSpec.m_nCurrentLevel;
		SkillSpec.m_nCurrentLevel=0;
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryMasterySkill)
	{
		LearnedLevel+=SkillSpec.m_nCurrentLevel;
		SkillSpec.m_nCurrentLevel=0;
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryUltimateSkill)
	{
		LearnedLevel+=SkillSpec.m_nCurrentLevel;
		SkillSpec.m_nCurrentLevel=0;
	}

	m_PlayerSkillComp->m_nSkillPoints+=LearnedLevel;
	UpdateAvailablePoint();
}

void UDiaSkillPanel::UpdateAvailablePoint()
{
	FFormatOrderedArguments Args;

	Args.Add(m_PlayerSkillComp->m_nSkillPoints);
	
	FText Result = FText::Format(m_FormatSkillPoint, Args);
	
	m_TextPointAvailable->SetText(Result);
}

void UDiaSkillPanel::UpdateTotalPoint()
{
	FFormatOrderedArguments Args;

	Args.Add(m_PlayerSkillComp->m_nTotalSkillPointSpents);
	
	FText Result = FText::Format(m_FormatTotalSkillPoint, Args);
	
	m_TextPointTotalSpent->SetText(Result);
}
