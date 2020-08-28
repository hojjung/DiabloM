
#include "ItemDataTable.h"

UDataTable* UItemDataTable::GetOptionTable = nullptr;
UDataTable* UItemDataTable::GetTierTable = nullptr;
UDataTable* UItemDataTable::GetDefaultItemTable = nullptr;

UItemDataTable::UItemDataTable(const FObjectInitializer& objInit): Super(objInit)
{
        static ConstructorHelpers::FObjectFinder<UDataTable> FoundOptionTable(
                TEXT("DataTable'/Game/DataTables/EquipOptionTable.EquipOptionTable'"));
        UItemDataTable::GetOptionTable = FoundOptionTable.Object;

        static ConstructorHelpers::FObjectFinder<UDataTable> FoundTierTable(
                TEXT("DataTable'/Game/DataTables/TierTable.TierTable'"));
        UItemDataTable::GetTierTable = FoundTierTable.Object;

        static ConstructorHelpers::FObjectFinder<UDataTable> FoundItemTable(
                TEXT("DataTable'/Game/DataTables/DefaultItemTable.DefaultItemTable'"));
        UItemDataTable::GetDefaultItemTable = FoundItemTable.Object;
}
