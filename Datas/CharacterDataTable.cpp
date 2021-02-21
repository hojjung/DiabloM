#include "CharacterDataTable.h"
#include "Characters/MonsterPawn.h"

UDataTable* UCharacterDataTable::GetMonsterEntityTable = nullptr;
UDataTable* UCharacterDataTable::GetPlayerEntityTable = nullptr;
//GetMonsterEntityTable

FMonsterEntityHandle::FMonsterEntityHandle()
{
    DataTable = UCharacterDataTable::GetMonsterEntityTable;
}

UCharacterDataTable::UCharacterDataTable()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable(
              TEXT("DataTable'/Game/DataTables/Entities/MonsterTable.MonsterTable'"));
    GetMonsterEntityTable = FoundEntityTable.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable2(
              TEXT("DataTable'/Game/DataTables/Entities/PlayerTable.PlayerTable'"));
    GetPlayerEntityTable = FoundEntityTable2.Object;
}

const FMonsterEntity& UCharacterDataTable::GetMonster(FName id)
{
    return  *GetMonsterEntityTable->FindRow<FMonsterEntity>(id,"");
}

const FMonsterEntity* UCharacterDataTable::GetMonsterPtr(FName id)
{
    return  GetMonsterEntityTable->FindRow<FMonsterEntity>(id,"");
}

const FPlayerEntityTable& UCharacterDataTable::GetPlayerEntity(FName id)
{
    return  *GetPlayerEntityTable->FindRow<FPlayerEntityTable>(id,"");
}

const FPlayerEntityTable* UCharacterDataTable::GetPlayerEntityPtr(FName id)
{
    return  GetPlayerEntityTable->FindRow<FPlayerEntityTable>(id,"");
}

