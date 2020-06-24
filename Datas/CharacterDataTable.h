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

USTRUCT(BlueprintType)//난이도,티어
struct FBaseStatTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FBaseStatTable()
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

USTRUCT(BlueprintType)//난이도,티어
struct FPlayerStatTable : public FBaseStatTable
{
	GENERATED_BODY()

public:
	FPlayerStatTable()
	{

	}

public:
	//Skill

	//EquipTable 직업마다 낄수있는 무기 다름
};

