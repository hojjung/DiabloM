#pragma once
#include "CharacterDataTable.h"
#include "DiabloM.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "DungeonDataTable.generated.h"

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FDungeonDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText m_DgShowName;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName m_DgId;//should same with level asset name
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UTexture2D* m_DgIcon;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FMonsterEntityHandle> m_AryMonster;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FMonsterEntityHandle m_BossMonster;
	//UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	//ItemDrop Tarray need
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
	static const FDungeonDataTableRow& GetDungeonData(FName id);

	static const FDungeonDataTableRow* GetDungeonDataPtr(FName id);
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FDungeonDataTableRowHandle : public FDataTableRowHandle
{
	GENERATED_BODY()

public:
	FDungeonDataTableRowHandle()
	{
		DataTable = UDungeonDataTable::GetDungeonTable;
	}
	
};
