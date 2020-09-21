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
    return *GetOptionTable->FindRow<FOption>(id,"Invalid Option ID");
}

const FOption* UOptionDataTable::GetOptionPtr(FName id)
{
    return GetOptionTable->FindRow<FOption>(id,"Invalid Option ID");
}

FText UOptionDataTable::GetOptionText(const FOptionSpec& spec)
{
    return UOptionDataTable::GetOption(spec.m_OptionID).GetOptionFormat(spec.m_fValue);
}

FText FOptionSpec::GetOptionText() const
{
    return UOptionDataTable::GetOptionText(*this);
}