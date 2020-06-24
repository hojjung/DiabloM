// Fill out your copyright notice in the Description page of Project Settings.


#include "DiaDragDrop.h"

void UDiaDragDrop::SetDDO(const FItemInstance & itemInst)
{
	m_DraggedItem = itemInst;
	m_nPreIndex = m_DraggedItem.GetIndex();
	OnDragCancelled.AddDynamic(this, &UDiaDragDrop::OnDragCancel);
}

void UDiaDragDrop::InitDrag(UDiaInvenGridSlot * preSlot, UDiaInvenGridPanel * prePanel)
{
	m_PreSlot = preSlot;
	m_PreGrid = prePanel;

	m_PreSlot->SetIconOpacity(0.5f);

}

void UDiaDragDrop::OnDragCancel(UDragDropOperation * meSelf)
{
	m_PreSlot->SetIconOpacity(1.f);
}
