// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem.h"


#include "AbilitySystem/Components/DiabloAbilitySystemComp.h"
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

    m_Head.m_Slot = ESlotsEquipAry::Head;
    m_Neck.m_Slot = ESlotsEquipAry::Neck;
    m_Torso.m_Slot = ESlotsEquipAry::Torso;
    m_Waist.m_Slot = ESlotsEquipAry::Waist;
    m_Leg.m_Slot = ESlotsEquipAry::Leg;
    m_Hand.m_Slot = ESlotsEquipAry::Hand;
    m_Shoulder.m_Slot = ESlotsEquipAry::Shoulder;
    m_WeaponRight.m_Slot = ESlotsEquipAry::WeaponRight;
    m_WeaponLeft.m_Slot = ESlotsEquipAry::WeaponLeft;
    m_FingerRight.m_Slot = ESlotsEquipAry::FingerRight;
    m_FingerLeft.m_Slot = ESlotsEquipAry::FingerLeft;

    UItemDataTable::GetAnimStanceTable->GetAllRows("NoAnimDataTable", m_AryAnimStances);

    SetUnequipItemToSlots(
        UCharacterDataTable::GetPlayerEntity(USaveLoadManager::Get->GetCurrentPlayerClassName()).GetUnequipableAry());


    CalculateAnimStance();
}

bool UEquipmentSystem::AddItem(int droppedIndex, FItemInstance& itemWantAdd) //drag된 대상이 어떤 아이템을 가졌는지 알방법이 없음
{
    if (itemWantAdd.m_Holder && droppedIndex == itemWantAdd.m_nGridIndex && Cast<UEquipmentSystem>(itemWantAdd.m_Holder)
        == this)
    {
        return false;
    }

    if (!CheckSlotValid(droppedIndex, itemWantAdd))
    {
        return false; //애초부터 안맞음 혹은 양손검 등으로 빈 오큐파이일때
    }

    if (!CheckSlotOccupied(droppedIndex)) //비어있음
    {
        SetItem(droppedIndex, itemWantAdd); //그냥 비어있던 슬롯

        if (itemWantAdd.m_Holder)
        {
            itemWantAdd.m_Holder->RemoveItem(itemWantAdd);
        }

        return true;
    }

    //비어있지 않음?

    if (m_ArySlots[droppedIndex]->m_Item.m_ItemID == NAME_None) //양손등이라 못끼는거
    {
        PRINTF("Probably Katana or Twohandsword left hand blocked");
        return false;
    }

    //스왑
    return SwapMove(GetItem(droppedIndex), itemWantAdd);
}

void UEquipmentSystem::RemoveItem(FItemInstance& itemWantErase)
{
    RemoveItemByIndex(itemWantErase.m_nGridIndex);
}

void UEquipmentSystem::RemoveItemByIndex(int index)
{
    const FItemType* ItemTypeErase = m_ArySlots[index]->m_Item.m_ItemData->m_ItemType.GetRow<FItemType>("");

    m_ArySlots[index]->m_bIsOccupied = false;

    for (auto* Slot : m_ArySlots)
    {
        if (TEST_BIT(ItemTypeErase->m_EquipInterruptSlot, Slot->m_Slot))
        {
            Slot->m_bIsOccupied = false;
            m_EquipSlotChanged.Broadcast(m_ArySlots[index]->m_Item, *Slot);
        }
    }

    m_ArySlots[index]->m_EquippedType = nullptr;
    m_ArySlots[index]->m_Item.ClearData();
    //
    if (m_ArySlots[index]->m_OptionHandle.IsValid())
    {
        m_TargetAbilitySys->RemoveActiveGameplayEffect(m_ArySlots[index]->m_OptionHandle);
        m_OnOptionChanged.Broadcast();
    }
    OnItemSlotChanged(index);
}

