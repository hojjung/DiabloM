// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Engine/SkeletalMesh.h"
#include "AbilitySystem/AbilityTypes.h"
#include "CharacterDataTable.generated.h"

/**
 * 
 */

#define MAXLEVEL 100

class UGameplayEffect;
UCLASS()
class DIABLOM_API UCharacterDataTable : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FEntityTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FEntityTable()
	{

	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_NameID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> m_AnimBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_UnitDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> m_DefaultStatTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> m_DefaultPassive;
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FPlayerEntityTable : public FEntityTable
{
	GENERATED_BODY()

public:
	FPlayerEntityTable()
	{

	}

public:
	//Skill

	//EquipTable �������� �����ִ� ���� �ٸ�
};

