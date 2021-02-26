// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "WeakInterfacePtr.h"
#include "Characters/UnitPawn.h"
#include "Managers/DiabloCheatManager.h"
#include "PlayerDiabloCharacter.generated.h"

class UEquipManager;
class UPlayerUpgradeManager;
class UDiaStatPanel;
class ADiabloPlayerController;
class UCameraDissolve;
class UPlayerSensing;
class UDefaultFSM;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFloatChange,float);
DECLARE_MULTICAST_DELEGATE(FOnMove);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFocusTargetChanged,AUnitPawn*);
DECLARE_MULTICAST_DELEGATE(FOnPlVisualChanged);
UCLASS( BlueprintType, Blueprintable)
class DIABLOM_API APlayerDiabloCharacter : public AUnitPawn
{
	GENERATED_BODY()
	
	friend UDiabloCheatManager;
	friend UPlayerSensing;
	friend UDiabloGameInstance;
public:
	APlayerDiabloCharacter(const FObjectInitializer& objInit);

public:
	FOnMove m_OnMove;

	FOnFocusTargetChanged m_OnFocusTarget;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	TArray<TEnumAsByte< EObjectTypeQuery>> m_AryTargetingObjectType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UCameraDissolve* m_DissolveCam;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UCameraComponent* m_TopCamera;
	//
protected:
	UPROPERTY()
	UPlayerUpgradeManager* m_PlUpgradeManager; 
	UPROPERTY()
	UPlayerSensing* m_PlayerSense;
	UPROPERTY()
	TArray< AActor*> m_AryIgnoreActor;
	UPROPERTY()
	ADiabloPlayerController* m_PlayerCon;
	
	const FPlayerEntityTable* m_PlayerEntityData;

	FDelegateHandle m_InventoryUpdateHandle;
    
	FDelegateHandle m_InventoryLoadedHandle;

	FDelegateHandle m_FocusedTargetDie;
	
	FVector m_Input;

	float m_fCurrentExp;
	
	float m_fMaxExp;

	float m_fCurrentGold;

	int m_SkinIndex;
	
	FOnFloatChange m_OnLevelChanged;
	
	FOnFloatChange m_OnExpGaugeChanged;
	
	FOnFloatChange m_OnRemainExpChanged;

	FOnFloatChange m_OnGoldChanged;

	FCharacterDiedDelegate m_OnRevived;

	UPROPERTY()
	TSet<AActor*> m_AlreadyHittenForIgnore;

	bool m_bIsDead;

	TWeakObjectPtr<AUnitPawn> m_FocusOutlinePawn;;

	const FPlayerEntityTable* m_PlayerData;

	FTimerHandle m_AttackTimer;

	bool m_bIsManualMove;

	TQueue<char> m_QueDmgType;

protected:
	virtual void BeginPlay() override;
	
	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void SetBaseAttackData(float viewAngle,float viewRadius,float focusRange);

	virtual FVector GetLastSeenLocation() override;

	virtual float TryAttack() override;

	void ApplyDamage(AUnitPawn* target,const BigInt& finalDmg);

public:
	void PlayerClassDataInject(UEquipManager* manager);
	
	virtual void FocusTarget(AUnitPawn* target) override;
	
	UFUNCTION(BlueprintCallable)
    void Revive();
	UFUNCTION(BlueprintCallable,Category="Interact")
	void InteractWithTarget();
	UFUNCTION(BlueprintCallable)
    void ResetCombo();
	
	void ShowOutlineOnTarget(AUnitPawn* Unit);
	
	void HideOutlineOnTarget();

	void EarnExp(float expEarned);

	void EarnGold(float goldEarned);

	void SetGold(float goldEarned);
	
	bool SpendGold(float goldSpend);

	float GetAttackSpeedMultiple();

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

	void UpdateRegenAbility();

	friend UDiabloGameInstance;

	FORCEINLINE FCharacterDiedDelegate& GetOnRevived()
	{
		return m_OnRevived;
	}

	virtual bool IsAlive() const override;
	
	float GetCastSpeed();


	UFUNCTION(BlueprintCallable)
	void PlayColorEffect(const FLinearColor& colorWant,float effectLength);
	
public:

	
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

	UFUNCTION(BlueprintCallable)
	void ApplyDamageToTarget();
	UFUNCTION(BlueprintCallable)
    void ApplyDamageToTargets(TArray<FHitResult>& aryTargets);

	void ApplyMoveSpeedToOrigin();
};

