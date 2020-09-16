// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem.h"


#include "IDetailTreeNode.h"
#include "AbilitySystem/DiabloAbilitySystemComp.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"
#include "Item/ItemManager.h"

UEquipmentSystem::~UEquipmentSystem()
{
}


void UEquipmentSystem::Init(UDiabloAbilitySystemComp* abilitySysCompo)
{
    m_TargetAbilitySys=abilitySysCompo;
    
    m_Head.SetEquipableType(EItemType::Helmet, true);
    m_Neck.SetEquipableType(EItemType::Necklace, true);
    m_Torso.SetEquipableType(EItemType::BodyArmor, true);
    m_Waist.SetEquipableType(EItemType::Belt, true);
    m_Leg.SetEquipableType(EItemType::Leggins, true);
    m_Hand.SetEquipableType(EItemType::Gauntlets, true);
    m_Shoulder.SetEquipableType(EItemType::ShoulderArmor, true);
    m_FingerLeft.SetEquipableType(EItemType::Ring, true);
    m_FingerRight.SetEquipableType(EItemType::Ring, true);

    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, true);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, true);
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, true);

    m_WeaponRight.SetEquipableType(EItemType::OneHandSword, true);
    m_WeaponRight.SetEquipableType(EItemType::TwohandSword, true);
    m_WeaponRight.SetEquipableType(EItemType::Katana, true);
    m_WeaponRight.SetEquipableType(EItemType::Dagger, true);
    m_WeaponRight.SetEquipableType(EItemType::Bow, true);
    m_WeaponRight.SetEquipableType(EItemType::Staff, true);

    m_ArySlots.Reset(12);
    m_ArySlots.Emplace(&m_Head);
    m_ArySlots.Emplace(&m_Neck);
    m_ArySlots.Emplace(&m_Torso);
    m_ArySlots.Emplace(&m_Waist);
    m_ArySlots.Emplace(&m_Leg);
    m_ArySlots.Emplace(&m_Hand);
    m_ArySlots.Emplace(&m_Shoulder);
    m_ArySlots.Emplace(&m_WeaponRight);
    m_ArySlots.Emplace(&m_WeaponLeft);
    m_ArySlots.Emplace(&m_FingerRight);
    m_ArySlots.Emplace(&m_FingerLeft);

    //right/left
    m_StanceFPtr[static_cast<int>(EItemType::None)][static_cast<int>(EItemType::None)] = &
        UEquipmentSystem::SetStanceAllNull;
    m_StanceFPtr[static_cast<int>(EItemType::None)][static_cast<int>(EItemType::OneHandSword)] = &
        UEquipmentSystem::SetStanceNull;
    m_StanceFPtr[static_cast<int>(EItemType::None)][static_cast<int>(EItemType::Dagger)] = &
        UEquipmentSystem::SetStanceNull;
    m_StanceFPtr[static_cast<int>(EItemType::None)][static_cast<int>(EItemType::Shield)] = &
        UEquipmentSystem::SetStanceNull;
    m_StanceFPtr[static_cast<int>(EItemType::None)][static_cast<int>(EItemType::TwohandSword)] = &
        UEquipmentSystem::SetStanceNull;
    m_StanceFPtr[static_cast<int>(EItemType::None)][static_cast<int>(EItemType::Katana)] = &
        UEquipmentSystem::SetStanceNull;
    //				  EItemType
    m_StanceFPtr[static_cast<int>(EItemType::OneHandSword)][static_cast<int>(EItemType::OneHandSword)] = &
        UEquipmentSystem::SetStanceDual;
    m_StanceFPtr[static_cast<int>(EItemType::Dagger)][static_cast<int>(EItemType::OneHandSword)] = &
        UEquipmentSystem::SetStanceDual;
    m_StanceFPtr[static_cast<int>(EItemType::OneHandSword)][static_cast<int>(EItemType::Dagger)] = &
        UEquipmentSystem::SetStanceDual;
    m_StanceFPtr[static_cast<int>(EItemType::Dagger)][static_cast<int>(EItemType::Dagger)] = &
        UEquipmentSystem::SetStanceDual;
    //				  EItemType
    m_StanceFPtr[static_cast<int>(EItemType::OneHandSword)][static_cast<int>(EItemType::Shield)] = &
        UEquipmentSystem::SetStanceShield;
    m_StanceFPtr[static_cast<int>(EItemType::Dagger)][static_cast<int>(EItemType::Shield)] = &
        UEquipmentSystem::SetStanceShield;
    //				  EItemType
    m_StanceFPtr[static_cast<int>(EItemType::OneHandSword)][static_cast<int>(EItemType::None)] = &
        UEquipmentSystem::SetStanceOneHand;
    //				  EItemType
    m_StanceFPtr[static_cast<int>(EItemType::Dagger)][static_cast<int>(EItemType::None)] = &
        UEquipmentSystem::SetStanceDagger;
    //				  EItemType
    m_StanceFPtr[static_cast<int>(EItemType::Katana)][static_cast<int>(EItemType::None)] = &
        UEquipmentSystem::SetStanceKatana;
    //				  EItemType
    m_StanceFPtr[static_cast<int>(EItemType::TwohandSword)][static_cast<int>(EItemType::None)] = &
        UEquipmentSystem::SetStanceTwoHand;
    m_StanceFPtr[static_cast<int>(EItemType::Bow)][static_cast<int>(EItemType::None)] = &
       UEquipmentSystem::SetStanceBow;
    m_StanceFPtr[static_cast<int>(EItemType::Staff)][static_cast<int>(EItemType::None)] = &
       UEquipmentSystem::SetStanceStaff;
}

