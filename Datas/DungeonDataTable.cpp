#include "DungeonDataTable.h"


UDataTable* UDungeonDataTable::GetDungeonTable = nullptr;

UDungeonDataTable::UDungeonDataTable()
{
    //
   static ConstructorHelpers::FObjectFinder<UDataTable> FoundDungeon(
              TEXT("DataTable'/Game/DataTables/Dungeon/DungeonDataTable.DungeonDataTable'"));
   GetDungeonTable=FoundDungeon.Object;
    //World'/Game/01_LevelMaps/StartHub.StartHub'
}

const FDungeonDataRow& UDungeonDataTable::GetDungeonData(FName id)
{
    return  *GetDungeonTable->FindRow<FDungeonDataRow>(id,"");
}

const FDungeonDataRow* UDungeonDataTable::GetDungeonDataPtr(FName id)
{
    return  GetDungeonTable->FindRow<FDungeonDataRow>(id,"");
}

