// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "AbilitySystem/Ability/PlayerAbility/PlayerUsePortal.h"
#include "AbilitySystem/Ability/PlayerAbility/Regen/PlayerManaRegenAbility.h"
#include "AbilitySystem/Ability/PlayerAbility/Regen/PlayerStaminaRegenAbility.h"
#include "Animations/DiaAniminstance.h"
#include "Characters/UnitPawn.h"
#include "Logic/PlayerAutoPlayFSM.h"
#include "Managers/DiabloCheatManager.h"
#include "SaveLoad/SaveCharacterStatus.h"
#include "PlayerDiabloCharacter.generated.h"

class UDiaStatPanel;
class IInteractable;
class ADiabloPlayerController;
class UCameraDissolve;
class UPlayerSensing;
class UPlayerBaseAttack;
class UDefaultFSM;
class UPlayerDiabloAttribute;
class UPlayerHealthPotion;
class UPlayerHpRegenAbility;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFloatChange,float);
DECLARE_MULTICAST_DELEGATE(FOnMove);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFocusTargetChanged,AUnitPawn*)
UCLASS()
class DIABLOM_API APlayerDiabloCharacter : public AUnitPawn
{
	GENERATED_BODY()
	
	friend UDiabloCheatManager;
	friend USaveLoadManager;
	friend UPlayerSensing;
	friend UDiabloGameInstance;
public:
	APlayerDiabloCharacter(const FObjectInitializer& objInit);

public:
	FOnMove m_OnMove;

	FOnFocusTargetChanged m_OnFocusTarget;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	TSubclassOf<UPlayerHpRegenAbility> m_GAPlayerHealthRegen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	TSubclassOf<UPlayerManaRegenAbility> m_GAPlayerManaRegen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	TSubclassOf<UPlayerStaminaRegenAbility> m_GAPlayerStaminaRegen;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	//TSubclassOf<UPlayerManaRegenAbility> m_GAPlayerRageRegen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	TSubclassOf<UPlayerHealthPotion> m_GAPlayerHealthPotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	TSubclassOf<UPlayerUsePortal> m_GAPlayerPortal;
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	bool m_bIsManualInit=false;
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
	UPlayerAutoPlayFSM* m_PlayerAutoPlay;
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
	
	TWeakInterfacePtr<IInteractable> m_FocusedInteractable;
	
	const FAnimStance* m_AnimStance;
	
	const FPlayerEntityTable* m_PlayerEntityData;

	FGameplayAbilitySpecHandle m_HpRegenHandle;

	FGameplayAbilitySpecHandle m_ResourceRegenHandle;

	FGameplayAbilitySpecHandle m_PotionHandle;

	FGameplayAbilitySpecHandle m_PortalHandle;
	
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

	FOnFloatChange m_OnGoldChanged;

	FCharacterDiedDelegate m_OnRevived;

	UPROPERTY()
	TSet<AActor*> m_AlreadyHittenForIgnore;

	bool m_bIsAttackInputPressed;

	bool m_bIsDead;

	TWeakObjectPtr<AUnitPawn> m_FocusOutlinePawn;;
	//
protected:

	virtual void BeginPlay() override;
	void LoadExp(const USaveCharacterStatus* loadedSaveData);

	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);
	
	

	

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
	
	virtual void FocusTarget(AUnitPawn* target) override;
	
	UFUNCTION(BlueprintCallable)
    void Revive();
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
	
	void SetAnimStance(const FAnimStance* animStance);

	void EarnExp(float expEarned);

	void EarnGold(float goldEarned);

	void SetGold(float goldEarned);
	
	bool SpendGold(float goldSpend);

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

	virtual FVector GetLastSeenLocation() override;

	friend UDiabloGameInstance;

	FORCEINLINE FCharacterDiedDelegate& GetOnRevived()
	{
		return m_OnRevived;
	}

	virtual bool IsAlive() const override;
	
	
	FGameplayAbilitySpec* DrinkPotion();

	void UsePortal();

	void CancelPortal();

	float GetCastSpeed();

	UPlayerDiabloAttribute* GetPlayerAttribute();

	UFUNCTION(BlueprintCallable)
	void PlayColorEffect(const FLinearColor& colorWant,float effectLength);
	
public:
	void GrantHpPotionAbility();
	
	void GrantBaseAttackAbility();
	
	void GrantHpRegenAbility();
	
	void GrantResourceRegenAbility();

	void GrantPortalAbility();

	FORCEINLINE IInteractable* GetFocusInteractable()
	{
		return  m_FocusedInteractable.Get();
	}
	
	FORCEINLINE float GetGold()
	{
		return m_fCurrentGold;
	}

	FORCEINLINE FOnFloatChange& GetOnGoldChanged()
	{
		return m_OnGoldChanged;
	}

	FORCEINLINE float GetExpPercent() const
	{
		return m_fCurrentExp / m_fMaxExp;
	}
	
	friend UDiaStatPanel;

    void SetAutoPlay(bool useAuto);
};

