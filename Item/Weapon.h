// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Characters/UnitPawn.h"
#include "Datas/ItemDataTable.h"

#include "Weapon.generated.h"

class APlayerDiabloCharacter;
UCLASS()
class DIABLOM_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
protected:
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	USphereComponent* m_RootSphere;
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	USkeletalMeshComponent* m_SkMeshWeapon;
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	UStaticMeshComponent* m_StMeshWeapon;
	
	TWeakObjectPtr< AUnitPawn> m_User;

	bool m_bIsAttacking;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FGameplayTag m_CachedAttackEvent;

	const FItemInstance* m_ItemInstPtr;

public:
	virtual void InitWeapon(AUnitPawn* pl,const FItemInstance* itemInst);
	

	FORCEINLINE bool GetIsAttacking()
	{
		return m_bIsAttacking;
	}

	virtual  void RemoveWeapon(AUnitPawn* pl);
};
