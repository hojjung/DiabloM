// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Characters/UnitPawn.h"
#include "Objs/Interactable.h"
#include "PlayerDiabloCharacter.generated.h"

/**
 * 
 */

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UCameraDissolve* m_DissolveCam;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	UCameraComponent* m_TopCamera;

	IInteractable* m_FocusedInteractable;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void TryCheckInteractable();

	void InteractWithTarget();

	void OnInteractFound();

	void AutoPlayTick();

	virtual void Tick(float DeltaTime) override;

	void AttackInput(float pressed);

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

};
