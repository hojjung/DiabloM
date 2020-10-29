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
	FGameplayTag m_TagTookPhysDamage;
	FGameplayTag m_TagTookFireDamage;
	FGameplayTag m_TagTookIceDamage;
	FGameplayTag m_TagTookElecDamage;
	FGameplayTag m_TagTookPoisonDamage;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag m_TagEventEndAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag m_TagEventBaseAttack;
 
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fDashTime;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fDashLimitRange;

	TArray<FCompositeSection> m_AryMontageSections;

	int m_nSectionIndex;

	FName GetSectionName();
public:
	void PlayAbilityAnimation(UAnimMontage* MontageToPlay, FName playSection,float AttackSpeed);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* m_BaseAttackMotion;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	void ResetComboSection();
	
protected:
	UFUNCTION()
    void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	UUnitMovement* GetMovement(APawn* want);

	void DashAttack(UUnitMovement* movementComp,FVector dashNormal,float dashLength,float dashTime);

	bool IsDashable( const FGameplayAbilityActorInfo* ActorInfo,float& outDistSqr,FVector& outDashNormal);
};


