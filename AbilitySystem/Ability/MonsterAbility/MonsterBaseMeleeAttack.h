// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "Characters/MonsterPawn.h"
#include "Characters/UnitMovement.h"
#include "MonsterBaseMeleeAttack.generated.h"

UCLASS()
class DIABLOM_API UMonsterBaseMeleeAttack : public UDiabloAbility
{
	GENERATED_BODY()
	
public:
	UMonsterBaseMeleeAttack();
protected:
	FGameplayTag m_TagTookDamage;
	FGameplayTag m_TagEventEndAbility;
	FGameplayTag m_TagEventBaseAttack;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fAttackRange;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fAttackAngle;

	float m_fAttackRangeSqr;
	float m_fAttackAngleCos;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* m_BaseAttackMotion;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY()
	AMonsterPawn* m_MonsterPawn;

protected:
	void PlayAbilityAnimation(UAnimMontage* MontageToPlay, FName playSection,float AttackSpeed);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	bool CheckAttackRange(const AActor* other) const;
	
	UFUNCTION()
    void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
    void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
