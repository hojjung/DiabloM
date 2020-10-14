#pragma once

#include "CharacterDataTable.h"
#include "DiabloM.h"
#include "SpawnDataTable.h"
#include "UObject/NoExportTypes.h"
#include "DungeonDataTable.generated.h"

USTRUCT(BlueprintType)
struct FMonsterHordeHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
public:
	
	FMonsterHordeHandle()
	{
		DataTable = USpawnDataTable::GetHordeTable;
	}

	
};
USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FDungeonDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FMonsterHordeHandle> m_AryHorde;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FName> m_IDPremadeDungeons;//should be add in stream level
	
};


UCLASS()
class DIABLOM_API UDungeonDataTable : public UObject
{
	GENERATED_BODY()

public:
	UDungeonDataTable();
public:
	static  UDataTable* GetDungeonTable;
	
public:
	static const FDungeonDataRow& GetDungeonData(FName id);

	static const FDungeonDataRow* GetDungeonDataPtr(FName id);
};

