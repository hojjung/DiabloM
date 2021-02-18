#pragma once

#include "CharacterDataTable.h"
#include "DiabloM.h"
#include "DungeonThemeAsset.h"
#include "GridFlowAsset.h"
#include "SpawnDataTable.h"
#include "Objs/Actor/DiaDungeon.h"
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
struct FDungeonStageData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassPlayerBuff;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassPlayerDebuff;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassMonsterBuff;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassMonsterDebuff;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FMonsterHordeHandle> m_AryHorde;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TArray<FDungeonDropTableHandle> m_AryDgDroptableHandle;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int m_nNamedMonsterCount =0;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassNamedMonsterBuff;
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    FMonsterSelect m_BossMob;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TSubclassOf<UDiabloAbility>> m_AryClassBossMonsterBuff;
	//던전 테마와 그리드 플로우도 여기 필요함	
};

USTRUCT(BlueprintType)
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
	FScalableFloat m_DgLevelTable;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FDungeonStageData> m_AryDgStageData;	
	
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


