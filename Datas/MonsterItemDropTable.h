// My First Hack n Slash

#pragma once

#include "DiabloM.h"

#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "MonsterItemDropTable.generated.h"





USTRUCT(BlueprintType)
struct FMonsterItemDropRow: public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FMonsterItemDropRow();
	
public:
	//TODO 아이템 타입 랜덤? - 아이템 을 떨굴 최대 개수, 그중 뜰 아이템 타입,pri 적용, 또 아이템 티어 높아질 확률 적용 필요
	//TODO 장비템은 아이템 타입으로 분간 되지만 재료 템은 아님, 재료 템 따로 필요
	//TODO 골드는 어떻게? - 그냥 숫자 최소 최대 랜덤 쓰고 랜덤 1~3개 까지 분산 시킬것
	//TODO 체력바는 개수만 랜덤
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fStancePriority;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryRightHandNeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryLeftHandNeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDiaAniminstance> m_StanceAnimation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPlayerDiabloAbility> m_BaseAttackAbility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fViewAngle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fViewRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_fFocusRange;
};

UCLASS()
class DIABLOM_API UMonsterItemDropTable : public UObject
{
	GENERATED_BODY()
public:
	UMonsterItemDropTable();

public:
	static  UDataTable* GetTierTable;

	static  UDataTable* GetItemTable;

	static  UDataTable* GetItemTypeTable;

	static  UDataTable* GetAnimStanceTable;

public:
	static const FAnimStance& GetAnimStance(FName id);

	static const FAnimStance* GetAnimStancePtr(FName id);
};


USTRUCT(BlueprintType)
struct FMonsterItemDropRowHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
public:
	FMonsterItemDropRowHandle();
};