#pragma once

#include "DiabloM.h"
#include "AbilitySystem/Ability/PlayerAbility/BaseSkillTemplate/PlayerBaseAttack.h"
#include "BarbarianRupture.generated.h"

UCLASS()
class DIABLOM_API UBarbarianRupture : public UPlayerBaseAttack
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fLevelPerBleedingDamage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> m_GETargetBleeding;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

	void DealBleedEffectToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar);
};
