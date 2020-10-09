// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Animations/DiaAniminstance.h"
#include "Characters/UnitPawn.h"
#include "Managers/DiabloCheatManager.h"
#include "SaveLoad/SaveCharacterStatus.h"
#include "PlayerDiabloCharacter.generated.h"

class IInteractable;
class ADiabloPlayerController;
class UCameraDissolve;
class UPlayerSensing;
class UPlayerBaseAttack;
class UDefaultFSM;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFloatChange,float);

UCLASS()
class DIABLOM_API APlayerDiabloCharacter : public AUnitPawn
{
	GENERATED_BODY()
	
	friend UDiabloCheatManager;
	friend USaveLoadManager;
	friend UPlayerSensing;
public:
	APlayerDiabloCharacter(const FObjectInitializer& objInit);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	TArray<TEnumAsByte< EObjectTypeQuery>> m_AryTargetingObjectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	UMaterialInstance* m_OutLineMat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	float m_fInteractRange;
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
	bool m_bUseAutoPlay;
	//
protected:
	UPROPERTY()
	UPlayerSensing* m_PlayerSense;
	
	UPROPERTY()
	USkeletalMeshComponent* m_FocusRenderer;
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
	UPROPERTY()
	AWeapon* m_RightWeapon;
	UPROPERTY()
	AWeapon* m_LeftWeapon;
	UPROPERTY()
	TArray< AActor*> m_AryIgnoreActor;
	UPROPERTY()
	ADiabloPlayerController* m_PlayerCon;
	UPROPERTY()
	TArray<TSubclassOf<UDiabloAbility>> m_GrantedMasteryAbilities;
	UPROPERTY()
	TArray<TSubclassOf<UDiabloAbility>> m_GrantedItemAbilities;
	
	IInteractable* m_FocusedInteractable;
	
	const FAnimStance* m_AnimStance;
	
	const FPlayerEntityTable* m_PlayerEntityData;

	FGameplayAbilitySpecHandle m_BaseAttackHandle;
	
	FDelegateHandle m_InventoryUpdateHandle;
    
	FDelegateHandle m_InventoryLoadedHandle;

	FDelegateHandle m_FocusedTargetDie;
	
	FVector m_Input;

	float m_fCurrentExp;
	
	float m_fMaxExp;

	int m_FaceIndex;
	
	int m_HairIndex;
	
	FOnFloatChange m_OnLevelChanged;
	
	FOnFloatChange m_OnExpGaugeChanged;
	
	FOnFloatChange m_OnRemainExpChanged;

	UPROPERTY()
	TSet<AActor*> m_AlreadyHittenForIgnore;

	float m_fBonusDamage;
	//
protected:
	virtual void BeginPlay() override;

	void LoadExp(const USaveCharacterStatus* loadedSaveData);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);
	
	virtual void FocusTarget(APawn* target) override;

	void AutoPlayTick(bool useAuto);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void SetFullHairMesh();

	void SetHalfHairMesh();

	void SetDefaultBodyMesh();

	void SetDefaultShoeMesh();

	void SetDefaultGloveMesh();

	bool CreateItemActor(const FItemInstance* itemInst,AWeapon** wantCachePointer,UStaticMeshComponent** attachRoot);

	void BindASCInput();

	
	
	void SetLoadedData(const USaveCharacterStatus* loadedSaveData);
	
	void SetBaseAttackAbility(const FAnimStance* animStance);

	void SetBaseAttackData(float viewAngle,float viewRadius,float focusRange);
public:
	UFUNCTION(BlueprintCallable,Category="Interact")
	void SetBonusDamage(float v);
	UFUNCTION(BlueprintCallable,Category="Interact")
	virtual void AttackInput(float pressed);
	UFUNCTION(BlueprintCallable,Category="Interact")
	void InteractWithTarget();
	UFUNCTION(BlueprintCallable)
    void ResetCombo();
	void ShowOutlineOnTarget(AUnitPawn* Unit);
	void HideOutlineOnTarget();

	void EquipMesh(const FItemInstance* meshItem,ESlotsEquipAry slotWant);
	
	virtual bool SetCharacterLevel(int NewLevel)override;
	
	UPlayerBaseAttack* GetBaseAttackInst();

	void RemoveAllEffect();

	void SetAnimStance(const FAnimStance* animStance);

	void EarnExp(float expEarned);

    void OnSeeTarget(APawn* target);
	
    void OnCantSeeTarget(APawn* target);
	
	void OnCanSeeTargetBlock(APawn* target);
	
	ADiabloPlayerController* GetDiaController();

	FORCEINLINE FOnFloatChange& GetLevelDele()
	{
		return m_OnLevelChanged;
	}

	FORCEINLINE FOnFloatChange& GetExpGaugeDele()
	{
		return m_OnExpGaugeChanged;
	}

	FORCEINLINE FOnFloatChange& GetRemainExpDele()
	{
		return m_OnRemainExpChanged;
	}

	FORCEINLINE const TArray<TEnumAsByte< EObjectTypeQuery>>& GetAryTarget()
	{
		return m_AryTargetingObjectType;
	}

	FORCEINLINE const TArray<AActor*> GetAryIgnoreActor()
	{
		return m_AryIgnoreActor;
	}

	void ClearFocusedTarget(AUnitPawn* target);

	TSet<AActor*>& GetAlreadyAttacked()
	{
		return m_AlreadyHittenForIgnore;
	}

	 virtual void EndAttack() override;

	FORCEINLINE float GetBonusDamage()
	{
		return m_fBonusDamage;
	}
};

