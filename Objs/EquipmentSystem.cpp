#include "EquipmentSystem.h"

#include "Managers/ItemManager.h"

void EquipmentSystem::Init()
{
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
        EquipmentSystem::SetStanceAllNull;
    m_StanceFPtr[static_cast<int>(EItemType::None)][static_cast<int>(EItemType::OneHandSword)] = &
        EquipmentSystem::SetStanceNull;
    m_StanceFPtr[static_cast<int>(EItemType::None)][static_cast<int>(EItemType::Dagger)] = &
        EquipmentSystem::SetStanceNull;
    m_StanceFPtr[static_cast<int>(EItemType::None)][static_cast<int>(EItemType::Shield)] = &
        EquipmentSystem::SetStanceNull;
    m_StanceFPtr[static_cast<int>(EItemType::None)][static_cast<int>(EItemType::TwohandSword)] = &
        EquipmentSystem::SetStanceNull;
    m_StanceFPtr[static_cast<int>(EItemType::None)][static_cast<int>(EItemType::Katana)] = &
        EquipmentSystem::SetStanceNull;
    //				  EItemType
    m_StanceFPtr[static_cast<int>(EItemType::OneHandSword)][static_cast<int>(EItemType::OneHandSword)] = &
        EquipmentSystem::SetStanceDual;
    m_StanceFPtr[static_cast<int>(EItemType::Dagger)][static_cast<int>(EItemType::OneHandSword)] = &
        EquipmentSystem::SetStanceDual;
    m_StanceFPtr[static_cast<int>(EItemType::OneHandSword)][static_cast<int>(EItemType::Dagger)] = &
        EquipmentSystem::SetStanceDual;
    m_StanceFPtr[static_cast<int>(EItemType::Dagger)][static_cast<int>(EItemType::Dagger)] = &
        EquipmentSystem::SetStanceDual;
    //				  EItemType
    m_StanceFPtr[static_cast<int>(EItemType::OneHandSword)][static_cast<int>(EItemType::Shield)] = &
        EquipmentSystem::SetStanceShield;
    m_StanceFPtr[static_cast<int>(EItemType::Dagger)][static_cast<int>(EItemType::Shield)] = &
        EquipmentSystem::SetStanceShield;
    //				  EItemType
    m_StanceFPtr[static_cast<int>(EItemType::OneHandSword)][static_cast<int>(EItemType::None)] = &
        EquipmentSystem::SetStanceOneHand;
    //				  EItemType
    m_StanceFPtr[static_cast<int>(EItemType::Dagger)][static_cast<int>(EItemType::None)] = &
        EquipmentSystem::SetStanceDagger;
    //				  EItemType
    m_StanceFPtr[static_cast<int>(EItemType::Katana)][static_cast<int>(EItemType::None)] = &
        EquipmentSystem::SetStanceKatana;
    //				  EItemType
    m_StanceFPtr[static_cast<int>(EItemType::TwohandSword)][static_cast<int>(EItemType::None)] = &
        EquipmentSystem::SetStanceTwoHand;
}

bool EquipmentSystem::AddItem(int droppedIndex, FItemInstance& itemWantAdd) //drag된 대상이 어떤 아이템을 가졌는지 알방법이 없음
{
    if (!CheckSlotValid(droppedIndex, itemWantAdd))
    {
        return false; //애초부터 안맞음 혹은 양손검 등으로 빈 오큐파이일때
    }

    if (!CheckSlotOccupied(droppedIndex))
    {
        SetItem(droppedIndex, itemWantAdd); //그냥 비어있던 슬롯
        itemWantAdd.m_Holder->RemoveItem(itemWantAdd);
        return true;
    }

    if (m_ArySlots[droppedIndex]->m_Item.m_ItemID == NAME_None)
    {
        PRINTF("Probably Katana or Twohandsword left hand blocked");
        return false;
    }

    return SwapMove(GetItem(droppedIndex), itemWantAdd);
}

void EquipmentSystem::RemoveItem(FItemInstance& itemWantErase)
{
    RemoveItemByIndex(itemWantErase.m_nGridIndex);
}

void EquipmentSystem::RemoveItemByIndex(int index)
{
    m_ArySlots[index]->SetOccupie(false);
    m_ArySlots[index]->m_EquippedType = EItemType::None;
    m_ArySlots[index]->m_Item.ClearData();

    OnItemSlotChanged(index);
}

void EquipmentSystem::OnItemSlotChanged(int index)
{
    CalculateStance();
    m_ItemChanged.Broadcast(index, GetItem(index));
    //이펙트 부여
    //무기 생성
    //
}

void EquipmentSystem::CalculateStance()
{
    (this->*m_StanceFPtr[static_cast<int>(GetEquippedItem(ESlots::WeaponRight))][static_cast<int>(GetEquippedItem(
        ESlots::WeaponLeft))])();

    m_OnStanceChanged.Broadcast(m_CurrentStance);
}


