#include "DungeonDataTable.h"

UDataTable* UDungeonDataTable::GetDungeonTable = nullptr;

TArray<FDungeonDataTableRow*> UDungeonDataTable::AryDgData;

UDungeonDataTable::UDungeonDataTable()
{
   static ConstructorHelpers::FObjectFinder<UDataTable> FoundDungeon(
              TEXT("DataTable'/Game/DataTables/Dungeon/DungeonData.DungeonData'"));
   GetDungeonTable = FoundDungeon.Object;

	
}

const FDungeonDataTableRow& UDungeonDataTable::GetDungeonData(FName id)
{
	return *GetDungeonTable->FindRow<FDungeonDataTableRow>(id,"");
}

const FDungeonDataTableRow* UDungeonDataTable::GetDungeonDataPtr(FName id)
{
	return GetDungeonTable->FindRow<FDungeonDataTableRow>(id,"");
}

