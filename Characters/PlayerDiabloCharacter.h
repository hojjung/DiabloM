// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Characters/UnitPawn.h"

#include "PlayerDiabloCharacter.generated.h"

/**
 * 
 */
class IInteractable;
class ADiabloPlayerController;
class UCameraDissolve;
UCLASS()
class DIABLOM_API APlayerDiabloCharacter : public AUnitPawn
{
	GENERATED_BODY()
	
public:
	APlayerDiabloCharacter(const FObjectInitializer& objInit);

	static const FName RightHandWeaponSocketTop;
	static const FName RightHandWeaponSocketBottom;
	static const FName LeftHandWeaponSocketTop;
	static const FName LeftHandWeaponSocketBottom;
	
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	float m_fInteractRange;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UCameraDissolve* m_DissolveCam;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UCameraComponent* m_TopCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	USkeletalMeshComponent* m_SkFace;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	USkeletalMeshComponent* m_SkHair;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	USkeletalMeshComponent* m_SkGlove;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	USkeletalMeshComponent* m_SkShoe;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	USkeletalMeshComponent* m_SkHeadGear;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	USkeletalMeshComponent* m_SkShoulderPad;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	USkeletalMeshComponent* m_SkBelt;
protected:
	UPROPERTY()
	USkeletalMesh* m_CachedBodyMesh;
	UPROPERTY()
	USkeletalMesh* m_CachedHairMesh;

	TScriptInterface< IInteractable> m_FocusedInteractable;

protected:
	virtual void SetUnit(FName unitID) override;
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void TryCheckInteractable();

	void OnInteractFound();

	void AutoPlayTick();

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void SetBodyMeshToCached();

	void SetHairMeshToCached();
	
public:
	UFUNCTION(BlueprintCallable,Category="Interact")
	void InteractWithTarget();
	UFUNCTION(BlueprintCallable,Category="Interact")
	void AttackInput(float pressed);
};
