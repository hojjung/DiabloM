#include "BarbarianFrenzy.h"
#include "Characters/PlayerDiabloCharacter.h"

void UBarbarianFrenzy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TryDashAttack(ActorInfo);
}

void UBarbarianFrenzy::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	const AUnitPawn* TargetChar=Cast<AUnitPawn>( EventData.Target);
	APlayerDiabloCharacter* PlayerChar=Cast<APlayerDiabloCharacter>(GetAvatarActorFromActorInfo());

	if (EventTag == m_TagEventEndAbility)
	{
		ResetComboSection();
		
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		
		return;
	}

	if (EventTag == m_TagEventBaseAttack)
	{
		if (!PlayerChar||!TargetChar||!TargetChar->IsAlive())
		{
			return;
		}

		DealDamageToTarget(TargetChar, PlayerChar);

		GiveAttackSpeedBonusEffect(TargetChar, PlayerChar);
	}	
}


void UBarbarianFrenzy::GiveAttackSpeedBonusEffect(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
{
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(m_GEInstigatorAttackSpeedBuff,1);
	
	PlayerChar->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, TargetChar->GetDiaAbilitySystem());
}
