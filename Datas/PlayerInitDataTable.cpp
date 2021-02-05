#include "PlayerInitDataTable.h"


UDataTable* UPlayerInitDataTable::GetPlayerItemTable = nullptr;

UPlayerInitDataTable::UPlayerInitDataTable()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundTable5(
          TEXT("DataTable'/Game/DataTables/PlayerCreation/PlayerClassSelectTable.PlayerClassSelectTable'"));
    UPlayerInitDataTable::GetPlayerItemTable = FoundTable5.Object;
    
        
}

const FPlayerInitItemTableRow& UPlayerInitDataTable::GetPlayerItem(FName id)
{
	return *GetPlayerItemTable->FindRow<FPlayerInitItemTableRow>(id,"");
}

const FPlayerInitItemTableRow* UPlayerInitDataTable::GetPlayerItemPtr(FName id)
{
	return GetPlayerItemTable->FindRow<FPlayerInitItemTableRow>(id,"");    
}

