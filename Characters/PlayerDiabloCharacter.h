// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Animations/DiaAniminstance.h"
#include "Characters/UnitPawn.h"
#include "SaveLoad/SaveCharacterStatus.h"


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
	
	int m_FaceIndex;
	int m_HairIndex;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	float m_fInteractRange;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UCameraDissolve* m_DissolveCam;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UCameraComponent* m_TopCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	USkeletalMeshComponent* m_SkShadow;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UStaticMeshComponent* m_StBackpack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UStaticMeshComponent* m_StRightWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UStaticMeshComponent* m_StLeftWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	TArray<TSubclassOf<UDiaAniminstance>> m_AryAnimBP;
protected:
	UPROPERTY()
	USkeletalMesh* m_DefaultFullHairMesh;
	UPROPERTY()
	USkeletalMesh* m_DefaultHalfHairMesh;
	UPROPERTY()
	USkeletalMesh* m_DefaultBodyMesh;
	UPROPERTY()
	USkeletalMesh* m_DefaultGloveMesh;
	UPROPERTY()
	USkeletalMesh* m_DefaultShoeMesh;
	//
	TScriptInterface< IInteractable> m_FocusedInteractable;

protected:
	void SetAnimStance(EAnimStance animStance);
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void TryCheckInteractable();

	void OnInteractFound();

	void AutoPlayTick();

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void SetFullHairMesh();

	void SetHalfHairMesh();

	void SetDefaultBodyMesh();

	void SetDefaultShoeMesh();

	void SetDefaultGloveMesh();
	
public:
	virtual void SetUnitStat(FName unitID) override;
	
	UFUNCTION(BlueprintCallable,Category="Interact")
	void InteractWithTarget();
	UFUNCTION(BlueprintCallable,Category="Interact")
	void AttackInput(float pressed);

	void SetLoadedData(const USaveCharacterStatus* loadedSaveData);

	void EquipMesh(const FItemData* meshItem,ESlots slotWant);


	private:
	EAnimStance GetAnimStance() const;
};
