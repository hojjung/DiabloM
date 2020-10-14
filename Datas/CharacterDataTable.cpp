#include "CharacterDataTable.h"
#include "Characters/MonsterPawn.h"


UDataTable* UCharacterDataTable::GetMonsterEntityTable = nullptr;
UDataTable* UCharacterDataTable::GetPlayerEntityTable = nullptr;
UDataTable* UCharacterDataTable::GetNPCEntityTable = nullptr;
UDataTable* UCharacterDataTable::GetMonsterTypeTable = nullptr;
TSubclassOf<AMonsterPawn> UCharacterDataTable::ClassMonsterPawn = nullptr;
//GetMonsterEntityTable

UCharacterDataTable::UCharacterDataTable()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable(
              TEXT("DataTable'/Game/DataTables/Entities/MonsterTable.MonsterTable'"));
    GetMonsterEntityTable = FoundEntityTable.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable2(
              TEXT("DataTable'/Game/DataTables/Entities/PlayerTable.PlayerTable'"));
    GetPlayerEntityTable = FoundEntityTable2.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable3(
              TEXT("DataTable'/Game/DataTables/Entities/NPCTable.NPCTable'"));
    GetNPCEntityTable=FoundEntityTable3.Object;

    static ConstructorHelpers::FObjectFinder<UDataTable> FoundEntityTable4(
              TEXT("DataTable'/Game/DataTables/Entities/MonsterTypeTable.MonsterTypeTable'"));
    GetMonsterTypeTable=FoundEntityTable4.Object;
//
    static ConstructorHelpers::FClassFinder<AMonsterPawn> FoundMonster(TEXT("Blueprint'/Game/Blueprints/Pawns/BP_Monster.BP_Monster_C'"));
    //
    ClassMonsterPawn=FoundMonster.Class;
}

const FMonsterTable& UCharacterDataTable::GetMonster(FName id)
{
    return  *GetMonsterEntityTable->FindRow<FMonsterTable>(id,"");
}

const FMonsterTable* UCharacterDataTable::GetMonsterPtr(FName id)
{
    return  GetMonsterEntityTable->FindRow<FMonsterTable>(id,"");
}

const FMonsterTypeRow& UCharacterDataTable::GetMonsterType(FName id)
{
    return *GetMonsterTypeTable->FindRow<FMonsterTypeRow>(id,"");
}

const FMonsterTypeRow* UCharacterDataTable::GetMonsterTypePtr(FName id)
{
    return GetMonsterTypeTable->FindRow<FMonsterTypeRow>(id,"");
}

const FPlayerEntityTable& UCharacterDataTable::GetPlayerEntity(FName id)
{
    return  *GetPlayerEntityTable->FindRow<FPlayerEntityTable>(id,"");
}

const FPlayerEntityTable* UCharacterDataTable::GetPlayerEntityPtr(FName id)
{
    return  GetPlayerEntityTable->FindRow<FPlayerEntityTable>(id,"");
}

const FNPCEntityTable& UCharacterDataTable::GetNPC(FName id)
{
    return  *GetNPCEntityTable->FindRow<FNPCEntityTable>(id,"");
}

const FNPCEntityTable* UCharacterDataTable::GetNPCPtr(FName id)
{
    return  GetNPCEntityTable->FindRow<FNPCEntityTable>(id,"");
}
