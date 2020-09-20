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
    m_TargetAbilitySys = abilitySysCompo;

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

    m_Head.m_Slot=ESlotsEquipAry::Head;
    m_Neck.m_Slot=ESlotsEquipAry::Neck;
    m_Torso.m_Slot=ESlotsEquipAry::Torso;
    m_Waist.m_Slot=ESlotsEquipAry::Waist;
    m_Leg.m_Slot=ESlotsEquipAry::Leg;
    m_Hand.m_Slot=ESlotsEquipAry::Hand;
    m_Shoulder.m_Slot=ESlotsEquipAry::Shoulder;
    m_WeaponRight.m_Slot=ESlotsEquipAry::WeaponRight;
    m_WeaponLeft.m_Slot=ESlotsEquipAry::WeaponLeft;
    m_FingerRight.m_Slot=ESlotsEquipAry::FingerRight;
    m_FingerLeft.m_Slot=ESlotsEquipAry::FingerLeft;
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

        if (itemWantAdd.m_Holder)
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
    m_ArySlots[index]->m_EquippedType = nullptr;
    m_ArySlots[index]->m_Item.ClearData();
    //
    if (m_ArySlots[index]->m_OptionHandle.IsValid())
    {
        m_TargetAbilitySys->RemoveActiveGameplayEffect(m_ArySlots[index]->m_OptionHandle);
        m_ArySlots[index]->m_OptionHandle.RemoveFromGlobalMap();
    }
    OnItemSlotChanged(index);
}

void UEquipmentSystem::OnItemSlotChanged(int index)
{
    m_ItemChanged.Broadcast(index, GetItem(index));
    //이펙트 부여
    //무기 생성
    //
}

bool UEquipmentSystem::CheckSlotOccupied(int index)
{
    return m_ArySlots[index]->m_EquippedType;
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
    const FItemType* ItemTypeWantAdd = itemWantAdd.m_ItemData->m_ItemType.GetRow<FItemType>("");

    if(!TEST_BIT(ItemTypeWantAdd->m_EquipableSlot,m_ArySlots[droppedIndex]->m_Slot))
    {
        PRINTF("BitEquipFail 1");
        return false;
    }

    for(auto Slot:m_ArySlots)
    {
        if(TEST_BIT(ItemTypeWantAdd->m_EquipInterruptSlot,Slot->m_Slot))
        {
            PRINTF("Interrupt Slot 2");
            return false;
        }
    }
    
    if (m_ArySlots[droppedIndex]->m_Item.IsEmpty()) //비어있지않음
    {
        return true;
    }

    //장비가 끼워지고 나면, 인터럽팅 슬롯을 빨갛게 칠해줘야하고
    //장비들을 돌려서 현재 애님스턴스를 반환해야함
    return true;
}

void UEquipmentSystem::SetItem(int droppedIndex, FItemInstance& itemWantAdd)
{
    const FItemType* ItemTypeWantAdd =  itemWantAdd.m_ItemData->m_ItemType.GetRow<FItemType>("");
    m_ArySlots[droppedIndex]->m_Item = itemWantAdd;
    m_ArySlots[droppedIndex]->m_EquippedType =ItemTypeWantAdd;
    //
    m_ArySlots[droppedIndex]->m_Item.m_nGridIndex = droppedIndex;
    m_ArySlots[droppedIndex]->m_Item.m_Holder = this;

    //UG
    TSubclassOf<UGameplayEffect> GameplayEffect = ItemTypeWantAdd->m_OptionGameEffect;
    
    if (itemWantAdd.m_AryOptions.Num() > 0)
    {
        for (int i = 0; i < itemWantAdd.m_AryOptions.Num(); i++)
        {
            auto Context = m_TargetAbilitySys->MakeEffectContext();
            Context.AddSourceObject(m_TargetAbilitySys->GetOwner());
            //
            FOptionSpec CurrentOption = itemWantAdd.m_AryOptions[i];
            //0918
            //장비종류마다 옵션 테이블 존제
            //장비 종류마다 옵션테이블 로우와 매칭되는 게임이펙트 한개 그리고 모디파이어 태그 존재
            //고유아이템은 if문으로 따로 처리, 지금 은 생략
            //고로 장비 종류에 맞춰서 옵션 테이블에서 선택되게 추가작업필요
            //
            //
            FGameplayEffectSpecHandle NewHandle = m_TargetAbilitySys->MakeOutgoingSpec(
                GameplayEffect, 1, Context);
            
            NewHandle.Data.Get()->SetSetByCallerMagnitude(ItemTypeWantAdd->m_Options[i].GetRow<FOption>("")->m_OptionTag,
                                                          CurrentOption.m_fValue);
            //
            m_ArySlots[droppedIndex]->m_OptionHandle = m_TargetAbilitySys->ApplyGameplayEffectSpecToTarget(
                *NewHandle.Data.Get(), m_TargetAbilitySys);
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

    // PRINTF("CurrentStance: %s", *EnumToStr(EAnimStance, m_CurrentStance));
    //
    // for (int i = 0; i < static_cast<int>(ESlots::Length); i++)
    // {
    //     PRINTF("Slot: %s - EquipItem: %s - Occupied: %hs", *EnumToStr(ESlots, (ESlots)i),
    //            *EnumToStr(EItemType, GetEquippedItem(i)), !m_ArySlots[i]->m_Item.IsEmpty() ? "Y" : "N");
    // }
}

const FItemType* UEquipmentSystem::GetEquippedItem(ESlotsEquipAry slot)
{
    return m_ArySlots[(int)slot]->m_EquippedType;
}

const FItemType* UEquipmentSystem::GetEquippedItem(int slotIndex)
{
    return m_ArySlots[slotIndex]->m_EquippedType;
}

void UEquipmentSystem::SetItemAry(TArray<FItemInstance>& equipSlot)
{
    for (int i = 0; i < m_ArySlots.Num(); i++)
    {
        if (!equipSlot[i].m_ItemData)
        {
            continue;
        }

        AddItem(i, equipSlot[i]);
    }
}



TArray<FEquipSlot*>& UEquipmentSystem::GetArySlotPtr()
{
    return m_ArySlots;
}