void UEquipmentSystem::CalculateAnimStance()
{
    bool RightOk = false, LeftOk = false;

    FItemType* RightType = GetItem(static_cast<int>(ESlotsEquipAry::WeaponRight)).m_ItemData
                               ? GetItem(static_cast<int>(ESlotsEquipAry::WeaponRight)).m_ItemData->m_ItemType.GetRow<
                                   FItemType>("")
                               : nullptr;
    FItemType* LeftType = GetItem(static_cast<int>(ESlotsEquipAry::WeaponLeft)).m_ItemData
                              ? GetItem(static_cast<int>(ESlotsEquipAry::WeaponLeft)).m_ItemData->m_ItemType.GetRow<
                                  FItemType>("")
                              : nullptr;

    for (auto* AnimS : m_AryAnimStances)
    {
        for (auto& RightHandType : AnimS->m_AryRightHandNeed)
        {
            //RightHandType.IsNull()
            if (RightHandType.GetRow<FItemType>("") == RightType)
            {
                RightOk = true;
                break;;
            }
        }

        for (auto& LeftHandType : AnimS->m_AryLeftHandNeed)
        {
            if (LeftHandType.GetRow<FItemType>("") == LeftType)
            {
                LeftOk = true;
                break;;
            }
        }

        if (RightOk && LeftOk)
        {
            m_CurrentStance = AnimS;
            PRINTF("AnimSet:%s", *m_CurrentStance->m_ShowingText.ToString());
            ADiabloPlayerController::Get->GetPlayerPawn()->SetAnimStance(m_CurrentStance);
            break;;
        }

        RightOk = false;
        LeftOk = false;
    }
}

void UEquipmentSystem::SetUnequipItemToSlots(TArray<TArray<FItemTypeHandle>> aryAryItemType)
{
    for (int i = 0; i < aryAryItemType.Num(); i++)
    {
        TArray<FItemType*> UnequipAry;

            for (const FItemTypeHandle& DataTableUnequipHandle : aryAryItemType[i])
            {
                if (DataTableUnequipHandle.IsNull())
                {
                    continue;
                }
                UnequipAry.Add(DataTableUnequipHandle.GetRow<FItemType>(""));
            }

            m_ArySlots[i]->m_AryCantEquipable = UnequipAry;
    }
}

void UEquipmentSystem::OnItemSlotChanged(int index)
{
    CalculateAnimStance();
    m_ItemChanged.Broadcast(index, GetItem(index));
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
    auto DropBackup = Drop;

    RemoveItemByIndex(DropIndex);
    Drag.m_Holder->RemoveItem(Drag);

    Drag.m_Holder->SetItem(DragIndex, DropBackup);
    SetItem(DropIndex, DragBackup);

    return true;
}

bool UEquipmentSystem::CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd)
{
    const FItemType* ItemTypeWantAdd = itemWantAdd.m_ItemData->m_ItemType.GetRow<FItemType>("");

    if (m_ArySlots[droppedIndex]->m_EquippedType&&ItemTypeWantAdd == m_ArySlots[droppedIndex]->m_EquippedType)
    {
        return true;   
    }
    
    
    if (GetItem(droppedIndex).IsEmpty() && m_ArySlots[droppedIndex]->m_bIsOccupied)//해당슬롯이 비어있는데 할당됐다면,양손무기의 반대손이라면
    {
        //양손무기 왼손
        return false;
    }

    if (!TEST_BIT(ItemTypeWantAdd->m_EquipableSlot, m_ArySlots[droppedIndex]->m_Slot))//무기의 장착 가능 슬롯인지
    {
        return false;
    }

    for (FItemType* CantEquipType : m_ArySlots[droppedIndex]->m_AryCantEquipable)//클래스가 달라서 못낌
    {
        if (CantEquipType == ItemTypeWantAdd)
        {
            PRINTF("CantEquip Class Type");
            return false;
        }
    }


    for (auto Slot : m_ArySlots)
    {
        if (TEST_BIT(ItemTypeWantAdd->m_EquipInterruptSlot, Slot->m_Slot))//방해 슬롯이 할당되어있다면,
        {
            if (!Slot->m_Item.IsEmpty())//&&Slot->m_bIsOccupied
            {
                return false;
            }
        }
    }

    // if (m_ArySlots[droppedIndex]->m_Item.IsEmpty()) //비어있지않음
    // {
    //     return true;
    // }
    //장비가 끼워지고 나면, 인터럽팅 슬롯을 빨갛게 칠해줘야하고
    //장비들을 돌려서 현재 애님스턴스를 반환해야함
    return true;
}