bool EquipmentSystem::CheckSlotOccupied(int index)
{
    return m_ArySlots[index]->m_bIsOccupied;
}

bool EquipmentSystem::SwapMove(FItemInstance& Drop, FItemInstance& Drag)
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

bool EquipmentSystem::CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd)
{
    auto ItemTypeWantAdd = ItemManager::GetItemData(itemWantAdd.m_ItemID).m_ItemType;

    if (!m_ArySlots[droppedIndex]->m_AbleEquipSlot[static_cast<int>(ItemTypeWantAdd)]) //못끼는 슬록이면 실패
    {
        return false;
    }


    if (m_ArySlots[droppedIndex]->m_Item.IsValid()) //비어있음
    {
        //이러면 양손검 빨간색도 떠버린다
        return true;
    }

    if (ItemTypeWantAdd == EItemType::TwohandSword || ItemTypeWantAdd == EItemType::Katana) //낄무기가 양손무기일걍우
    {
        auto ItemTypeDroppedBefore = ItemManager::GetItemData(m_ArySlots[droppedIndex]->m_Item.m_ItemID).m_ItemType;
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

void EquipmentSystem::SetItem(int droppedIndex, FItemInstance& itemWantAdd)
{
    m_ArySlots[droppedIndex]->m_Item = itemWantAdd;
    m_ArySlots[droppedIndex]->SetOccupie(true);
    m_ArySlots[droppedIndex]->m_EquippedType = ItemManager::GetItemData(m_ArySlots[droppedIndex]->m_Item.m_ItemID).
        m_ItemType;
    //
    m_ArySlots[droppedIndex]->m_Item.m_nGridIndex = droppedIndex;
    m_ArySlots[droppedIndex]->m_Item.m_Holder = this;

    OnItemSlotChanged(droppedIndex);
}

FItemInstance& EquipmentSystem::GetItem(int index)
{
    return m_ArySlots[index]->m_Item;
}

void EquipmentSystem::PrintEquipStats()
{
    PRINTF("----EquipPrint----");

    PRINTF("CurrentStance: %s", *EnumToStr(EAnimStance, m_CurrentStance));

    for (int i = 0; i < static_cast<int>(ESlots::Length); i++)
    {
        PRINTF("Slot: %s - EquipItem: %s", *EnumToStr(ESlots, (ESlots)i), *EnumToStr(EItemType, GetEquippedItem(i)));
    }
}

EItemType EquipmentSystem::GetEquippedItem(ESlots slot)
{
    return m_ArySlots[static_cast<int>(slot)]->m_EquippedType;
}

EItemType EquipmentSystem::GetEquippedItem(int slotIndex)
{
    return m_ArySlots[slotIndex]->m_EquippedType;
}


#pragma region SetFPtr


void EquipmentSystem::SetStanceAllNull()
{
    m_WeaponLeft.SetOccupie(false);
    //아래랑 똑같아보이지만 아래의 경우는 왼손에 무기가 들려있으나 오른손에 무기가 없어서 공격을 못하는것임
    m_WeaponRight.SetEquipableType(EItemType::Katana, true);
    m_WeaponRight.SetEquipableType(EItemType::TwohandSword, true);
    m_CurrentStance = EAnimStance::None;
}

void EquipmentSystem::SetStanceNull()
{
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, true);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, true);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, true);
    //
    if (!m_WeaponLeft.m_bIsOccupied)
    {
        m_WeaponRight.SetEquipableType(EItemType::Katana, true);
        m_WeaponRight.SetEquipableType(EItemType::TwohandSword, true);
    }
    m_CurrentStance = EAnimStance::None;
}

void EquipmentSystem::SetStanceOneHand()
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

void EquipmentSystem::SetStanceDual()
{
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, true);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, true);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, true);
    //
    m_WeaponRight.SetEquipableType(EItemType::Katana, false);
    m_WeaponRight.SetEquipableType(EItemType::TwohandSword, false);
    m_CurrentStance = EAnimStance::DualSword;
}

void EquipmentSystem::SetStanceShield()
{
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, true);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, true);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, true);
    //
    m_WeaponRight.SetEquipableType(EItemType::Katana, false);
    m_WeaponRight.SetEquipableType(EItemType::TwohandSword, false);
    m_CurrentStance = EAnimStance::Shield;
}

void EquipmentSystem::SetStanceDagger()
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

void EquipmentSystem::SetStanceKatana()
{
    //
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, false);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, false);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, false);
    //
    m_WeaponLeft.SetOccupie(true);
    m_CurrentStance = EAnimStance::Katana;
}

void EquipmentSystem::SetStanceTwoHand()
{
    //
    m_WeaponLeft.SetEquipableType(EItemType::Dagger, false);
    m_WeaponLeft.SetEquipableType(EItemType::OneHandSword, false);
    m_WeaponLeft.SetEquipableType(EItemType::Shield, false);
    //
    m_WeaponLeft.SetOccupie(true);
    m_CurrentStance = EAnimStance::TwohandSword;
}


#pragma endregion
