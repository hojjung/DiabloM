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
    }
    if(m_ArySlots[index]->m_AryAbilitySpec.Num()>0)
    {
        for(FGameplayAbilitySpecHandle AbilSpec : m_ArySlots[index]->m_AryAbilitySpec)
        {
            m_TargetAbilitySys->CancelAbilityHandle(AbilSpec);
        }
        
        m_ArySlots[index]->m_AryAbilitySpec.Reset();
    }
    m_OnOptionChanged.Broadcast();
    
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

void UEquipmentSystem::SetUnequipItemToSlots(TArray<TArray<FItemTypeHandle>>&& aryAryItemType)
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

//이펙트 한개가 아니라
//어빌리티 한개로 핸들
//그냥 이펙트한개로 해볼것?

//한개의 어빌리티에게 어트리뷰트 옵션을 모아서 이펙트로 만들고 적용
//그 어빌리티에게 다른 옵션 어빌리티를 적용


//제약조건
//1.태그 조건부는 GE가 없고 GA가 가지고 있다.
//2.단순스텟옵션은 데이터 테이블에서 끝내고싶다 일일히 블프를 만드는게 아니라
//3.어떤 방법을 쓰든 장비 해제시 해제를 위해 핸들은 한개만 나와야한다.

//모든 옵션을 GA로 만들면 전부 동적할당 필요함

//GE가 GA의 인스턴스를 끼워줄 방법?
//GA의 배열을 가진 GA로 만들것?

//GA기능만 상수 블프로 만들면되지 않나?
//이 체제의 문제점은 조건부 옵션,여기서 이펙트 만들때 수동으로 체크?
//조건부 시발 만들지마 졷같네



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

    if (m_ArySlots[droppedIndex]->m_EquippedType && ItemTypeWantAdd == m_ArySlots[droppedIndex]->m_EquippedType)
    {
        return true;
    }


    if (GetItem(droppedIndex).IsEmpty() && m_ArySlots[droppedIndex]->m_bIsOccupied) //해당슬롯이 비어있는데 할당됐다면,양손무기의 반대손이라면
    {
        //양손무기 왼손
        return false;
    }

    if (!TEST_BIT(ItemTypeWantAdd->m_EquipableSlot, m_ArySlots[droppedIndex]->m_Slot)) //무기의 장착 가능 슬롯인지
    {
        return false;
    }

    for (FItemType* CantEquipType : m_ArySlots[droppedIndex]->m_AryCantEquipable) //클래스가 달라서 못낌
    {
        if (CantEquipType == ItemTypeWantAdd)
        {
            PRINTF("CantEquip Class Type");
            return false;
        }
    }


    for (auto Slot : m_ArySlots)
    {
        if (TEST_BIT(ItemTypeWantAdd->m_EquipInterruptSlot, Slot->m_Slot)) //방해 슬롯이 할당되어있다면,
        {
            if (!Slot->m_Item.IsEmpty()) //&&Slot->m_bIsOccupied
            {
                return false;
            }
        }
    }

    return true;
}

void UEquipmentSystem::SetItem(int droppedIndex, FItemInstance& itemWantAdd)
{
    int ItemLevel = itemWantAdd.m_nItemLevel;
    
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
    if (itemWantAdd.m_AryOptions.Num() < 1)//No Option
    {
        OnItemSlotChanged(droppedIndex);//JustEnd
        return;
    }

    FGameplayEffectContextHandle Context = m_TargetAbilitySys->MakeEffectContext();

    Context.AddSourceObject(m_TargetAbilitySys->GetOwner());

    FGameplayEffectSpecHandle NewHandle = m_TargetAbilitySys->MakeOutgoingSpec(UOptionDataTable::GetGEItemOption,ItemLevel,Context);
    
    for(FOptionSpec& OP : itemWantAdd.m_AryOptions)
    {
        if(OP.m_DataOption->m_GrantAbility)//Ability OO
        {
            FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(OP.m_DataOption->m_GrantAbility,ItemLevel,INDEX_NONE,
                m_TargetAbilitySys->GetOwner());
            
            FGameplayAbilitySpecHandle AbilityHandle = m_TargetAbilitySys->GiveAbility(AbilitySpec);
            
            m_ArySlots[droppedIndex]->m_AryAbilitySpec.Emplace(AbilityHandle);
        }
        else
        {
            NewHandle.Data.Get()->SetSetByCallerMagnitude(
                OP.m_DataOption->m_TagAttribute,
                OP.m_fValue);
        }
    }
    if(NewHandle.Data.Get()->SetByCallerTagMagnitudes.Num()>0)
    {
        m_ArySlots[droppedIndex]->m_OptionHandle = m_TargetAbilitySys->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
    }
    
    m_OnOptionChanged.Broadcast();

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
        // PRINTF("Slot: %s - EquipItem: %s - Occupied: %hs", *EnumToStr(ESlotsEquipAry, (ESlotsEquipAry)i),
        //        *GetEquippedItem(i)->m_ShowingName.ToString(), !m_ArySlots[i]->m_Item.IsEmpty() ? "Y" : "N");
    }
}

const FItemType* UEquipmentSystem::GetEquippedItemType(ESlotsEquipAry slot)
{
    return m_ArySlots[(int)slot]->m_EquippedType;
}

const FItemType* UEquipmentSystem::GetEquippedItemType(int slotIndex)
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
