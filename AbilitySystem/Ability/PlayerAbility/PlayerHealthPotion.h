// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerAbility/PlayerDiabloAbility.h"
#include "PlayerHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerHealthPotion : public UPlayerDiabloAbility
{
	GENERATED_BODY()

public:
	UPlayerHealthPotion();

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag m_TagHealthHeal;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag m_TagEventEndAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag m_TagEventReceive;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fHealPercent;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* m_PotionDrinkMotion;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> m_GEPotionHeal;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION()
    void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	void PlayAbilityAnimation(UAnimMontage* MontageToPlay, FName playSection,float AttackSpeed);
};
