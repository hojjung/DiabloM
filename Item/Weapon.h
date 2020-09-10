// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayTagContainer.h"
#include "Weapon.generated.h"

class APlayerDiabloCharacter;
UCLASS()
class DIABLOM_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
protected:
	UPROPERTY(VisibleAnywhere,Category="Weapon")
	UCapsuleComponent* m_Coll;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USkeletalMeshComponent* m_WeaponMesh;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FName m_Id;

	UPROPERTY()
	APlayerDiabloCharacter* m_User;

	bool m_bIsAttacking;

	UPROPERTY()
	FGameplayTag m_CachedAttackEvent;

	UPROPERTY()
	TSet<AActor*> m_AlreadyHittenForIgnore;

protected:
	virtual void BeginPlay() override;

public:
	void SetWeapon(FName weaponId, APlayerDiabloCharacter* usingPlayer);

	UFUNCTION(BlueprintCallable)
	void WeaponAttackBegin(FGameplayTag attackEventTag);

	UFUNCTION(BlueprintCallable)
	void WeaponAttackEnd();

	UFUNCTION()
	void WeaponCollBegin(AActor* overlappedActor,AActor* otherActor);

	FORCEINLINE bool GetIsAttacking()
	{
		return m_bIsAttacking;
	}
};
