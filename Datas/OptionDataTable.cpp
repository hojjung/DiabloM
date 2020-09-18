#include "OptionDataTable.h"

UDataTable* UOptionDataTable::GetOptionTable = nullptr;

UOptionDataTable::UOptionDataTable()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundOptionTable(
              TEXT("DataTable'/Game/DataTables/Options/EquipOptionTable.EquipOptionTable'"));
    UOptionDataTable::GetOptionTable = FoundOptionTable.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> FoundOptionGETable(
              TEXT("DataTable'/Game/DataTables/Options/OptionGETable.OptionGETable'"));
    UOptionDataTable::GetOptionGETable = FoundOptionGETable.Object;
}

const FOption& UOptionDataTable::GetOption(FName id)
{
    return *GetOptionTable->FindRow<FOption>(id,"");
}

const FOption* UOptionDataTable::GetOptionPtr(FName id)
{
    return GetOptionTable->FindRow<FOption>(id,"");
}

const FOptionGE& UOptionDataTable::GetOptionGE(FName id)
{
    return *GetOptionGETable->FindRow<FOptionGE>(id,"");
}

const FOptionGE* UOptionDataTable::GetOptionGEPtr(FName id)
{
    return GetOptionGETable->FindRow<FOptionGE>(id,"");
}