bool UEquipmentSystem::AddItem(int droppedIndex, FItemInstance& itemWantAdd) //drag된 대상이 어떤 아이템을 가졌는지 알방법이 없음
{
    if (!CheckSlotValid(droppedIndex, itemWantAdd))
    {
        return false; //애초부터 안맞음 혹은 양손검 등으로 빈 오큐파이일때
    }

    if (!CheckSlotOccupied(droppedIndex))
    {
        SetItem(droppedIndex, itemWantAdd); //그냥 비어있던 슬롯
        
        if(itemWantAdd.m_Holder)
        {
            itemWantAdd.m_Holder->RemoveItem(itemWantAdd);
        }
        return true;
    }

    if (m_ArySlots[droppedIndex]->m_Item.m_ItemID == NAME_None)
    {
        PRINTF("Probably Katana or Twohandsword left hand blocked");
        return false;
    }

    return SwapMove(GetItem(droppedIndex), itemWantAdd);
}

void UEquipmentSystem::RemoveItem(FItemInstance& itemWantErase)
{
    RemoveItemByIndex(itemWantErase.m_nGridIndex);
}

void UEquipmentSystem::RemoveItemByIndex(int index)
{
    m_ArySlots[index]->SetOccupie(false);
    m_ArySlots[index]->m_EquippedType = EItemType::None;
    m_ArySlots[index]->m_Item.ClearData();

    OnItemSlotChanged(index);
}

void UEquipmentSystem::OnItemSlotChanged(int index)
{
    CalculateStance();
    m_ItemChanged.Broadcast(index, GetItem(index));
    //이펙트 부여
    //무기 생성
    //
}

void UEquipmentSystem::CalculateStance()
{
    (this->*m_StanceFPtr[static_cast<int>(GetEquippedItem(ESlots::WeaponRight))][static_cast<int>(GetEquippedItem(
        ESlots::WeaponLeft))])();

    m_OnStanceChanged.Broadcast(m_CurrentStance);
}


bool UEquipmentSystem::CheckSlotOccupied(int index)
{
    return m_ArySlots[index]->m_bIsOccupied;
}

bool UEquipmentSystem::SwapMove(FItemInstance& Drop, FItemInstance& Drag)
{
    int DropIndex = Drop.m_nGridIndex;
    int DragIndex = Drag.m_nGridIndex;

    if (!(CheckSlotValid(DropIndex, Drag) && Drag.m_Holder->CheckSlotValid(DragIndex, Drop)))
    {
        return false;
    }

    auto DragBackup = Drag;
    Drag.m_Holder->SetItem(DragIndex, Drop);

    SetItem(DropIndex, DragBackup);

    return true;
}

