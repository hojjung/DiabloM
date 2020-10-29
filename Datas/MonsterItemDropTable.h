#pragma once
#include "DiabloM.h"
#include "ItemDataTable.h"
#include "Engine/DataTable.h"
#include "Item/DroppedGold.h"
#include "Item/DroppedItem.h"
#include "Objs/Actor/HealthSphere.h"
#include "UObject/NoExportTypes.h"
#include "MonsterItemDropTable.generated.h"



USTRUCT(BlueprintType)
struct FItemDropData 
{
	GENERATED_USTRUCT_BODY()
public:
	FItemDropData()
	{
		m_fDropRatePriority=0.f;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemDataHandle m_DropHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0"))
	float m_fDropRatePriority;//There is no max
};

USTRUCT(BlueprintType)
struct FUniqueItemDropData 
{
	GENERATED_USTRUCT_BODY()
public:
	FUniqueItemDropData()
	{
		m_fDropRateOnePerMax=0.f;
	}
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FUniqueItemDataHandle m_DropHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0", UIMin = "0",ClampMax = "1", UIMax = "1"))
	float m_fDropRateOnePerMax;
};


USTRUCT(BlueprintType)
struct FMonsterItemDropRow: public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FMonsterItemDropRow();
	
public:
	//TODO 드랍 확률
	//TODO 드랍개수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemDropData> m_AryDropItems;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FUniqueItemDropData> m_AropDropUniqueItems;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<int> m_AryHpDropRand;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<int> m_AryItemDropRand;
};

UCLASS()
class DIABLOM_API UMonsterItemDropTable : public UObject //블프 레퍼런스 이쪽에서 해줄것
{
	GENERATED_BODY()
public:
	UMonsterItemDropTable();

public:
	static TSubclassOf<AHealthSphere> ClassDropHealthSphere;

	static TSubclassOf<ADroppedItem> ClassDropItemActor;

	static TSubclassOf<ADroppedGold> ClassDropGoldActor;
	
	static  UDataTable* GetMonsterRewardDropTable;

public:
	static const FMonsterItemDropRow& GetDropRewardTableRow(FName id);

	static const FMonsterItemDropRow* GetDropRewardTableRowPtr(FName id);
};


USTRUCT(BlueprintType)
struct FMonsterItemDropRowHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
public:
	FMonsterItemDropRowHandle();
};