void UEquipmentSystem::SetItem(int droppedIndex, FItemInstance& itemWantAdd)
{
    const FItemType* ItemTypeWantAdd = itemWantAdd.m_ItemData->m_ItemType.GetRow<FItemType>("");
    m_ArySlots[droppedIndex]->m_Item = itemWantAdd;
    m_ArySlots[droppedIndex]->m_EquippedType = ItemTypeWantAdd;
    //
    m_ArySlots[droppedIndex]->m_Item.m_nGridIndex = droppedIndex;
    m_ArySlots[droppedIndex]->m_Item.m_Holder = this;
    m_ArySlots[droppedIndex]->m_bIsOccupied = true;


    for (auto* Slot : m_ArySlots)
    {
        if (TEST_BIT(ItemTypeWantAdd->m_EquipInterruptSlot, Slot->m_Slot))
        {
            Slot->m_bIsOccupied = true;
            m_EquipSlotChanged.Broadcast(m_ArySlots[droppedIndex]->m_Item, *Slot);
        }
    }
    //UG
    TSubclassOf<UGameplayEffect> GameplayEffect = ItemTypeWantAdd->m_OptionGameEffect;

    if (itemWantAdd.m_AryOptions.Num() > 0)
    {
        auto Context = m_TargetAbilitySys->MakeEffectContext();
        Context.AddSourceObject(m_TargetAbilitySys->GetOwner());
        FGameplayEffectSpecHandle NewHandle = m_TargetAbilitySys->MakeOutgoingSpec(GameplayEffect, 1, Context);

        for (int i = 0; i < itemWantAdd.m_AryOptions.Num(); i++)
        {
            FOptionSpec CurrentOption = itemWantAdd.m_AryOptions[i];

            NewHandle.Data.Get()->SetSetByCallerMagnitude(
                itemWantAdd.m_AryOptions[i].m_DataOption->m_OptionTag,
                CurrentOption.m_fValue);
            //
        }

        m_ArySlots[droppedIndex]->m_OptionHandle = m_TargetAbilitySys->ApplyGameplayEffectSpecToSelf(
            *NewHandle.Data.Get());

        //수동으로 불러줄것
        //그렇게 되면 그냥 위젯 전체 업데이트 함수 만들어놓을것
        m_OnOptionChanged.Broadcast();
    }

    OnItemSlotChanged(droppedIndex);
}

FItemInstance& UEquipmentSystem::GetItem(int index)
{
    return m_ArySlots[index]->m_Item;
}

FItemInstance& UEquipmentSystem::GetItem(ESlotsEquipAry index)
{
    return m_ArySlots[(int)index]->m_Item;
}

void UEquipmentSystem::PrintEquipStats()
{
    PRINTF("----EquipPrint----");

    //PRINTF("CurrentStance: %s", *EnumToStr(EAnimStance, m_CurrentStance));

    for (int i = 0; i < static_cast<int>(ESlotsEquipAry::Length); i++)
    {
        PRINTF("Slot: %s - EquipItem: %s - Occupied: %hs", *EnumToStr(ESlotsEquipAry, (ESlotsEquipAry)i),
               *GetEquippedItem(i)->m_ShowingName.ToString(), !m_ArySlots[i]->m_Item.IsEmpty() ? "Y" : "N");
    }
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
