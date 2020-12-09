#include "SkillLearnButton.h"


void USkillLearnButton::UpdateLevelText()
{
	if(m_HoldSpec->m_nCurrentLevel<1)
	{
		m_TextSkillLevel->SetText(FText::FromString("X"));
	}
	else
	{
		m_TextSkillLevel->SetText(UKismetTextLibrary::Conv_IntToText(m_HoldSpec->m_nCurrentLevel,false,false));
	}
}

void USkillLearnButton::InitSkillButton(FSkillDataSpec& skill_spec)
{
	m_HoldSpec=&skill_spec;
	m_HoldSpec->m_LearnBtn=this;
	m_ImageSkillIcon->SetBrushFromTexture(m_HoldSpec->m_SkillDataPtr->m_SkillIcon);

	UpdateLevelText();
	ClearCooldown();
}

void USkillLearnButton::SkillSelected()//직접 부르지말고 델리게이트?
{
	m_SelectFrame->SetCheckedState(ECheckBoxState::Checked);
}

void USkillLearnButton::SkillDeselected()
{
	m_SelectFrame->SetCheckedState(ECheckBoxState::Unchecked);
}

void USkillLearnButton::CloseHotKeyPanel()
{
	m_OnDragEnd.Broadcast();
}

void USkillLearnButton::SetCooldownProgress(float cd, float m_f_max_cd)
{
	m_SkillCooldown->SetCooldownProgress(cd,m_f_max_cd);
}

UDiaDragDropSkill* USkillLearnButton::CreateDDO()
{
	auto* DDO = Cast<UDiaDragDropSkill>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDiaDragDropSkill::StaticClass()));
	
	DDO->SetDDO(m_HoldSpec,this);
	
	return DDO;
}

void USkillLearnButton::ClearCooldown()
{
	m_SkillCooldown->SetCooldownProgress(0.f,0.f);
}

FReply USkillLearnButton::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	
	m_OnClicked.Broadcast(InGeometry,*m_HoldSpec);//팝업
	
	return Reply;
}

FReply USkillLearnButton::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent,this,EKeys::LeftMouseButton).NativeReply;
}

void USkillLearnButton::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
    if(m_HoldSpec->m_nCurrentLevel<1)
    {
	    return;
    }
	m_OnDragDetect.Broadcast();
	OutOperation = CreateDDO();
}

void USkillLearnButton::StartCooldown()
{
	m_SkillCooldown->StartCooldown();
}
