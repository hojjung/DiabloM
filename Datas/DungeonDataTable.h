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

USTRUCT(BlueprintType)
struct FMonsterEntity : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMonsterEntity(): m_Mesh(nullptr),
                      m_nAvoidLevel(1), m_nDropTableIndex(0),
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
	float m_fMoveSpeed = 230.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "200.0"))
	float m_fAttackRange = 330.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "0.5"))
	float m_fAttackSpeed = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(UIMin = "1.0"))
	int m_nAvoidLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nDropTableIndex;
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
	UTexture2D* m_DgIcon;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FMonsterEntityHandle> m_AryMonster;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FMonsterEntityHandle m_BossMonster;
	//UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	//ItemDrop Tarray need
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
