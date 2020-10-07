// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item/Weapon.h"
#include "WeaponMelee.generated.h"

/**
 * 
 */
//(FComponentBeginOverlapSignature, UPrimitiveComponent, OnComponentBeginOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult &, SweepResult);
UCLASS()
class DIABLOM_API AWeaponMelee : public AWeapon
{
	GENERATED_BODY()

public:
	AWeaponMelee();
	
protected:
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	UCapsuleComponent* m_MeleeCollison;

	FDelegateHandle m_StartDeleHandle;

	FDelegateHandle m_EndDeleHandle;

	
public:
	virtual void InitWeapon(APlayerDiabloCharacter* pl, const FItemInstance* itemInst) override;

	
	UFUNCTION()
	void OnOverlapWeapon(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void RemoveWeapon(APlayerDiabloCharacter* pl) override;

	UFUNCTION(BlueprintCallable)
	void BeginWeaponAttack();
	
	UFUNCTION(BlueprintCallable)
	void EndWeaponAttack();
};
