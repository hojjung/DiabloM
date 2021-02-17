#pragma once

#include "CharacterDataTable.h"
#include "DiabloM.h"
#include "DungeonThemeAsset.h"
#include "GridFlowAsset.h"
#include "SpawnDataTable.h"
#include "UObject/NoExportTypes.h"
#include "DungeonDataTable.generated.h"

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FDungeonDropTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FItemDataHandle> m_AryDropItems;
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FDungeonDropTableHandle : public FDataTableRowHandle
{
	GENERATED_BODY()

public:
	FDungeonDropTableHandle()
	{
		DataTable = USpawnDataTable::GetHordeTable;
	}
};



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
	bool m_bIsInfinityDg=false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FMonsterHordeHandle m_Horde;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassPlayerBuff;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassPlayerDebuff;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassMonsterBuff;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassMonsterDebuff;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FScalableFloat m_DgLevelTable;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName m_IDDgTheme;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FDungeonDropTableHandle m_DgDroptableHandle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FMonsterSelect m_BossMob;
	

	int StageLevelToDungeonLevel(int dgLv)const
	{
		return m_DgLevelTable.GetValueAtLevel(dgLv);
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

	static  UDataTable* GetDungeonDropTable;
	
public:
	static const FDungeonDataRow& GetDungeonData(FName id);

	static const FDungeonDataRow* GetDungeonDataPtr(FName id);

	static const FDungeonDropTable& GetDungeonDropData(FName id);

	static const FDungeonDropTable* GetDungeonDropDataPtr(FName id);
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


