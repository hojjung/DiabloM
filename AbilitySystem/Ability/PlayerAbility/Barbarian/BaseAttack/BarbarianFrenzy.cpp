#include "BarbarianFrenzy.h"
#include "Characters/PlayerDiabloCharacter.h"

void UBarbarianFrenzy::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

void UBarbarianFrenzy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	m_bIsGained = false;

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
		if (!PlayerChar||!TargetChar||!TargetChar->IsAlive()||!CheckAttackRange(EventData.Target))
		{
			return;
		}

		if(DealDamageToTarget(TargetChar, PlayerChar))
		{
			GiveAttackSpeedBonusEffect(TargetChar, PlayerChar);
			
			if(m_bIsGained)
			{
				return;
			}
			
			FGameplayEffectSpecHandle EffectSpecHandle =MakeOutgoingGameplayEffectSpec(m_GEBaseAttackGainResource,1);

			PlayerChar->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);

			m_bIsGained = true;
		}
	}	
}

void UBarbarianFrenzy::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	m_bIsGained = false;
}


void UBarbarianFrenzy::GiveAttackSpeedBonusEffect(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
{
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(m_GEInstigatorAttackSpeedBuff,1);
	
	PlayerChar->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
}
