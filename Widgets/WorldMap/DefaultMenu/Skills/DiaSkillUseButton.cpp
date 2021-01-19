#include "DiaSkillUseButton.h"
#include "DiaDragDropSkill.h"
#include "SkillLearnButton.h"


void UDiaSkillUseButton::Init(UPlayerDiabloAbilitySystemComp* diaComp,int index)
{
	m_fMaxCD=0.f;
	m_GaSpec=nullptr;
	m_EquippedSkillSpec=nullptr;
	m_nIndex=-1;
	m_bIsPressing =false;
	m_bIsDragSkill=false;
	m_bIsSkillUsable=false;
	m_nIndex=index;
	ClearSkillSpec();
	m_PlayerDiaComp=diaComp;
	FSlateBrush Brush;
	m_SkillIcon->SetBrush(Brush);
	m_SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UDiaSkillUseButton::SetSkillSpec(FSkillDataSpec* skillSpec)
{
	if(m_EquippedSkillSpec)//버튼에 장착된 스킬 제거
	{
		if(!m_PlayerDiaComp->UnequipSkill(m_EquippedSkillSpec))
		{
			return;
		}
	}
	
	if(skillSpec->m_nEquipIndex>-1)// 드래그한 스킬을 다른곳에서 해제
	{
		m_PlayerDiaComp->UnequipSkill(skillSpec);
	}
	
	m_EquippedSkillSpec=skillSpec;
	
	m_EquippedSkillSpec->m_nEquipIndex=m_nIndex;

	FSlateBrush DefaultStyle;
	
	DefaultStyle.SetResourceObject(m_EquippedSkillSpec->m_SkillDataPtr->m_SkillIcon);
	
	m_SkillIcon->SetBrush(DefaultStyle);
	
	//m_BtnSkill->SetVisibility(ESlateVisibility::Visible);

	m_bIsSkillUsable = true;

	m_bIsDragSkill = m_EquippedSkillSpec->m_SkillDataPtr->m_bIsJoystickDragger;

	m_bIsDragSkill=true;

	m_Joystick->SetUseDrag(true);//TEST

	m_PlayerDiaComp->EquipSkill(m_EquippedSkillSpec);
}

void UDiaSkillUseButton::ClearSkillSpec()
{
	m_bIsSkillUsable = false;
	m_GaSpec=nullptr;
	m_EquippedSkillSpec=nullptr;
	
	FSlateBrush DefaultStyle;
	
	m_SkillIcon->SetBrush(DefaultStyle);
	
	ClearCooldown();
}

void UDiaSkillUseButton::ClearCooldown()
{
	m_fMaxCD=0.f;
	m_SkillCooldown->SetCooldownProgress(0.f,0.f);
}

void UDiaSkillUseButton::OnPressBtn()
{
	if(m_bIsDragSkill)
	{
		return;
	}
	if(!m_bIsSkillUsable)
	{
		return;
	}
	m_bIsPressing =true;
}

void UDiaSkillUseButton::OnReleaseBtn()
{
	if(m_bIsDragSkill)
	{
		return;
	}
	if(!m_bIsSkillUsable)
	{
		return;
	}
	m_bIsPressing =false;
}

void UDiaSkillUseButton::UseSkill()
{
    FGameplayAbilitySpec* AbilSpec = m_PlayerDiaComp->UseSkill(m_EquippedSkillSpec);

	if(!AbilSpec)
	{
		return;
	}

	m_GaSpec=AbilSpec;
	
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

	if(m_bIsPressing)
	{
		UseSkill();
	}

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

FReply UDiaSkillUseButton::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply ASD = Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
	OnPressBtn();
	return ASD;
}

FReply UDiaSkillUseButton::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply ASD = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	OnReleaseBtn();

	return  ASD;
}

void UDiaSkillUseButton::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	OnReleaseBtn();
}


//float CD = Spec->Ability->GetCooldownTimeRemaining();
// if(m_GaSpec) = FindAbilitySpecFromHandle(Handle);
//
// if(&Spec==Spec2)
// {
// 	PRINTF("Here");
// }