#include "ItemDataTable.h"

UDataTable* UItemDataTable::GetTierTable = nullptr;
UDataTable* UItemDataTable::GetItemTable = nullptr;
UDataTable* UItemDataTable::GetItemTypeTable = nullptr;
UDataTable* UItemDataTable::GetAnimStanceTable = nullptr;

FItemInstance::FItemInstance(const FItemData* itemData, FName tierID, int gridIndex, IItemHolder* holder,
                             TArray<FOptionSpec>& aryUseEffect, const FItemTier* itemTier)
{
    m_ItemData = itemData;
    m_ItemID = m_ItemData->m_ItemID;
    m_nCurrentStack = m_ItemData->m_nInitStack;
    m_nGridIndex = gridIndex;
    m_Holder = holder;
    m_nMaxStack = m_ItemData->m_nMaxStack;
    m_bStackable = m_ItemData->m_bStackable;
    m_AryOptions = aryUseEffect;
    m_TierID = tierID;

    if (!itemTier)
    {
        m_ItemTier = UItemDataTable::GetItemTierPtr(m_TierID);
    }
    else
    {
        m_ItemTier = itemTier;
    }
}



UItemDataTable::UItemDataTable()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundTierTable(
        TEXT("DataTable'/Game/DataTables/Items/TierTable.TierTable'"));
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
    //DataTable'/Game/DataTables/Items/ItemTypeTable.ItemTypeTable'
    //DataTable'/Game/DataTables/Items/AnimStanceTable.AnimStanceTable'
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

FItemData::FItemData(): m_SkEquipment(nullptr), m_StEquipment(nullptr), m_ItemMesh(nullptr), m_ItemIcon(nullptr)
{
    m_bStackable = true;
    m_nInitStack = 1;
    m_nMaxStack = 99; 

    m_nSellValue = 100;
    m_bEquipable = true;

    m_ItemID = "NeedName";
    m_ItemType.DataTable = UItemDataTable::GetItemTypeTable;
}

FAnimStance::FAnimStance()
{
    m_fStancePriority = 1.0f;
    m_AryRightHandNeed.Add(FItemTypeHandle());
    m_AryLeftHandNeed.Add(FItemTypeHandle());
}

