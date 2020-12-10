#include "DiaSkillPanel.h"
#define LOCTEXT_NAMESPACE "DiaSkillPanel"
void UDiaSkillPanel::Init(UPlayerDiabloAbilitySystemComp* playerSkillComp)
{
	m_FormatSkillPoint =LOCTEXT("SkillPanelPointAvailable","Points Available:{0}");
	
	m_FormatTotalSkillPoint =LOCTEXT("SkillPanelPointTotal","Total Points Spent:{0}");
	
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
		m_BaseSkillBelt->AddChildToHorizontalBox(CreateSkillButton(SkillSpec,m_PlayerSkillComp));
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryPowerSkill)
	{
		m_PowerSkillBelt->AddChildToHorizontalBox(CreateSkillButton(SkillSpec,m_PlayerSkillComp));
	}
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryDefensvieSkill)
	{
		m_DefenseSkillBelt->AddChildToHorizontalBox(CreateSkillButton(SkillSpec,m_PlayerSkillComp));
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_ArySpecialSkill)
	{
		m_SpeicalSkillBelt->AddChildToHorizontalBox(CreateSkillButton(SkillSpec,m_PlayerSkillComp));
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryMasterySkill)
	{
		m_MasterySkillBelt->AddChildToHorizontalBox(CreateSkillButton(SkillSpec,m_PlayerSkillComp));
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryUltimateSkill)
	{
		m_UltimateSkillBelt->AddChildToHorizontalBox(CreateSkillButton(SkillSpec,m_PlayerSkillComp));
	}
	//
	UpdateAvailablePoint();
	UpdateTotalPoint();
	//LevelLoad and update
	m_PlayerSkillComp->m_OnSkillLevelChanged.AddUObject(this,&UDiaSkillPanel::UpdateAllWidgetWrap);
}

USkillLearnButton* UDiaSkillPanel::CreateSkillButton(FSkillDataSpec& skillSpec,UPlayerDiabloAbilitySystemComp* compDi)
{
	USkillLearnButton* SkillButtonCreated = CreateWidget<USkillLearnButton>(this, m_ClassSkillLearnButton);

	SkillButtonCreated->InitSkillButton(skillSpec,compDi);

	m_AryAllSkillLearnButton.Emplace(SkillButtonCreated);

	return SkillButtonCreated;
}

void UDiaSkillPanel::ResetSkillPoint()
{
	int LearnedLevel=0;
	//need limit here
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryBaseSkill)
	{
		m_PlayerSkillComp->UnequipSkill(&SkillSpec);
		LearnedLevel+=SkillSpec.m_nCurrentLevel;
		SkillSpec.m_nCurrentLevel=0;
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryPowerSkill)
	{
		m_PlayerSkillComp->UnequipSkill(&SkillSpec);
		LearnedLevel+=SkillSpec.m_nCurrentLevel;
		SkillSpec.m_nCurrentLevel=0;
	}
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryDefensvieSkill)
	{
		m_PlayerSkillComp->UnequipSkill(&SkillSpec);
		LearnedLevel+=SkillSpec.m_nCurrentLevel;
		SkillSpec.m_nCurrentLevel=0;
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_ArySpecialSkill)
	{
		m_PlayerSkillComp->UnequipSkill(&SkillSpec);
		LearnedLevel+=SkillSpec.m_nCurrentLevel;
		SkillSpec.m_nCurrentLevel=0;
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryMasterySkill)
	{
		m_PlayerSkillComp->UnequipSkill(&SkillSpec);
		LearnedLevel+=SkillSpec.m_nCurrentLevel;
		SkillSpec.m_nCurrentLevel=0;
	}
	//
	for(FSkillDataSpec& SkillSpec : m_PlayerSkillComp->m_AryUltimateSkill)
	{
		m_PlayerSkillComp->UnequipSkill(&SkillSpec);
		LearnedLevel+=SkillSpec.m_nCurrentLevel;
		SkillSpec.m_nCurrentLevel=0;
	}

	m_PlayerSkillComp->m_nSkillPoints+=LearnedLevel;
	m_PlayerSkillComp->m_nTotalSkillPointSpents=0;
	
	UpdateAllWidget();
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

void UDiaSkillPanel::UpdateAllWidgetWrap(FSkillDataSpec* notUse)
{
	UpdateAllWidget();
}

void UDiaSkillPanel::UpdateAllWidget(FSkillDataSpec* notUse,int notUseIndex)
{
	UpdateAvailablePoint();
	UpdateTotalPoint();

	for(USkillLearnButton* LBtn :GetAllSkillLearnBtn())
	{
		LBtn->UpdateLevelText();
		if(LBtn->GetSkillSpec()->m_nEquipIndex<0)
		{
			LBtn->SkillDeselected();	
		}
		else
		{
			LBtn->SkillSelected();
		}
	}
}
#undef LOCTEXT_NAMESPACE
