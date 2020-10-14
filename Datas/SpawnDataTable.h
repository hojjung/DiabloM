// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Datas/CharacterDataTable.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "SpawnDataTable.generated.h"





USTRUCT(BlueprintType)
struct FMonsterSelect
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMonsterEntityHandle m_MonsterEntity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int m_nCount=1;
	
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FMonsterHordeRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FMonsterSelect>m_AryMonsterEntity;
	
};

UCLASS()
class DIABLOM_API USpawnDataTable : public UObject
{
	GENERATED_BODY()
public:
	USpawnDataTable();
	
public:
	static  UDataTable* GetHordeTable;
	
public:
	static const FMonsterHordeRow& GetMonsterHorde(FName id);

	static const FMonsterHordeRow* GetMonsterHordePtr(FName id);
};

