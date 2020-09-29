// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Animations/DiaAniminstance.h"
#include "Characters/UnitPawn.h"
#include "Managers/DiabloCheatManager.h"
#include "SaveLoad/SaveCharacterStatus.h"


#include "PlayerDiabloCharacter.generated.h"

/**
 * 
 */
class IInteractable;
class ADiabloPlayerController;
class UCameraDissolve;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFloatChange,float);
UCLASS()
class DIABLOM_API APlayerDiabloCharacter : public AUnitPawn
{
	GENERATED_BODY()
	
public:
	APlayerDiabloCharacter(const FObjectInitializer& objInit);
	void LoadExp(const USaveCharacterStatus* loadedSaveData);

	static const FName RightHandWeaponSocketTop;
	static const FName RightHandWeaponSocketBottom;
	static const FName LeftHandWeaponSocketTop;
	static const FName LeftHandWeaponSocketBottom;
	
	int m_FaceIndex;
	int m_HairIndex;
	
	FOnFloatChange m_OnLevelChanged;
	FOnFloatChange m_OnAttackPerSecChanged;
	FOnFloatChange m_OnExpGaugeChanged;
	FOnFloatChange m_OnRemainExpChanged;
	
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
	TScriptInterface<IInteractable> m_FocusedInteractable;

	float m_fCurrentExp;
	
	float m_fMaxExp;

	UPROPERTY()
	AWeapon* m_RightWeapon;
	UPROPERTY()
	AWeapon* m_LeftWeapon;
	
	const FAnimStance* m_AnimStance;
protected:
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

	virtual void SetAttackSpeed(float get_attack_speed)override;

	bool CreateItemActor(const FItemInstance* itemInst,AWeapon** wantCachePointer,UStaticMeshComponent** attachRoot);
	//FItemInstance*,AWeapon**,USceneComponent*
public:
	
	virtual void SetUnitStat(FName unitID,int level) override;
	
	UFUNCTION(BlueprintCallable,Category="Interact")
	void InteractWithTarget();
	

	virtual void AttackInput(float pressed)override;

	void SetLoadedData(const USaveCharacterStatus* loadedSaveData);

	void EquipMesh(const FItemInstance* meshItem,ESlotsEquipAry slotWant);

	void RemoveAllEffect();

	void SetAnimStance(const FAnimStance* animStance);
	void ShowDamageNumber(const float local_damage_done, AUnitPawn* unit_pawn);

	void EarnExp(float expEarned);

	virtual bool SetCharacterLevel(int NewLevel)override;

	friend UDiabloCheatManager;
	friend USaveLoadManager;
};
