
#include "ShopItemTable.h"

UDataTable*  UShopItemTable::GetShopItemData=nullptr;

FShopItemSell::FShopItemSell()
{
    m_bAutoLevel=false;
    
    m_bAutoTier=false;

    m_nLevel=1;
}

FShopItemSellHandle::FShopItemSellHandle()
{
    DataTable=UShopItemTable::GetShopItemData;
}

UShopItemTable::UShopItemTable()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundShopItemTable(
          TEXT("DataTable'/Game/DataTables/Items/ShopDefaultTable.ShopDefaultTable'"));
    
    UShopItemTable::GetShopItemData=FoundShopItemTable.Object;
}

const FShopItemSellData& UShopItemTable::GetShopSellItemData(FName id)
{
    return  *UShopItemTable::GetShopItemData->FindRow<FShopItemSellData>(id,"");
}

const FShopItemSellData* UShopItemTable::GetShopSellItemDataPtr(FName id)
{
    return  UShopItemTable::GetShopItemData->FindRow<FShopItemSellData>(id,"");
}
