#include "BarbarianSkewer.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/UnitPawn.h"

void UBarbarianSkewer::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TryDashAttack(ActorInfo);
}

void UBarbarianSkewer::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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

		if(DealDamageToTarget(TargetChar, PlayerChar))
		{
			if(TryClearCooldown(TargetChar, PlayerChar))
			{
				return;
			}
			
			DealBleedEffectToTarget(TargetChar, PlayerChar);

			FVector Location1 = PlayerChar->GetActorLocation();
    
			FVector Location2 = TargetChar->GetActorLocation();

			FVector DashNormal = (Location2 - Location1).GetSafeNormal();

			SetDash(Cast<UUnitMovement>(TargetChar->GetMovementComponent()),DashNormal,600.f,0.4f);

		}

		
	}
}


bool UBarbarianSkewer::TryClearCooldown(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
{
	if(!TargetChar->IsAlive())
	{
		PRINTF("CooldownClear");
		PlayerChar->GetDiaAbilitySystem()->SetGameplayEffectDurationHandle(m_CooldownHandle,0.f);
		//PlayerChar->GetDiaAbilitySystem()->Cool
		return true;
	}
	return false;
}

void UBarbarianSkewer::DealBleedEffectToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
{
	FGameplayEffectSpecHandle BleedEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
        m_GETargetBleeding, GetAbilityLevel());
	
	float Rate = m_fLevelPerBleedingDamage * GetAbilityLevel();

	float PhysDmg=PlayerChar->GetAttributeSet()->GetPhysicalDamage();
	
	BleedEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookPhysDamage,PhysDmg*Rate);

	PlayerChar->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToTarget(*BleedEffectSpecHandle.Data,TargetChar->GetDiaAbilitySystem());

	TryClearCooldown(TargetChar, PlayerChar);
}