bool UEquipmentSystem::CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd)
{
    EItemType ItemTypeWantAdd = itemWantAdd.m_ItemData->m_ItemType;

    if (!m_ArySlots[droppedIndex]->m_AbleEquipSlot[static_cast<int>(ItemTypeWantAdd)]) //못끼는 슬록이면 실패
    {
        return false;
    }

    if (m_ArySlots[droppedIndex]->m_Item.IsEmpty()) //비어있지않음
    {
        return true;
    }

    if (ItemTypeWantAdd == EItemType::TwohandSword || ItemTypeWantAdd == EItemType::Katana) //낄무기가 양손무기일걍우
    {
        EItemType ItemTypeDroppedBefore = m_ArySlots[droppedIndex]->m_Item.m_ItemData->m_ItemType;
        //이미껴진 무기를 가져옴
        if (ItemTypeDroppedBefore == EItemType::TwohandSword
            || ItemTypeDroppedBefore == EItemType::Katana) //만일 껴저있는 무기가 양손무기라면 가능 이후 교체로 진행
        {
            return true;;
        }

        if (m_WeaponLeft.m_bIsOccupied) //왼손에 뭔가 껴저있다면 실패
        {
            return false;
        }
    }

    return true;
}

void UEquipmentSystem::SetItem(int droppedIndex, FItemInstance& itemWantAdd)
{
    m_ArySlots[droppedIndex]->m_Item = itemWantAdd;
    m_ArySlots[droppedIndex]->SetOccupie(true);
    m_ArySlots[droppedIndex]->m_EquippedType = m_ArySlots[droppedIndex]->m_Item.m_ItemData->m_ItemType;
    //
    m_ArySlots[droppedIndex]->m_Item.m_nGridIndex = droppedIndex;
    m_ArySlots[droppedIndex]->m_Item.m_Holder = this;


    //UG

    if(itemWantAdd.m_ItemData->m_Options.Num()<1)
    {
        return;
    }
    

    for(auto& Option : itemWantAdd.m_ItemData->m_Options)
    {
        if(Option.IsNull())
        {
            continue;
        }
        //
        auto Context= m_TargetAbilitySys->MakeEffectContext();
        Context.AddSourceObject(m_TargetAbilitySys->GetOwner());
        //
        //
        FGameplayEffectSpecHandle NewHandle = m_TargetAbilitySys->MakeOutgoingSpec(itemWantAdd.m_ItemData->GetOption(0).m_OptionGe, 1, Context);

        NewHandle.Data.Get()->SetSetByCallerMagnitude(itemWantAdd.m_ItemData->GetOption(0).m_OptionTag, 999.f);
        //제거 테스트
        FActiveGameplayEffectHandle AA = m_TargetAbilitySys->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), m_TargetAbilitySys);

        FGameplayEffectQuery Query;
        Query.EffectSource = m_TargetAbilitySys->GetOwner();

        if(AA.IsValid())
        {
            PRINTF("It is valide");      
        }
         if (m_TargetAbilitySys->RemoveActiveEffects(Query))
         {
          PRINTF("Erased1");   
         }

    }

    OnItemSlotChanged(droppedIndex);
}

FItemInstance& UEquipmentSystem::GetItem(int index)
{
    return m_ArySlots[index]->m_Item;
}

void UEquipmentSystem::PrintEquipStats()
{
    PRINTF("----EquipPrint----");

    PRINTF("CurrentStance: %s", *EnumToStr(EAnimStance, m_CurrentStance));

    for (int i = 0; i < static_cast<int>(ESlots::Length); i++)
    {
        PRINTF("Slot: %s - EquipItem: %s - Occupied: %hs", *EnumToStr(ESlots, (ESlots)i),
               *EnumToStr(EItemType, GetEquippedItem(i)), m_ArySlots[i]->m_bIsOccupied ? "Y" : "N");
    }
}

EItemType UEquipmentSystem::GetEquippedItem(ESlots slot)
{
    return m_ArySlots[static_cast<int>(slot)]->m_EquippedType;
}

EItemType UEquipmentSystem::GetEquippedItem(int slotIndex)
{
    return m_ArySlots[slotIndex]->m_EquippedType;
}

