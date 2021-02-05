#include "ItemDataTable.h"
#include "AbilitySystem/Ability/PlayerAbility/BaseSkillTemplate/PlayerBaseAttack.h"
#include "AbilitySystem/GameEffect/ItemOptionGameEffect.h"

UDataTable* UItemDataTable::GetTierTable = nullptr;
UDataTable* UItemDataTable::GetItemTable = nullptr;
UDataTable* UItemDataTable::GetItemTypeTable = nullptr;
UDataTable* UItemDataTable::GetAnimStanceTable = nullptr;
//GetUniqueItemTypeTable

FItemInstance::FItemInstance(const FItemData* itemData, FName tierID, int gridIndex, IItemHolder* holder,
                             TArray<FOptionSpec>& aryUseEffect, int lv)
{
    m_ItemData = itemData;
    m_ItemID = m_ItemData->m_ItemID;
    m_nCurrentStack = m_ItemData->m_ItemType.GetRow<FItemType>("")->m_nInitStack;
    m_nGridIndex = gridIndex;
    m_Holder = holder;
    m_AryOptions = aryUseEffect;
    m_nItemLevel=lv;

    m_TierID = tierID;
    m_ItemTier = UItemDataTable::GetItemTierPtr(m_TierID);

    m_fBuyCost=(GetFullStackSellValue()*2.f);


    if(m_Holder)
    {
        if(IsEmpty() || m_nCurrentStack<1)
        {
            PRINTF("ITEMInst WTF");
        }
    }
}



UItemDataTable::UItemDataTable()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundTierTable(
        TEXT("DataTable'/Game/DataTables/Items/ItemTierTable.ItemTierTable'"));
    UItemDataTable::GetTierTable = FoundTierTable.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> FoundItemTable(
        TEXT("DataTable'/Game/DataTables/Items/DefaultItemTable.DefaultItemTable'"));
    UItemDataTable::GetItemTable = FoundItemTable.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> FoundItemTypeTable(
      TEXT("/DataTable'/Game/DataTables/Items/ItemTypeTable.ItemTypeTable'"));
    UItemDataTable::GetItemTypeTable = FoundItemTypeTable.Object;
    
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundAnimTable(
      TEXT("DataTable'/Game/DataTables/Items/AnimStanceTable.AnimStanceTable'"));
    UItemDataTable::GetAnimStanceTable = FoundAnimTable.Object;
}

const FItemTier& UItemDataTable::GetItemTier(FName id)
{
    return *GetTierTable->FindRow<FItemTier>(id, "");
}

const FItemTier* UItemDataTable::GetItemTierPtr(FName id)
{
    return GetTierTable->FindRow<FItemTier>(id, "");
}

const FItemData& UItemDataTable::GetItemData(FName id)
{
    return *GetItemTable->FindRow<FItemData>(id, "");
}

const FItemData* UItemDataTable::GetItemDataPtr(FName id)
{
    return GetItemTable->FindRow<FItemData>(id, "");
}

const FItemType& UItemDataTable::GetItemType(FName id)
{
    return *GetItemTypeTable->FindRow<FItemType>(id, "");
}

const FItemType* UItemDataTable::GetItemTypePtr(FName id)
{
    return GetItemTypeTable->FindRow<FItemType>(id, "");
}

const FAnimStance& UItemDataTable::GetAnimStance(FName id)
{
    return *GetAnimStanceTable->FindRow<FAnimStance>(id, "");
}

const FAnimStance* UItemDataTable::GetAnimStancePtr(FName id)
{
    return GetAnimStanceTable->FindRow<FAnimStance>(id, "");
}


FItemTypeHandle::FItemTypeHandle()
{
    FDataTableRowHandle::DataTable = UItemDataTable::GetItemTypeTable;
    FDataTableRowHandle::RowName=NAME_None;
}

FItemTierHandle::FItemTierHandle()
{
    DataTable = UItemDataTable::GetTierTable;
}

FItemType::FItemType(): m_ItemTypeIcon(nullptr), m_DropItemMesh(nullptr), m_bEquipable(false), m_bStackable(false),
                        m_nInitStack(1),
                        m_nMaxStack(99),
                        m_EquipableSlot(),
                        m_EquipInterruptSlot()
{
    m_fSellValueRate = 1.f;
    m_TypeID = "SetSameWithRowID";
    m_ShowingName = FText::FromString("ShowNameExOneHandSword");
}

FItemData::FItemData(): m_ItemIcon(nullptr)
{
    m_nDefaultSellValue = 100;
    
    m_ItemID = "NeedName";
    m_ItemType.DataTable = UItemDataTable::GetItemTypeTable;
}

FAnimStance::FAnimStance()
{
    m_AryRightHandNeed.Add(FItemTypeHandle());
    m_AryLeftHandNeed.Add(FItemTypeHandle());
}

