// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "ItemDataTable.h"
#include "AbilitySystem/AbilityTypes.h"

#include "Characters/Logic/MobFSMBase.h"

#include "CharacterDataTable.generated.h"


class AMonsterPawn;


USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FEntityTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FEntityTable(): m_DeathMontage(nullptr)
	{
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_NameID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_UnitDesc;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> m_DefaultStatTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* m_DeathMontage;
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FMonsterTypeRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMonsterTypeRow()
	{
		m_ShowingText=FText::FromString("LikeUndead,Animal");
		
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText m_ShowingText;
	
	//MoreThings
};




USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FPlayerEntityTable : public FEntityTable
{
	GENERATED_BODY()

public:
	//1. CharStat-Done
	//2. Equipable ItemType,지금은 모든 장비타입을 낄수 있으니까, 못끼는 테이블을 만들면 된다
	//3. SkillTable
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryUnequipHead;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryUnequipNeck;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryUnequipTorso;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryUnequipWaist;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryUnequipLeg;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryUnequipHand;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryUnequipShoulder;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryUnequipWeaponRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryUnequipWeaponLeft;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryUnequipFingerRight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemTypeHandle> m_AryUnequipFingerLeft;


	FORCEINLINE TArray<TArray<FItemTypeHandle>> GetUnequipableAry() const
	{
		TArray<TArray<FItemTypeHandle>> AryAryMade;

		AryAryMade.Add(m_AryUnequipHead);
		AryAryMade.Add(m_AryUnequipNeck);
		AryAryMade.Add(m_AryUnequipTorso);
		AryAryMade.Add(m_AryUnequipWaist);
		AryAryMade.Add(m_AryUnequipLeg);
		AryAryMade.Add(m_AryUnequipHand);
		AryAryMade.Add(m_AryUnequipShoulder);
		AryAryMade.Add(m_AryUnequipWeaponRight);
		AryAryMade.Add(m_AryUnequipWeaponLeft);
		AryAryMade.Add(m_AryUnequipFingerRight);
		AryAryMade.Add(m_AryUnequipFingerLeft);
		
		return  AryAryMade;
	}
};



// m_Head.m_Slot = ESlotsEquipAry::Head;
// m_Neck.m_Slot = ESlotsEquipAry::Neck;
// m_Torso.m_Slot = ESlotsEquipAry::Torso;
// m_Waist.m_Slot = ESlotsEquipAry::Waist;
// m_Leg.m_Slot = ESlotsEquipAry::Leg;
// m_Hand.m_Slot = ESlotsEquipAry::Hand;
// m_Shoulder.m_Slot = ESlotsEquipAry::Shoulder;
// m_WeaponRight.m_Slot = ESlotsEquipAry::WeaponRight;
// m_WeaponLeft.m_Slot = ESlotsEquipAry::WeaponLeft;
// m_FingerRight.m_Slot = ESlotsEquipAry::FingerRight;
// m_FingerLeft.m_Slot = ESlotsEquipAry::FingerLeft;

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FNPCEntityTable : public FEntityTable
{
	GENERATED_BODY()

public:
	FNPCEntityTable()
	{
	}

};


struct FMonsterTable;
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

	static  UDataTable* GetMonsterTypeTable;

	static TSubclassOf<AMonsterPawn> ClassMonsterPawn;
public:
	static const FMonsterTable& GetMonster(FName id);

	static const FMonsterTable* GetMonsterPtr(FName id);

	static const FMonsterTypeRow& GetMonsterType(FName id);
	
	static const FMonsterTypeRow* GetMonsterTypePtr(FName id);

	static const FPlayerEntityTable& GetPlayerEntity(FName id);
	
	static const FPlayerEntityTable* GetPlayerEntityPtr(FName id);

	static const FNPCEntityTable& GetNPC(FName id);

	static const FNPCEntityTable* GetNPCPtr(FName id);
};


USTRUCT(BlueprintType)
struct FPlayerTypeHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
public:
	FPlayerTypeHandle()
	{
		DataTable=UCharacterDataTable::GetPlayerEntityTable;
	}

};

USTRUCT(BlueprintType)
struct FMonsterTypeHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
public:
	
	FMonsterTypeHandle()
	{
		DataTable = UCharacterDataTable::GetMonsterTypeTable;
	}
	
};

USTRUCT(BlueprintType)
struct FMonsterEntityHandle :public FDataTableRowHandle
{
	GENERATED_USTRUCT_BODY()
public:
	
	FMonsterEntityHandle()
	{
		DataTable = UCharacterDataTable::GetMonsterEntityTable;
	}
	
};

USTRUCT(BlueprintType)//���̵�,Ƽ��
struct FMonsterTable : public FEntityTable
{
	GENERATED_BODY()

public:
	FMonsterTable(): m_Mesh(nullptr)
	{
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* m_Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> m_AnimBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMonsterTypeHandle m_TypeHandle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDiabloAbility> m_BaseAttack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UMobFSMBase> m_MobFSM;
	
};