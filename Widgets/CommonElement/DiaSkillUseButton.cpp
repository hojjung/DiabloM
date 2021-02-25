#include "DiaSkillUseButton.h"
#include "Characters/PlayerDiabloCharacter.h"


void UDiaSkillUseButton::Init(int index)
{
	m_fMaxCD=0.f;
	//m_EquippedSkillSpec=nullptr;
	m_nIndex=-1;
	m_bIsPressing =false;
	m_bIsDragSkill=false;
	m_bIsSkillUsable=false;
	m_nIndex=index;
	
	ClearSkillSpec();
	
	m_Joystick->m_OnDropEnd.AddUObject(this,&UDiaSkillUseButton::UseSkill);
}

// void UDiaSkillUseButton::SetSkillSpec(FSkillDataSpec* skillSpec)
// {
// 	if(m_EquippedSkillSpec)//버튼에 장착된 스킬 제거
// 	{
// 	}
// 	
// 	if(skillSpec->m_nEquipIndex>-1)// 드래그한 스킬을 다른곳에서 해제
// 	{
// //		m_PlayerDiaComp->UnequipSkill(skillSpec);
// 	}
// 	
// 	m_EquippedSkillSpec=skillSpec;
// 	
// 	m_EquippedSkillSpec->m_nEquipIndex=m_nIndex;
//
// 	m_Joystick->SetIcon(m_EquippedSkillSpec->m_SkillDataPtr->m_SkillIcon);
// 	
// 	m_bIsSkillUsable = true;
//
// 	m_bIsDragSkill = m_EquippedSkillSpec->m_SkillDataPtr->m_bIsJoystickDragger;
//
// 	m_Joystick->SetUseDrag(m_bIsDragSkill);
//
// //	m_PlayerDiaComp->EquipSkill(m_EquippedSkillSpec);
// }

void UDiaSkillUseButton::ClearSkillSpec()
{
	m_bIsSkillUsable = false;
	
//	m_GaSpec=nullptr;
	
	//m_EquippedSkillSpec=nullptr;
	
	m_Joystick->ClearIcon();
	
	ClearCooldown();
}

void UDiaSkillUseButton::ClearCooldown()
{
	m_fMaxCD=0.f;
	
	m_SkillCooldown->SetCooldownProgress(0.f,0.f);
}

void UDiaSkillUseButton::OnPressBtn()
{
	if(!m_bIsSkillUsable)
	{
		return;
	}
	
	if(m_bIsDragSkill)
	{
		m_OnPressed.Broadcast(this);

		// UPlayerBaseAttack* SkillCDO = Cast< UPlayerBaseAttack>( m_EquippedSkillSpec->m_SkillDataPtr->m_SkillAbility->GetDefaultObject()); 
		// float Radius =  SkillCDO->GetRadius();
		// float Range =  SkillCDO->GetRange();
		//
		// m_Joystick->SetRadius(Radius,Range);

		//PRINTF("Radius:%f",Radius);
	}

	m_bIsPressing =true;
}

void UDiaSkillUseButton::OnReleaseBtn()
{
	if(!m_bIsSkillUsable)
	{
		return;
	}
	m_bIsPressing =false;

	if(m_bIsDragSkill)
	{
		m_OnReleased.Broadcast(this);
	}
}

void UDiaSkillUseButton::UseSkill()
{
 //    FGameplayAbilitySpec* AbilSpec = m_PlayerDiaComp->UseSkill(m_EquippedSkillSpec);
 //
	// if(!AbilSpec)
	// {
	// 	return;
	// }
 //
	// m_GaSpec = AbilSpec;
	//
	// m_fMaxCD = m_GaSpec->Ability->GetCooldownTimeRemaining(m_PlayerDiaComp->AbilityActorInfo.Get());
	
	if(m_fMaxCD>0.f && m_fCurrentCD<=0.f)
	{
		m_SkillCooldown->StartCooldown();
		
		//m_EquippedSkillSpec->m_LearnBtn->StartCooldown();
	}
}

bool UDiaSkillUseButton::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                      UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	//UDiaDragDropSkill* DDOSkill = Cast<UDiaDragDropSkill>(InOperation);

	//if(!DDOSkill)
	{
		return false;
	}

	//SetSkillSpec(DDOSkill->m_DraggedSkillData);
	
	return true;
}

void UDiaSkillUseButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(m_bIsPressing&&!m_bIsDragSkill)
	{
//		m_PlayerPawn->HomingRotateToTarget();
		
		UseSkill();	
	}

	if(m_fMaxCD<=0.f)
	{
		return;
	}

	m_fCurrentCD = 1.f;
	
	if(m_fCurrentCD<=0.f)
	{
		ClearCooldown();
		
		//m_EquippedSkillSpec->m_LearnBtn->ClearCooldown();
		
		return;
	}
	
	m_SkillCooldown->SetCooldownProgress(m_fCurrentCD,m_fMaxCD);//0이 끝임
	
	//m_EquippedSkillSpec->m_LearnBtn->SetCooldownProgress(m_fCurrentCD,m_fMaxCD);//0이 끝임
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