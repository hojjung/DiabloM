#pragma once
#include "DiabloM.h"
#include "SkillDataTable.h"
#include "CharacterDataTable.generated.h"


class AMonsterPawn;

USTRUCT(BlueprintType)
struct FPlayerEntityTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_NameID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_PlayerSkin = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> m_AnimBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_BaseAttackAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "1.0", UIMax = "4.4"))
	float m_fAttackSpeedMultiple = 1.f;
};

USTRUCT(BlueprintType)
struct FMonsterEntity : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMonsterEntity(): m_Mesh(nullptr),
	                  m_nDroptableRollCount(1), m_nDropTableIndex(0), m_HittenSound(nullptr), m_DeathSound(nullptr),
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "200.0"))
	float m_fAttackRange = 330.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "0.5"))
	float m_fAttackSpeed = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "0.0"))
	int m_nDroptableRollCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nDropTableIndex;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* m_HittenSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* m_DeathSound;
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

//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//TArray<FItemDataHandle> m_AryRewardDropTableHandle;


UCLASS()
class DIABLOM_API UCharacterDataTable : public UObject
{
	GENERATED_BODY()
	
	public:
	UCharacterDataTable();

public:
	static  UDataTable* GetMonsterEntityTable;

	static  UDataTable* GetPlayerEntityTable;

public:
	static const FMonsterEntity& GetMonster(FName id);

	static const FMonsterEntity* GetMonsterPtr(FName id);

	static const FPlayerEntityTable& GetPlayerEntity(FName id);
	
	static const FPlayerEntityTable* GetPlayerEntityPtr(FName id);
};


USTRUCT(BlueprintType)
struct FMonsterEntityHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
public:
	FMonsterEntityHandle();
};