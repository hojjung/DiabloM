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
	TSubclassOf<UGameplayEffect> m_GEInstigatorAttackSpeedBuff;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> m_GEBaseAttackGainResource;
	
	bool m_bIsGained;
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void GiveAttackSpeedBonusEffect(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar);
};
