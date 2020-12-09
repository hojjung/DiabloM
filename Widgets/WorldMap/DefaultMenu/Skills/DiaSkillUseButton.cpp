#include "DiaSkillUseButton.h"

#include "DiaDragDropSkill.h"
#include "SkillLearnButton.h"

UDiaSkillUseButton::UDiaSkillUseButton(const FObjectInitializer& objInit):Super(objInit)
{
	m_fMaxCD=0.f;
	m_GaSpec=nullptr;
	m_EquippedSkillSpec=nullptr;
	m_nIndex=-1;
}

void UDiaSkillUseButton::Init(UPlayerDiabloAbilitySystemComp* diaComp,int index)
{
	m_BtnSkill->OnClicked.AddDynamic(this,&UDiaSkillUseButton::UseSkill);
	m_nIndex=index;
	ClearSkillSpec();
	m_PlayerDiaComp=diaComp;
}

void UDiaSkillUseButton::SetSkillSpec(FSkillDataSpec* skillSpec)
{
	if(m_EquippedSkillSpec)//버튼에 장착된 스킬 제거
	{
		m_PlayerDiaComp->UnequipSkill(m_EquippedSkillSpec);
		
		//ClearSkillSpec();//called from dele
	}
	if(skillSpec->m_nEquipIndex>-1)// 드래그한 스킬을 다른곳에서 해제
	{
		m_PlayerDiaComp->UnequipSkill(skillSpec);
	}
	
	m_EquippedSkillSpec=skillSpec;
	m_EquippedSkillSpec->m_nEquipIndex=m_nIndex;

	FButtonStyle Style;
	FSlateBrush DefaultStyle;
	DefaultStyle.SetResourceObject(m_EquippedSkillSpec->m_SkillDataPtr->m_SkillIcon);
	
	Style.Normal = DefaultStyle;
	Style.Hovered= DefaultStyle;
	DefaultStyle.TintColor = FSlateColor(FLinearColor(0.5f,0.5f,0.5f,1.f));
	Style.Pressed= DefaultStyle;
	
	m_BtnSkill->SetStyle(Style);

	m_PlayerDiaComp->EquipSkill(m_EquippedSkillSpec);
	
}

void UDiaSkillUseButton::ClearSkillSpec()
{
	
	m_GaSpec=nullptr;
	m_EquippedSkillSpec=nullptr;
	m_BtnSkill->SetStyle(FButtonStyle());
	ClearCooldown();
}

void UDiaSkillUseButton::ClearCooldown()
{
	m_fMaxCD=0.f;
	m_SkillCooldown->SetCooldownProgress(0.f,0.f);
}

void UDiaSkillUseButton::UseSkill()
{
    PRINTF("UseSkill");
	
	m_GaSpec = m_PlayerDiaComp->UseSkill(m_EquippedSkillSpec);

	if(!m_GaSpec)
	{
		return;
	}
	
	m_fMaxCD = m_GaSpec->Ability->GetCooldownTimeRemaining(m_PlayerDiaComp->AbilityActorInfo.Get());

	if(m_fMaxCD>0.f)
	{
		m_SkillCooldown->StartCooldown();
		m_EquippedSkillSpec->m_LearnBtn->StartCooldown();
	}
}

bool UDiaSkillUseButton::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                      UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UDiaDragDropSkill* DDOSkill = Cast<UDiaDragDropSkill>(InOperation);

	if(!DDOSkill)
	{
		return false;
	}

	SetSkillSpec(DDOSkill->m_DraggedSkillData);
	
	return true;
}

void UDiaSkillUseButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!m_GaSpec||m_fMaxCD<=0.f)
	{
		return;
	}

	float CD = m_GaSpec->Ability->GetCooldownTimeRemaining(m_PlayerDiaComp->AbilityActorInfo.Get());
	
	if(CD<=0.f)
	{
		ClearCooldown();
		m_EquippedSkillSpec->m_LearnBtn->ClearCooldown();
		return;
	}
	
	m_SkillCooldown->SetCooldownProgress(CD,m_fMaxCD);//0이 끝임
	m_EquippedSkillSpec->m_LearnBtn->SetCooldownProgress(CD,m_fMaxCD);//0이 끝임
}

//float CD = Spec->Ability->GetCooldownTimeRemaining();
// if(m_GaSpec) = FindAbilitySpecFromHandle(Handle);
//
// if(&Spec==Spec2)
// {
// 	PRINTF("Here");
// }