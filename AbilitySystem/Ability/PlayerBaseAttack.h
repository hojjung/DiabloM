// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Ability/DiabloAbility.h"
#include "PlayerBaseAttack.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerBaseAttack : public UDiabloAbility
{
	GENERATED_BODY()

public:
	UPlayerBaseAttack();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* m_BaseAttackMotion;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Damage;

	UFUNCTION()
    void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	TArray<FCompositeSection> m_AryMontageSections;

	int m_nSectionIndex;

	FName GetSectionName();
};


