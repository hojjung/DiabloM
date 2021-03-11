#include "DiaSkillUseButton.h"

#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/UpgradeMenu/SkillUpgradeButton.h"


void UDiaSkillUseButton::Init(int index)
{
	m_fMaxCD = 0.f;
	//m_EquippedSkillSpec=nullptr;
	m_nIndex = -1;
	m_bIsPressing = false;
	m_bIsDragSkill = false;
	m_bIsSkillUsable = false;
	m_nIndex = index;

	ClearSkillSpec();

	m_Joystick->m_OnDropEnd.AddUObject(this, &UDiaSkillUseButton::UseSkill);

	m_DeleHandle = UDiabloGameInstance::Get->GetPlCon()->m_OnTick.AddUObject(
		this, &UDiaSkillUseButton::CustomTickForBind);
}

UDiaSkillUseButton::~UDiaSkillUseButton()
{
	//pl con already destoryed
	if(UDiabloGameInstance::Get&&UDiabloGameInstance::Get->GetPlCon())
	{
		UDiabloGameInstance::Get->GetPlCon()->m_OnTick.Remove(m_DeleHandle);
	}
}

void UDiaSkillUseButton::SetSkillSpec(FSkillSpec* skillSpec)
{
	if (!skillSpec)
	{
		ClearSkillSpec();
		return;
	}

	m_EquippedSkillSpec = skillSpec;
	m_EquippedSkillSpec->m_nIndex = m_nIndex;

	m_Joystick->SetIcon(m_EquippedSkillSpec->m_SkillData->m_SkillIcon);

	m_bIsSkillUsable = true;

	m_bIsDragSkill = m_EquippedSkillSpec->m_SkillData->m_bIsDragSkill;

	m_Joystick->SetUseDrag(m_bIsDragSkill);


	
}

void UDiaSkillUseButton::ClearSkillSpec()
{
	m_bIsSkillUsable = false;

	m_EquippedSkillSpec = nullptr;

	m_Joystick->ClearIcon();

	ClearCooldown();
}

void UDiaSkillUseButton::ClearCooldown()
{
	m_fMaxCD = 0.f;

	m_SkillCooldown->SetCooldownProgress(0.f, 0.f);
}

void UDiaSkillUseButton::OnPressBtn()
{

	if (USkillUpgradeButton::CurrentSelected)
	{
		UDiabloGameInstance::Get->m_PlayerUpgradeManager->EquipSkill(
			m_nIndex, USkillUpgradeButton::CurrentSelected->GetSkillSpec());
		return;
	}

	if (!m_bIsSkillUsable)
	{
		return;
	}

	if (m_bIsDragSkill)
	{
		m_OnPressed.Broadcast(this);
	}

	m_bIsPressing = true;
}

void UDiaSkillUseButton::OnReleaseBtn()
{
	if (!m_bIsSkillUsable)
	{
		return;
	}
	m_bIsPressing = false;

	if (m_bIsDragSkill)
	{
		m_OnReleased.Broadcast(this);
	}
}

void UDiaSkillUseButton::TryStartCooldown()
{
	m_fMaxCD = m_EquippedSkillSpec->m_SkillData->m_fSkillCoolTime;
	
	m_fCurrentCD = m_fMaxCD; 
	
	if (m_fCurrentCD > 0.f)
	{
		m_SkillCooldown->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_SkillCooldown->StartCooldown();
	}
}

void UDiaSkillUseButton::UseSkill()
{
	//UseSkill

	if(!UDiabloGameInstance::Get->m_PlayerUpgradeManager->UseSkill(m_nIndex))
	{
		return;
	}

	//TryStartCooldown();
}

bool UDiaSkillUseButton::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                      UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return true;
}

FReply UDiaSkillUseButton::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply ASD = Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	OnPressBtn();

	return FReply::Handled();
}

FReply UDiaSkillUseButton::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply ASD = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	OnReleaseBtn();

	return FReply::Handled();
}

void UDiaSkillUseButton::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	OnReleaseBtn();
}

void UDiaSkillUseButton::CustomTickForBind(float delta)
{
	if (m_bIsPressing && !m_bIsDragSkill)
	{
		UDiabloGameInstance::Get->GetPlChar()->HomingRotateToTarget();

		UseSkill();
	}

	m_fCurrentCD -= delta;

	if (m_fCurrentCD <= 0.f)
	{
		ClearCooldown();

		return;
	}

	m_SkillCooldown->SetCooldownProgress(m_fCurrentCD, m_fMaxCD); //0이 끝임
}

