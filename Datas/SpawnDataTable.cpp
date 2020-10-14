#include "SpawnDataTable.h"

UDataTable* USpawnDataTable::GetHordeTable = nullptr;

USpawnDataTable::USpawnDataTable()
{
    //
    
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundSpawnTable(
              TEXT("DataTable'/Game/DataTables/Dungeon/MonsterHorde.MonsterHorde'"));
    GetHordeTable=FoundSpawnTable.Object;
}

const FMonsterHordeRow& USpawnDataTable::GetMonsterHorde(FName id)
{
    return  *GetHordeTable->FindRow<FMonsterHordeRow>(id,"");
}

const FMonsterHordeRow* USpawnDataTable::GetMonsterHordePtr(FName id)
{
    return  GetHordeTable->FindRow<FMonsterHordeRow>(id,"");
}

