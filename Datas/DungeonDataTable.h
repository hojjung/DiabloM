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
	UTexture2D* m_DgIcon;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText m_DgShowName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FMonsterHordeHandle> m_AryHorde;//이중한개의 호드만 사용
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FName> m_IDPremadeDungeons;//should be add in stream level
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassPlayerBuff;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassPlayerDebuff;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassMonsterBuff;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassMonsterDebuff;

	int StageLevelToDungeonLevel(int dgLv)const
	{
		return dgLv;
	}
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

USTRUCT(BlueprintType)
struct FDgDataHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
public:
	
	FDgDataHandle()
	{
		DataTable = UDungeonDataTable::GetDungeonTable;
	}

	
};