// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "AbilitySystem/AbilityTypes.h"
#include "CharacterDataTable.generated.h"





USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FEntityTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FEntityTable(): m_Mesh(nullptr)
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

	//EquipTable �������� �����ִ� ���� �ٸ�
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FNPCEntityTable : public FEntityTable
{
	GENERATED_BODY()

public:
	FNPCEntityTable()
	{
	}

};



class UGameplayEffect;
UCLASS()
class DIABLOM_API UCharacterDataTable : public UObject
{
	GENERATED_BODY()
	
public:
	UCharacterDataTable();

public:
	static  UDataTable* GetMonsterEntityTable;

	static  UDataTable* GetPlayerEntityTable;

	static  UDataTable* GetNPCEntityTable;

public:
	static const FEntityTable& GetMonster(FName id);

	static const FEntityTable* GetMonsterPtr(FName id);

	static const FPlayerEntityTable& GetPlayerEntity(FName id);
	
	static const FPlayerEntityTable* GetPlayerEntityPtr(FName id);

	static const FNPCEntityTable& GetNPC(FName id);

	static const FNPCEntityTable* GetNPCPtr(FName id);
};
