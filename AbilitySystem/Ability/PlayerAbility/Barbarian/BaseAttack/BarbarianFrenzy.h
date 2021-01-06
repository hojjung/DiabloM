// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerAbility/BaseSkillTemplate/PlayerBaseAttack.h"
#include "BarbarianFrenzy.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UBarbarianFrenzy : public UPlayerBaseAttack
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag m_TagEventTrace;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> m_GETargetDamage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> m_GEInstigatorAttackSpeedBuff;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;
	
	void DealDamageToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar);

	void GiveAttackSpeedBonusEffect(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar);
};
