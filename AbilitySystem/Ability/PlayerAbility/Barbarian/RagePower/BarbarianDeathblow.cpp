// My First Hack n Slash


#include "BarbarianDeathblow.h"


#include "Characters/UnitPawn.h"

void UBarbarianDeathblow::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TryNormalAttack();
}

void UBarbarianDeathblow::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::EventReceived(EventTag, EventData);
	
	const AUnitPawn* TargetChar=Cast<AUnitPawn>( EventData.Target);
	APlayerDiabloCharacter* PlayerChar=Cast<APlayerDiabloCharacter>(GetAvatarActorFromActorInfo());

	if (EventTag == m_TagEventBaseAttack)
	{
		if (!PlayerChar||!TargetChar||!TargetChar->IsAlive())
		{
			return;
		}

		if(DealDamageToTarget(TargetChar, PlayerChar))
		{
			
		}
	}
}

