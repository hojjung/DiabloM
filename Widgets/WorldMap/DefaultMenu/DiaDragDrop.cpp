#include "DiaDragDrop.h"

UDiaDragDrop* UDiaDragDrop::GetDDOInst = nullptr;

void UDiaDragDrop::SetDDO(const FItemInstance & itemInst)
{
	m_DraggedItem = itemInst;
	m_nPreIndex = m_DraggedItem.m_nGridIndex;
	OnDragCancelled.AddDynamic(this, &UDiaDragDrop::OnDragCancel);
	UDragDropOperation::OnDrop.AddDynamic(this, &UDiaDragDrop::OnDrop);
}

void UDiaDragDrop::InitDrag(UImage * preSlot)
{
	m_PreSlot = preSlot;
	//m_PreGrid = prePanel;
	DefaultDragVisual = m_PreSlot;
	
	m_PreSlot->SetOpacity(0.5f);
	
}

void UDiaDragDrop::OnDragCancel(UDragDropOperation * meSelf)
{
	m_PreSlot->SetOpacity(1.f);
	UDiaDragDrop::GetDDOInst = nullptr;
	PRINTF("DragCancel DDO");
}

void UDiaDragDrop::OnDrop(UDragDropOperation * meSelf)
{
	m_PreSlot->SetOpacity(1.f);
	UDiaDragDrop::GetDDOInst = nullptr;
	PRINTF("Drop DDO");
}