void UEquipmentSystem::SetItemAry(TArray<FItemInstance>& equipSlot)
{
    for (int i = 0; i < m_ArySlots.Num(); i++)
    {
        if(!equipSlot[i].m_ItemData)
        {
            continue;
        }
        
        AddItem(i,equipSlot[i]);
    }
}


#pragma region SetFPtr


void UEquipmentSystem::SetStanceAllNull()
{
    //
    m_WeaponRight.SetEquipableType(EItemType::Katana, true);
    m_WeaponRight.SetEquipableType(EItemType::TwohandSword, true);
    //
    m_WeaponLeft.SetOccupie(false);
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, true);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, true);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, true);
    //아래랑 똑같아보이지만 아래의 경우는 왼손에 무기가 들려있으나 오른손에 무기가 없어서 공격을 못하는것임
    m_WeaponRight.SetEquipableType(EItemType::Katana, true);
    m_WeaponRight.SetEquipableType(EItemType::TwohandSword, true);
    m_CurrentStance = EAnimStance::None;
}

void UEquipmentSystem::SetStanceNull()
{
    m_WeaponRight.SetEquipableType(EItemType::Katana, false);
    m_WeaponRight.SetEquipableType(EItemType::TwohandSword, false);
    //
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, true);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, true);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, true);
    //
    if (!m_WeaponLeft.m_bIsOccupied) //비어있을때
    {
        m_WeaponRight.SetEquipableType(EItemType::Katana, true);
        m_WeaponRight.SetEquipableType(EItemType::TwohandSword, true);
        m_WeaponLeft.SetOccupie(false);
    }

    m_CurrentStance = EAnimStance::None;
}

void UEquipmentSystem::SetStanceOneHand()
{
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, true);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, true);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, true);
    //
    m_WeaponLeft.SetOccupie(false);
    m_WeaponRight.SetEquipableType(EItemType::Katana, true);
    m_WeaponRight.SetEquipableType(EItemType::TwohandSword, true);
    m_CurrentStance = EAnimStance::OneHandSword;
}

void UEquipmentSystem::SetStanceDual()
{
    m_WeaponRight.SetEquipableType(EItemType::Katana, false);
    m_WeaponRight.SetEquipableType(EItemType::TwohandSword, false);
    //
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, true);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, true);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, true);
    //
    m_CurrentStance = EAnimStance::DualSword;
}

void UEquipmentSystem::SetStanceShield()
{
    m_WeaponRight.SetEquipableType(EItemType::Katana, false);
    m_WeaponRight.SetEquipableType(EItemType::TwohandSword, false);
    //
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, true);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, true);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, true);
    //
    m_CurrentStance = EAnimStance::Shield;
}

void UEquipmentSystem::SetStanceDagger()
{
    m_WeaponLeft.SetOccupie(false);
    m_WeaponRight.SetEquipableType(EItemType::Katana, true);
    m_WeaponRight.SetEquipableType(EItemType::TwohandSword, true);
    //
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, true);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, true);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, true);
    //
    m_CurrentStance = EAnimStance::Dagger;
}

void UEquipmentSystem::SetStanceKatana()
{
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, false);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, false);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, false);
    //
    m_WeaponLeft.SetOccupie(true);
    m_CurrentStance = EAnimStance::Katana;
}

void UEquipmentSystem::SetStanceTwoHand()
{
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, false);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, false);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, false);
    //
    m_WeaponLeft.SetOccupie(true);
    m_CurrentStance = EAnimStance::TwohandSword;
}

void UEquipmentSystem::SetStanceBow()
{
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, false);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, false);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, false);
    //
    m_WeaponLeft.SetOccupie(true);
    m_CurrentStance = EAnimStance::Bow;
}

void UEquipmentSystem::SetStanceStaff()
{
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, false);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, false);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, false);
    //
    m_WeaponLeft.SetOccupie(true);
    m_CurrentStance = EAnimStance::Staff;
}


#pragma endregion

TArray<FEquipSlot*>& UEquipmentSystem::GetArySlotPtr()
{
    return m_ArySlots;
}
