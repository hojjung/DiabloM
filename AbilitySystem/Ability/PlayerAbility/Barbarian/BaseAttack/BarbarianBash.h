// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/PlayerAbility/BaseSkillTemplate/PlayerBaseAttack.h"
#include "BarbarianBash.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UBarbarianBash : public UPlayerBaseAttack
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> m_GETargetDamage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> m_GETargetBashStun;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int m_nWantStunPercentMaxCount = 5;

	float m_nfStunPercent;
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;
	
	bool DealDamageToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar);

	void TryGiveBashEffect(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar);
};
