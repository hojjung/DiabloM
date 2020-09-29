// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item/Weapon.h"
#include "WeaponMelee.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AWeaponMelee : public AWeapon
{
	GENERATED_BODY()

public:
	AWeaponMelee();
	
protected:
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	UCapsuleComponent* m_MeleeCollison;
	UPROPERTY()
	TSet<AActor*> m_AlreadyHittenForIgnore;
	
	FGameplayTag m_CachedAttackEvent;
	
public:
	virtual void InitWeapon(APlayerDiabloCharacter* pl, const FItemInstance* itemInst) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable)
	void BeginWeaponAttack(FGameplayTag tag);
	
	UFUNCTION(BlueprintCallable)
	void EndWeaponAttack();
};
