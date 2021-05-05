#pragma once
#include "DiabloM.h"
#include "DungeonDataTable.generated.h"


UCLASS()
class DIABLOM_API UDungeonDataTable : public UObject
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FMonsterEntity : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMonsterEntity(): 
                      m_SpawnAnim(nullptr), m_BaseAttackAnim(nullptr),
                      m_DeathMontage(nullptr),
                      m_TookHitMontage(nullptr)

	{
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> m_MonsterMeshSoft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "0.3"))
	float m_fScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "0.3"))
	float m_fBossMonsterRenderScale = 3.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "200.0"))
	float m_fMoveSpeed = 230.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "200.0"))
	float m_fAttackRange = 330.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "0.5"))
	float m_fAttackSpeed = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> m_AnimBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_SpawnAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_BaseAttackAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_DeathMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_TookHitMontage;
};
USTRUCT(BlueprintType)
struct FMonsterEntityHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
public:
	FMonsterEntityHandle();
};

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
	TArray<FMonsterEntityHandle> m_Monsters;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(UIMin = "1.0"))
	int m_nMonsterLevel;

public:
	BigInt GetMobHp() const
	{
		int level = m_nMonsterLevel;
		
		BigInt Value = 150;

		for(int i=1; i< level;i++)
		{
			Value.MultiplyFast(3);
		}

		return Value;
	}

	BigInt GetMobGold() const
	{
		int level = m_nMonsterLevel;
		
		BigInt Value =2800;

		return (Value * level)+30;
	}
};


