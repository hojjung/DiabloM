#include "DungeonDataTable.h"

#include "Managers/DungeonManager.h"

//UDataTable* UDungeonDataTable::GetDungeonTable = nullptr;

//TArray<FDungeonDataTableRow*> UDungeonDataTable::AryDgData;

// UDungeonDataTable::UDungeonDataTable()
// {
//    static ConstructorHelpers::FObjectFinder<UDataTable> FoundDungeon(
//               TEXT("DataTable'/Game/DataTables/Dungeon/DungeonData.DungeonData'"));
//    GetDungeonTable = FoundDungeon.Object;
//
// 	
// }

FMonsterEntityHandle::FMonsterEntityHandle()
{
	DataTable = UDungeonManager::MonsterEntityTable;
}
