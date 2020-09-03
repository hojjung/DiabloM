#include "OptionDataTable.h"

UDataTable* UOptionDataTable::GetOptionTable = nullptr;

UOptionDataTable::UOptionDataTable()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundOptionTable(
              TEXT("DataTable'/Game/DataTables/Options/EquipOptionTable.EquipOptionTable'"));
    UOptionDataTable::GetOptionTable = FoundOptionTable.Object;
}

const FOption& UOptionDataTable::GetOption(FName id)
{
    return *GetOptionTable->FindRow<FOption>(id,"");
}

const FOption* UOptionDataTable::GetOptionPtr(FName id)
{
    return GetOptionTable->FindRow<FOption>(id,"");
}

