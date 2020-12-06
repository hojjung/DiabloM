#include "DiaDragDropSkill.h"
#include "SkillLearnButton.h"

UDiaDragDropSkill* UDiaDragDropSkill::GetDDOInst =nullptr;

void UDiaDragDropSkill::SetDDO(FSkillDataSpec* skillSpec, USkillLearnButton* skillLearnBtn)
{
	PRINTF("SkillDDO");
	m_DraggedSkillData=skillSpec;

	m_SkillLearnWidget=skillLearnBtn;
	
	m_SkillLearnWidget->m_ImageSkillIcon->SetRenderOpacity(0.5f);
	
	DefaultDragVisual=m_SkillLearnWidget->m_ImageSkillIcon;
	
	OnDragCancelled.AddDynamic(this, &UDiaDragDropSkill::OnDragCancel);
	
	UDragDropOperation::OnDrop.AddDynamic(this, &UDiaDragDropSkill::OnDrop);
	 
	if(UDiaDragDropSkill::GetDDOInst)
	{
		FPointerEvent Event;
		UDiaDragDropSkill::GetDDOInst->DragCancelled(Event);
	}
	
	UDiaDragDropSkill::GetDDOInst = this;
}

void UDiaDragDropSkill::OnDragCancel(UDragDropOperation* meSelf)
{
	m_SkillLearnWidget->m_ImageSkillIcon->SetRenderOpacity(1.f);
	UDiaDragDropSkill::GetDDOInst = nullptr;
}

void UDiaDragDropSkill::OnDrop(UDragDropOperation* meSelf)//drop true
{
	//Drop을 불러준 위젯이 Deselect부르게
	m_SkillLearnWidget->m_ImageSkillIcon->SetRenderOpacity(1.f);
	UDiaDragDropSkill::GetDDOInst = nullptr;
	m_SkillLearnWidget->SkillSelected();
}
