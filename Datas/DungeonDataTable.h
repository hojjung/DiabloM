#pragma once
#include "CharacterDataTable.h"
#include "DiabloM.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "DungeonDataTable.generated.h"


UCLASS()
class DIABLOM_API UDungeonDataTable : public UObject
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FItemDropTableRow : public FTableRowBase
{
	GENERATED_BODY()

	//public:
	
};
USTRUCT(BlueprintType)
struct FItemDropTableHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
public:
	FItemDropTableHandle();
};
USTRUCT(BlueprintType)
struct FMonsterEntity : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMonsterEntity(): m_Mesh(nullptr),
                      m_nAvoidLevel(1),
                      m_SpawnAnim(nullptr), m_BaseAttackAnim(nullptr),
                      m_DeathMontage(nullptr),
                      m_TookHitMontage(nullptr)

	{
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "1.0"))
	float m_fScale = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "200.0"))
	float m_fMoveSpeed = 230.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "200.0"))
	float m_fAttackRange = 330.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "0.5"))
	float m_fAttackSpeed = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "1.0"))
	int m_nAvoidLevel = 1;
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
	FMonsterEntityHandle m_Monster;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FMonsterEntityHandle m_GoldGoblin;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fBossMonsterStatFactor = 10.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fBossMonsterRenderScale = 3.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(UIMin = "1.0"))
	int m_nMonsterLevel;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(UIMin = "1.0"))
	float m_fInitValue = 20;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)//인컴은 항상 선형적이다
	float m_fBaseValue = 1.67f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fBaseCost = 9;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float m_fCostMultiFactor = 1.06f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FItemDropTableHandle m_NormalDropTableHandle;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FItemDropTableHandle m_TreasureDropTableHandle;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FItemDropTableHandle m_BossDropTableHandle;

private:
	static float GetLevelBonus(int level)
	{
		if(level < 25)
		{
			return 2.f;
		}
		else if(level < 50)
		{
			return 4.f;
		}
		else if(level < 100)
		{
			return 8.f;
		}
		else if(level < 200)
		{
			return 16.f;
		}
		else if(level < 300)
		{
			return 32.f;
		}
		else if(level < 400)
		{
			return 64.f;
		}
		else if(level < 500)
		{
			return 128.f;
		}
		else if(level < 600)
		{
			return 256.f;
		}
		else if(level < 1000)
		{
			return 512.f;
		}
		else 
		{
			return 1024.f;
		}
	}

public:
	BigInt GetMobHp() const
	{
		int level = m_nMonsterLevel;
		
		float BonusMulti = GetLevelBonus(level);

		BigInt Value = m_fBaseValue;

		return (Value * BonusMulti * level) + m_fInitValue;
	}

	BigInt GetMobGold() const
	{
		int level  = m_nMonsterLevel;

		float Factor =  FMath::Pow(m_fCostMultiFactor,level);

		BigInt Cost = m_fBaseCost;

		Cost.Multiply(Factor);

		return Cost;
	}

	BigInt GetBossMobHp() const
	{
		int level = m_nMonsterLevel;
		
		float BonusMulti = GetLevelBonus(level);

		BigInt Value = m_fBaseValue;

		return ((Value * BonusMulti * level) + m_fInitValue) *m_fBossMonsterStatFactor;
	}

	BigInt GetBossMobGold() const
	{
		int level  = m_nMonsterLevel;

		float Factor =  FMath::Pow(m_fCostMultiFactor,level);

		BigInt Cost = m_fBaseCost;

		Cost.Multiply(Factor*m_fBossMonsterStatFactor);

		return Cost;
	}
};


