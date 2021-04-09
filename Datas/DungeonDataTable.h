#pragma once
#include "DiabloM.h"
#include "GameplayTagContainer.h"
#include "Managers/PlayerUpgradeManager.h"
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
	FMonsterEntityHandle m_Monster;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(UIMin = "1.0"))
	int m_nMonsterLevel;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FItemDropTableHandle m_NormalDropTableHandle;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FItemDropTableHandle m_TreasureDropTableHandle;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FItemDropTableHandle m_BossDropTableHandle;

private:
	static float GetLevelBonus(int level)
	{
		if(level <8)
		{
			return 1.f;
		}
		else if(level < 16)
		{
			return 2.f;
		}
		else if(level < 24)
		{
			return 4.f;
		}
		else if(level < 32)
		{
			return 8.f;
		}
		else if(level < 40)
		{
			return 16.f;
		}
		else if(level < 48)
		{
			return 32.f;
		}
		else if(level < 56)
		{
			return 64.f;
		}
		else if(level < 64)
		{
			return 128.f;
		}
		else if(level < 72)
		{
			return 256;
		}
		else if(level < 80)
		{
			return 512;
		}
		else
		{
			return 1024;
		}
	}

public:
	BigInt GetMobHp() const
	{
		int level = m_nMonsterLevel;
		
		float BonusMulti = GetLevelBonus(level);

		BigInt Value = 150;

		for(int i=1; i< level;i++)
		{
			Value.MultiplyFast(6);
		}

		Value.Multiply(BonusMulti);

		return Value;
	}

	BigInt GetMobGold() const
	{
		int level = m_nMonsterLevel;
		
		float BonusMulti = GetLevelBonus(level);

		BigInt Value =2800;

		return (Value * BonusMulti * level)+30;
	}
};


