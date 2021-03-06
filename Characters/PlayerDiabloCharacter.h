// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "WeakInterfacePtr.h"
#include "Characters/UnitPawn.h"
#include "Managers/DiabloCheatManager.h"
#include "Managers/MonsterSpawnManager.h"


#include "PlayerDiabloCharacter.generated.h"

struct FPetSpec;
struct FAccessorySpec;
struct FWingSpec;
struct FWeaponSpec;
struct FPlayerClassSpec;
class UEquipManager;
class UPlayerUpgradeManager;
class UDiaStatPanel;
class ADiabloPlayerController;
class UCameraDissolve;
class UPlayerSensing;
class UDefaultFSM;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFloatChange,float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFloatChange2,float,float);
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

	FOnFloatChange2 m_OnRageChanged;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player")
	TArray<TEnumAsByte< EObjectTypeQuery>> m_AryTargetingObjectType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UCameraDissolve* m_DissolveCam;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UCameraComponent* m_TopCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Player")
	UChildActorComponent* m_PetComp;
	//
protected:
	UPROPERTY()
	UPlayerUpgradeManager* m_PlUpgradeManager;
	UPROPERTY()
	UEquipManager* m_EquipManager;
	UPROPERTY()
	UPlayerSensing* m_PlayerSense;
	UPROPERTY()
	TArray< AActor*> m_AryIgnoreActor;
	UPROPERTY()
	ADiabloPlayerController* m_PlayerCon;
	
	const FPlayerClassSpec* m_PlayerEntityData;

	FVector m_Input;

	UPROPERTY()
	TSet<AActor*> m_AlreadyHittenForIgnore;

	bool m_bIsDead;

	TWeakObjectPtr<AUnitPawn> m_FocusOutlinePawn;;

	FTimerHandle m_AttackTimer;

	bool m_bIsManualMove;

	TQueue<char> m_QueDmgType;

	UPROPERTY()
	AEquipmentActor* m_CreatedWing;
	UPROPERTY()
	AEquipmentActor* m_CreatedWeapon;
	UPROPERTY()
	AEquipmentActor* m_CreatedPet;

	float m_fMaxRage;

	float m_fGainRagePer;

	float m_fCurrentRage;

	float m_fBuff01MaxTime;

	float m_fBuff02MaxTime;

	float m_fBuff01DeltaCount;

	float m_fBuff02DeltaCount;
	
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

	bool GetDmg(BigInt& outDmg);

	//
	void StartBuff01(float sec);

	void EndBuff01();

	void StartBuff02(float sec);

	void EndBuff02();

public:
	void PlayerClassDataInject(const FPlayerClassSpec& spec);
	
	void WeaponDataInject(const FWeaponSpec& spec);
	
	void WingDataInject(const FWingSpec& spec);
	
	void AccessoryDataInject(const FAccessorySpec& spec);
	
	void PetDataInject(const FPetSpec& spec);

public:
	virtual void FocusTarget(AUnitPawn* target) override;
	
	UFUNCTION(BlueprintCallable)
    void Revive();
	
	void ShowOutlineOnTarget(AUnitPawn* Unit);
	
	void HideOutlineOnTarget();

	float GetAttackSpeedMultiple();

	ADiabloPlayerController* GetDiaController();

	virtual void Die()override;
	
	virtual void OnDeathAnimEnd()override;
	
	FORCEINLINE const TArray<TEnumAsByte< EObjectTypeQuery>>& GetAryTarget()
	{
		return m_AryTargetingObjectType;
	}

	FORCEINLINE const TArray<AActor*>& GetAryIgnoreActor()
	{
		return m_AryIgnoreActor;
	}

	void ClearFocusedTarget(AUnitPawn* target);

	TSet<AActor*>& GetAlreadyAttacked()
	{
		return m_AlreadyHittenForIgnore;
	}

	virtual bool IsAlive() const override;
	
	UFUNCTION(BlueprintCallable)
	void PlayColorEffect(const FLinearColor& colorWant,float effectLength);

	
	void TriggerSkill(const FName& name,TArray<FHitResult>* aryHits=nullptr);

public:
	friend UDiabloGameInstance;
	friend UDiaStatPanel;

	UFUNCTION(BlueprintCallable)
	void ApplyDamageToTarget();
	UFUNCTION(BlueprintCallable)
    void ApplyDamageToTargets(TArray<FHitResult>& aryTargets);

	void ApplyMoveSpeedToOrigin();

	virtual int GetAccuLevel() override;

	FVector GetCameraLoc() const
	{
		return m_TopCamera->GetComponentLocation();
	}

	bool GetIsManualMove() const
	{
		return m_bIsManualMove;
	}

	friend UMonsterSpawnManager;

	void SetManualMoveLocation(FVector goalLocation);

	void GainRagePoint();

	bool SpendRagePoint(float rage);


	float GetRage()
	{
		return m_fCurrentRage;
	}
};

