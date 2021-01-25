// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Ability/PlayerAbility/PlayerDiabloAbility.h"
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "Characters/UnitMovement.h"

#include "PlayerBaseAttack.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerBaseAttack : public UPlayerDiabloAbility
{
	GENERATED_BODY()

public:
	UPlayerBaseAttack();
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fRadius;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fDuringMoveSpeedModifier;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fLevelPerDamageRate;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag m_TagEventEndAbility;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag m_TagEventBaseAttack;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* m_BaseAttackMotion;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> m_GETargetDamage;
	

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fDashTime;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fAttackRange;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fAttackAngle;
	
	int m_nSectionIndex;
	
	FGameplayTag m_TagTookPhysDamage;
	
	FGameplayTag m_TagTookFireDamage;
	
	FGameplayTag m_TagTookIceDamage;
	
	FGameplayTag m_TagTookElecDamage;
	
	FGameplayTag m_TagTookPoisonDamage;

	TArray<FCompositeSection> m_AryMontageSections;

	float m_fDashLimitSqr;

	float m_fDashAcceptSqr;

	float m_fAttackRangeSqr;
	
	float m_fAttackAngleCos;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	void PlayAbilityAnimation(UAnimMontage* MontageToPlay, FName playSection,float AttackSpeed);
	void TryDashAttack(const FGameplayAbilityActorInfo* ActorInfo);
	void TryNormalAttack();

	void SetDash(UUnitMovement* movementComp,FVector dashNormal,float dashLength,float dashTime);
	
	FName GetSectionName();
	
	UUnitMovement* GetMovement(APawn* want);

	bool IsDashable( const FGameplayAbilityActorInfo* ActorInfo,float& outDistSqr,FVector& outDashNormal);

	bool CheckAttackRange(const AActor* other) const;
	
	virtual bool DealDamageToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar);
	
public:
	void ResetComboSection();

	UFUNCTION()
	virtual void OnCancelledWrapper();

	UFUNCTION()
    virtual void OnCompletedWrapper();
	
	UFUNCTION()
    void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    virtual void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

public:
	FORCEINLINE float GetRadius()
	{
		return m_fRadius;
	}
};


