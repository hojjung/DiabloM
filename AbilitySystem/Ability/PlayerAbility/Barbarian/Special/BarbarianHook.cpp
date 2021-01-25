// My First Hack n Slash


#include "BarbarianHook.h"


#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/UnitPawn.h"


void UBarbarianHook::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	TryNormalAttack();
}

void UBarbarianHook::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	const AUnitPawn* TargetChar=Cast<AUnitPawn>( EventData.Target);
	APlayerDiabloCharacter* PlayerChar=Cast<APlayerDiabloCharacter>(GetAvatarActorFromActorInfo());

	if (EventTag == m_TagEventBaseAttack)
	{
		if (!PlayerChar||!TargetChar||!TargetChar->IsAlive())
		{
			return;
		}

		if(DealDamageToTarget(TargetChar,PlayerChar))
		{
			FVector Location1 = TargetChar->GetActorLocation();
    
			FVector Location2 = PlayerChar->GetActorLocation();

			FVector DashNormal = (Location2 - Location1).GetSafeNormal();

			float Length = FVector::Dist2D(Location1,Location2);
			
			SetDash(Cast<UUnitMovement>(TargetChar->GetMovementComponent()),DashNormal,Length,0.5f);
		}
	}
}
