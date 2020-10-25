#include "DiaEquipmentPanel.h"
#include "Widgets/WorldMap/DefaultMenu/DiaInvenGridSlot.h"
#include "Datas/ItemDataTable.h"
#include "Widgets/WorldMap/DefaultMenu/ItemPopupInfo.h"
#include "WidgetLayoutLibrary.h"
UDiaEquipmentPanel* UDiaEquipmentPanel::GetEquipWidgetInst = nullptr;

void UDiaEquipmentPanel::Init(UEquipmentSystem * equipContainer)
{
	m_EquipSys = equipContainer;
	m_nPopupSelectedIndex=-1;
	m_ArySlots.Reset(10);
	m_ArySlots.Emplace(m_SlotHead);
	m_ArySlots.Emplace(m_SlotNeck);
	m_ArySlots.Emplace(m_SlotTorso);
	m_ArySlots.Emplace(m_SlotWaist);
	m_ArySlots.Emplace(m_SlotLeg);
	m_ArySlots.Emplace(m_SlotHand);
	m_ArySlots.Emplace(m_SlotShoulder);
	m_ArySlots.Emplace(m_SlotWeaponRight);
	m_ArySlots.Emplace(m_SlotWeaponLeft);
	m_ArySlots.Emplace(m_SlotFingerRight);
	m_ArySlots.Emplace(m_SlotFingerLeft);

	m_SlotHead->InitSlot(0);
	m_SlotNeck->InitSlot(1);
	m_SlotTorso->InitSlot(2);
	m_SlotWaist->InitSlot(3);
	m_SlotLeg->InitSlot(4);
	m_SlotHand->InitSlot(5);
	m_SlotShoulder->InitSlot(6);
	m_SlotWeaponRight->InitSlot(7);
	m_SlotWeaponLeft->InitSlot(8);
	m_SlotFingerRight->InitSlot(9);
	m_SlotFingerLeft->InitSlot(10);

	m_SlotHead->m_OnDropIndex.BindUObject(this,&UDiaEquipmentPanel::EquipItem);
	m_SlotNeck->m_OnDropIndex.BindUObject(this,&UDiaEquipmentPanel::EquipItem);
	m_SlotTorso->m_OnDropIndex.BindUObject(this,&UDiaEquipmentPanel::EquipItem);
	m_SlotWaist->m_OnDropIndex.BindUObject(this,&UDiaEquipmentPanel::EquipItem);
	m_SlotLeg->m_OnDropIndex.BindUObject(this,&UDiaEquipmentPanel::EquipItem);
	m_SlotHand->m_OnDropIndex.BindUObject(this,&UDiaEquipmentPanel::EquipItem);
	m_SlotShoulder->m_OnDropIndex.BindUObject(this,&UDiaEquipmentPanel::EquipItem);
	m_SlotWeaponLeft->m_OnDropIndex.BindUObject(this,&UDiaEquipmentPanel::EquipItem);
	m_SlotWeaponRight->m_OnDropIndex.BindUObject(this,&UDiaEquipmentPanel::EquipItem);
	m_SlotFingerLeft->m_OnDropIndex.BindUObject(this,&UDiaEquipmentPanel::EquipItem);
	m_SlotFingerRight->m_OnDropIndex.BindUObject(this,&UDiaEquipmentPanel::EquipItem);

	m_SlotHead->m_OnDragIndex.BindUObject(this, &UDiaEquipmentPanel::CheckItemEquipable);
	m_SlotNeck->m_OnDragIndex.BindUObject(this, &UDiaEquipmentPanel::CheckItemEquipable);
	m_SlotTorso->m_OnDragIndex.BindUObject(this, &UDiaEquipmentPanel::CheckItemEquipable);
	m_SlotWaist->m_OnDragIndex.BindUObject(this, &UDiaEquipmentPanel::CheckItemEquipable);
	m_SlotLeg->m_OnDragIndex.BindUObject(this, &UDiaEquipmentPanel::CheckItemEquipable);
	m_SlotHand->m_OnDragIndex.BindUObject(this, &UDiaEquipmentPanel::CheckItemEquipable);
	m_SlotShoulder->m_OnDragIndex.BindUObject(this, &UDiaEquipmentPanel::CheckItemEquipable);
	m_SlotWeaponLeft->m_OnDragIndex.BindUObject(this, &UDiaEquipmentPanel::CheckItemEquipable);
	m_SlotWeaponRight->m_OnDragIndex.BindUObject(this, &UDiaEquipmentPanel::CheckItemEquipable);
	m_SlotFingerLeft->m_OnDragIndex.BindUObject(this, &UDiaEquipmentPanel::CheckItemEquipable);
	m_SlotFingerRight->m_OnDragIndex.BindUObject(this, &UDiaEquipmentPanel::CheckItemEquipable);

	

	m_EquipSys->GetItemChangeCallback().AddUObject(this, &UDiaEquipmentPanel::UpdateSlot);
	m_EquipSys->GetEquipChanged().AddUObject(this,&UDiaEquipmentPanel::UpdateEquipSlot);

	UDiaEquipmentPanel::GetEquipWidgetInst = this;


	for(int i=0; i<m_EquipSys->GetArySlotPtr().Num();i++)
	{
		if(m_EquipSys->GetItem(i).IsEmpty())
		{
			continue;
		}
		UpdateEquipSlot(m_EquipSys->GetItem(i),*m_EquipSys->GetArySlotPtr()[i]);
		UpdateSlot(i,m_EquipSys->GetItem(i));
	}
}

bool UDiaEquipmentPanel::EquipItem(int dropIndex, FItemInstance& drag)
{
	return m_EquipSys->AddItem(dropIndex, drag);
}

void UDiaEquipmentPanel::UnequipItem(int dropIndex)
{
	m_EquipSys->RemoveItemByIndex(dropIndex);
}

bool UDiaEquipmentPanel::CheckItemEquipable(int dropIndex, FItemInstance & drag)
{
	return m_EquipSys->CheckSlotValid(dropIndex,drag);
}

void UDiaEquipmentPanel::UpdateSlot(int index,  FItemInstance& itemInst)
{
	m_ArySlots[index]->SetSlot(m_EquipSys->GetItem(index));

	//
	
	PRINTF("UpdateSlot EquipPanel");
}
void UDiaEquipmentPanel::UpdateEquipSlot(const FItemInstance& itemInst, const FEquipSlot& slot)
{
	if (slot.m_bIsOccupied && slot.m_Item.IsEmpty())
	{
		m_ArySlots[(int)slot.m_Slot]->UpdateItemVisual(itemInst);
		m_ArySlots[(int)slot.m_Slot]->SetVisualColorTint(FLinearColor::Red);
	}
	else
	{
		m_ArySlots[(int)slot.m_Slot]->ClearSlot();
	}
}
