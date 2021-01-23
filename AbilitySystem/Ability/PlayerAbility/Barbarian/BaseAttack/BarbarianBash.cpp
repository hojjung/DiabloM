// My First Hack n Slash


#include "BarbarianBash.h"


#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/UnitPawn.h"

void UBarbarianBash::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	m_nfStunPercent = 1.f - (1.f - 1.f / m_nWantStunPercentMaxCount);
}

void UBarbarianBash::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	m_bIsGained = false;
	
	TryDashAttack(ActorInfo);
}

void UBarbarianBash::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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
			TryGiveBashEffect(TargetChar, PlayerChar);

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

void UBarbarianBash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	m_bIsGained = false;
}


void UBarbarianBash::TryGiveBashEffect(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
{
	float Rand01 = FMath::RandRange(0.f,1.0f);

	if(m_nfStunPercent<Rand01)
	{
		return;
	}

	FGameplayEffectSpecHandle EffectSpecHandle =MakeOutgoingGameplayEffectSpec(m_GETargetBashStun,1);
	
	PlayerChar->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, TargetChar->GetDiaAbilitySystem());
}