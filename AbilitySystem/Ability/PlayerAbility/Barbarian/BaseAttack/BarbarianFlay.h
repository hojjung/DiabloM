// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerAbility/BaseSkillTemplate/PlayerBaseAttack.h"
#include "BarbarianFlay.generated.h"

/**
 * 캐스팅 공격
 */
UCLASS()
class DIABLOM_API UBarbarianFlay : public UPlayerBaseAttack
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float m_fLevelPerBleedingDamage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> m_GETargetDamage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> m_GETargetBleeding;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;
	
	bool DealDamageToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar);

	void DealBleedEffectToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar);

};
