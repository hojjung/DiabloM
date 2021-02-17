#include "DungeonDataTable.h"


UDataTable* UDungeonDataTable::GetDungeonTable = nullptr;

UDataTable* UDungeonDataTable::GetDungeonDropTable = nullptr;

UDungeonDataTable::UDungeonDataTable()
{
    //
   static ConstructorHelpers::FObjectFinder<UDataTable> FoundDungeon(
              TEXT("DataTable'/Game/DataTables/Dungeon/DungeonDataTable.DungeonDataTable'"));
   GetDungeonTable = FoundDungeon.Object;
    //
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundDungeonDrop(
              TEXT("DataTable'/Game/DataTables/Dungeon/DungeonDropDataTable.DungeonDropDataTable'"));
    GetDungeonDropTable = FoundDungeonDrop.Object;
}

const FDungeonDataRow& UDungeonDataTable::GetDungeonData(FName id)
{
    return  *GetDungeonTable->FindRow<FDungeonDataRow>(id,"");
}

const FDungeonDataRow* UDungeonDataTable::GetDungeonDataPtr(FName id)
{
    return  GetDungeonTable->FindRow<FDungeonDataRow>(id,"");
}

const FDungeonDropTable& UDungeonDataTable::GetDungeonDropData(FName id)
{
    return  *GetDungeonDropTable->FindRow<FDungeonDropTable>(id,"");
}

const FDungeonDropTable* UDungeonDataTable::GetDungeonDropDataPtr(FName id)
{
    return  GetDungeonDropTable->FindRow<FDungeonDropTable>(id,"");
}

