#include "DiaEquipmentPanel.h"
#include "Widgets/DiaInvenGridSlot.h"

void UDiaEquipmentPanel::Init(EquipmentSystem * equipContainer)
{
	m_EquipSys = equipContainer;

	m_ArySlots.Reset(10);
	m_ArySlots.Emplace(m_SlotHead);
	m_ArySlots.Emplace(m_SlotNeck);
	m_ArySlots.Emplace(m_SlotTorso);
	m_ArySlots.Emplace(m_SlotWaist);
	m_ArySlots.Emplace(m_SlotLeg);
	m_ArySlots.Emplace(m_SlotHand);
	m_ArySlots.Emplace(m_SlotShoulder);
	m_ArySlots.Emplace(m_SlotWeaponLeft);
	m_ArySlots.Emplace(m_SlotWeaponRight);
	m_ArySlots.Emplace(m_SlotFingerLeft);
	m_ArySlots.Emplace(m_SlotFingerRight);

	m_SlotHead->InitSlot(0);
	m_SlotNeck->InitSlot(1);
	m_SlotTorso->InitSlot(2);
	m_SlotWaist->InitSlot(3);
	m_SlotLeg->InitSlot(4);
	m_SlotHand->InitSlot(5);
	m_SlotShoulder->InitSlot(6);
	m_SlotWeaponLeft->InitSlot(7);
	m_SlotWeaponRight->InitSlot(8);
	m_SlotFingerLeft->InitSlot(9);
	m_SlotFingerRight->InitSlot(10);

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

	m_EquipSys->GetItemChangeCallback().AddUObject(this, &UDiaEquipmentPanel::UpdateSlot);
	m_EquipSys->GetStanceChangeCallaback().AddUObject(this, &UDiaEquipmentPanel::UpdateStance);
}

bool UDiaEquipmentPanel::EquipItem(int dropIndex, FItemInstance& drag)
{
	return m_EquipSys->AddItem(dropIndex, drag);
}

void UDiaEquipmentPanel::UpdateSlot(int index, const FItemInstance& itemInst)
{
	m_ArySlots[index]->SetSlot(m_EquipSys->GetItem(index));
	PRINTF("UpdateSlot EquipPanel");
}

void UDiaEquipmentPanel::UpdateStance(EAnimStance currentStance)
{
	if (currentStance == EAnimStance::Katana || currentStance == EAnimStance::TwohandSword)
	{
		m_ArySlots[(int)ESlots::WeaponLeft]->UpdateItemVisual(m_EquipSys->GetItem((int)ESlots::WeaponRight));
		m_ArySlots[(int)ESlots::WeaponLeft]->SetVisualColorTint(FColor::Red);
	}
	//else if(currentStance == EAnimStance::DualSword || currentStance == EAnimStance::Shield)
	else
	{
		m_ArySlots[(int)ESlots::WeaponLeft]->SetVisualColorTint(FColor::White);

		if (!m_EquipSys->GetItem((int)ESlots::WeaponLeft).m_ItemData)
		{
			m_ArySlots[(int)ESlots::WeaponLeft]->ClearSlot();
		}

		PRINTF("ClearSlot EquipPanel");
	}
}

//void EquipmentSystem::ChangeStance()//erase
//{
	//auto SkeletalMesh = GetMesh();

	////SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//SkeletalMesh->SetAnimInstanceClass(m_AnimSets[(int)m_CurrentStance]);
	//auto AnimInst = Cast<UDGRG_HumanCharacter_AnimInstance>(SkeletalMesh->GetAnimInstance());
	//SetMaxCombo(AnimInst->m_Motions.Num());
	//ResetCombo();
	//m_OnStanceChanged.Broadcast(m_CurrentStance);
	//PRINTF("Current Stance Index: %s", *GETENUMSTRING("E_STANCE", m_CurrentStance));
//}
