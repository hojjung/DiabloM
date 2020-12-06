#include "DiaDragDrop.h"

UDiaDragDrop* UDiaDragDrop::GetDDOInst = nullptr;

void UDiaDragDrop::SetDDO(const FItemInstance & itemInst,UImage * preSlot)
{
	m_DraggedItem = itemInst;
	m_nPreIndex = m_DraggedItem.m_nGridIndex;
	OnDragCancelled.AddDynamic(this, &UDiaDragDrop::OnDragCancel);
	UDragDropOperation::OnDrop.AddDynamic(this, &UDiaDragDrop::OnDrop);
	m_PreSlot = preSlot;
	
	DefaultDragVisual = m_PreSlot;
	
	m_PreSlot->SetOpacity(0.5f);

	if(UDiaDragDrop::GetDDOInst)
	{
		FPointerEvent Event;
		UDiaDragDrop::GetDDOInst->DragCancelled(Event);
	}
	
	UDiaDragDrop::GetDDOInst = this;
}


void UDiaDragDrop::OnDragCancel(UDragDropOperation * meSelf)
{
	m_PreSlot->SetOpacity(1.f);
	UDiaDragDrop::GetDDOInst = nullptr;
}

void UDiaDragDrop::OnDrop(UDragDropOperation * meSelf)
{
	m_PreSlot->SetOpacity(1.f);
	UDiaDragDrop::GetDDOInst = nullptr;
}