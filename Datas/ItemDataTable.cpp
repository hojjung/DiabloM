#include "ItemDataTable.h"

UDataTable* UItemDataTable::GetTierTable = nullptr;
UDataTable* UItemDataTable::GetItemTable = nullptr;

UItemDataTable::UItemDataTable()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundTierTable(
        TEXT("DataTable'/Game/DataTables/Items/TierTable.TierTable'"));
    UItemDataTable::GetTierTable = FoundTierTable.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> FoundItemTable(
        TEXT("DataTable'/Game/DataTables/Items/DefaultItemTable.DefaultItemTable'"));
    UItemDataTable::GetItemTable = FoundItemTable.Object;

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


FItemData::FItemData(): m_SkEquipment(nullptr), m_StEquipment(nullptr), m_ItemMesh(nullptr), m_ItemIcon(nullptr)
{
    m_ItemTier.DataTable = UItemDataTable::GetTierTable;
    m_ItemTier.RowName = "Normal";

    m_ItemType = EItemType::Misc;
    m_bStackable = true;
    m_nInitStack = 1;
    m_nMaxStack = 99;

    m_nSellValue = 100;
    m_bEquipable = true;

    m_ItemID = "NeedName";
//OnDataTableChangedDelegate
    if(m_Options.Num()>0)
    {
        for(auto Op : m_Options)
        {
            Op.DataTable=UOptionDataTable::GetOptionTable;
        }
    }
}