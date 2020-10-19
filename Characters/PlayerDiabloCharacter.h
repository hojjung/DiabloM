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
	TSubclassOf<UDiabloAbility> m_GAPlayerHealthRegen;
	//
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
	UPROPERTY()
	TSubclassOf<UPlayerBaseAttack> m_PlayerBaseAttack;
	
	IInteractable* m_FocusedInteractable;
	
	const FAnimStance* m_AnimStance;
	
	const FPlayerEntityTable* m_PlayerEntityData;


	
	FDelegateHandle m_InventoryUpdateHandle;
    
	FDelegateHandle m_InventoryLoadedHandle;

	FDelegateHandle m_FocusedTargetDie;
	
	FVector m_Input;

	float m_fCurrentExp;
	
	float m_fMaxExp;

	float m_fCurrentGold;

	int m_FaceIndex;
	
	int m_HairIndex;
	
	FOnFloatChange m_OnLevelChanged;
	
	FOnFloatChange m_OnExpGaugeChanged;
	
	FOnFloatChange m_OnRemainExpChanged;

	FCharacterDiedDelegate m_OnRevived;

	UPROPERTY()
	TSet<AActor*> m_AlreadyHittenForIgnore;

	float m_fBonusDamage;

	bool m_bIsAttackInputPressed;

	bool m_bIsDead;
	//
protected:
	void LoadExp(const USaveCharacterStatus* loadedSaveData);

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);
	
	virtual void FocusTarget(AUnitPawn* target) override;

	void AutoPlayTick(bool useAuto);

	virtual void Tick(float DeltaTime) override;
	
	void TickAttack();

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
	void Init();
	void GrantHpRegenAbility();
	UFUNCTION(BlueprintCallable)
    void Revive();
	UFUNCTION(BlueprintCallable,Category="Interact")
	void SetBonusDamage(float v);
	UFUNCTION(BlueprintCallable,Category="Interact")
	void OnAttackPressed();
	UFUNCTION(BlueprintCallable,Category="Interact")
    void OnAttackRelease();
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
	void GrantBaseAttackAbility();

	void SetAnimStance(const FAnimStance* animStance);

	void EarnExp(float expEarned);

	void EarnGold(float goldEarned);

    void OnSeeTarget(APawn* target);
	
    void OnCantSeeTarget(APawn* target);
	
	void OnCanSeeTargetBlock(APawn* target);
	
	ADiabloPlayerController* GetDiaController();

	virtual void Die()override;
	virtual void OnDeathAnimEnd()override;
	
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

	virtual FVector GetLastSeenLocation() override;

	friend UDiabloGameInstance;

	FORCEINLINE FCharacterDiedDelegate& GetOnRevived()
	{
		return m_OnRevived;
	}

	virtual bool IsAlive() override;
};

