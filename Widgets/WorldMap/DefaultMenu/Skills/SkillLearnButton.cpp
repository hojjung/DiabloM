#include "SkillLearnButton.h"


void USkillLearnButton::InitSkillButton(FSkillDataSpec& skill_spec)
{
	m_HoldSpec=&skill_spec;
	m_ImageSkillIcon->SetBrushFromTexture(m_HoldSpec->m_SkillDataPtr->m_SkillIcon);

	if(m_HoldSpec->m_nCurrentLevel<1)
	{
		m_TextSkillLevel->SetText(FText::FromString("X"));
	}
	else
	{
		m_TextSkillLevel->SetText(UKismetTextLibrary::Conv_IntToText(m_HoldSpec->m_nCurrentLevel,false,false));
	}	
}

void USkillLearnButton::SkillSelected()//직접 부르지말고 델리게이트?
{
	m_SelectFrame->SetCheckedState(ECheckBoxState::Checked);
}

void USkillLearnButton::SkillDeselected()
{
	m_SelectFrame->SetCheckedState(ECheckBoxState::Unchecked);
}

UDiaDragDropSkill* USkillLearnButton::CreateDDO()
{
	auto* DDO = Cast<UDiaDragDropSkill>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDiaDragDropSkill::StaticClass()));
	
	DDO->SetDDO(m_HoldSpec,this);
	
	return DDO;
}

FReply USkillLearnButton::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	
	// if (!IsSlotEmpty())
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

	// if (IsSlotEmpty())
	// {
	// 	return;
	// }
	m_OnDragDetect.ExecuteIfBound();
	OutOperation = CreateDDO();
}
