#include "DiaDragDrop.h"
#include "DiabloM.h"


void UDiaDragDrop::SetDDO(const FItemInstance & itemInst)
{
	m_DraggedItem = itemInst;
	m_nPreIndex = m_DraggedItem.m_nGridIndex;
	OnDragCancelled.AddDynamic(this, &UDiaDragDrop::OnDragCancel);
	UDragDropOperation::OnDrop.AddDynamic(this, &UDiaDragDrop::OnDrop);
}

void UDiaDragDrop::InitDrag(UDiaInvenGridSlot * preSlot)
{
	m_PreSlot = preSlot;
	//m_PreGrid = prePanel;
	DefaultDragVisual = m_PreSlot->m_SizeItemVisual;
	
	m_PreSlot->SetIconOpacity(0.5f);
	
}

void UDiaDragDrop::OnDragCancel(UDragDropOperation * meSelf)
{
	m_PreSlot->SetIconOpacity(1.f);
	UDiaInvenGridSlot::GetDDOInst = nullptr;
	PRINTF("DragCancel DDO");
}

void UDiaDragDrop::OnDrop(UDragDropOperation * meSelf)
{
	m_PreSlot->SetIconOpacity(1.f);
	UDiaInvenGridSlot::GetDDOInst = nullptr;
	PRINTF("Drop DDO");
}