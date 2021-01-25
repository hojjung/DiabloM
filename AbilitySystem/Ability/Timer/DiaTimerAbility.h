// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "DiaTimerAbility.generated.h"

/**
 * 
 */
class AUnitPawn;
UCLASS()
class DIABLOM_API UDiaTimerAbility : public UDiabloAbility
{
    GENERATED_BODY()
public:
    UDiaTimerAbility();

protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
    float m_fInterval;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
    TSubclassOf<UGameplayEffect> m_EffectWant_SetByCaller;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
    FGameplayTag m_GamePlayTagSetByCaller;
    
    FTimerHandle m_TimerHandle_OnTimer;

    FGameplayEffectSpecHandle m_TimerEffectSpecHandle;

protected:
    virtual void SetGameEffectSpec();
    
    virtual void OnTimer();

    virtual float GetAttributeWantFrom();

    void SetTimer(const float TimeDelay);

    void SetSensingInterval(const float newSensingInterval);

    void SetSensingUpdatesEnabled(const bool bEnabled);

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                 const FGameplayEventData* TriggerEventData) override;

    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                            bool bWasCancelled) override;
};


