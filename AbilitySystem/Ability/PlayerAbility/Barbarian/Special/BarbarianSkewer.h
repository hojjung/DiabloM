// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerAbility/BaseSkillTemplate/PlayerBaseAttack.h"
#include "BarbarianSkewer.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UBarbarianSkewer : public UPlayerBaseAttack
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
	
	bool TryClearCooldown(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar);

	void DealBleedEffectToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar);
};
