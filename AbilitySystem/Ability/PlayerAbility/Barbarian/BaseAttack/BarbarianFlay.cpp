#include "BarbarianFlay.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/UnitPawn.h"

void UBarbarianFlay::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TryDashAttack(ActorInfo);
}

void UBarbarianFlay::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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
			DealBleedEffectToTarget(TargetChar, PlayerChar);
		}
		else
		{
			PRINTF("Flay Missed");
		}
	}
}

bool UBarbarianFlay::DealDamageToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
{
	FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
        m_GETargetDamage, GetAbilityLevel());

	float Rate = m_fLevelPerDamageRate * GetAbilityLevel();

	float PhysDmg=PlayerChar->GetAttributeSet()->GetPhysicalDamage();
	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookPhysDamage,PhysDmg*Rate);

	float FireDmg=PlayerChar->GetAttributeSet()->GetAtkFire();
	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookFireDamage,FireDmg*Rate);

	float ElecDmg=PlayerChar->GetAttributeSet()->GetAtkElec();
	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookElecDamage,ElecDmg*Rate);

	float PoisonDmg=PlayerChar->GetAttributeSet()->GetAtkPoison();
	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookPoisonDamage,PoisonDmg*Rate);

	float IceDmg=PlayerChar->GetAttributeSet()->GetAtkCold();
	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookIceDamage,IceDmg*Rate);

	FActiveGameplayEffectHandle ActiveEffectHandle = PlayerChar->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToTarget(
		*DamageEffectSpecHandle.Data, TargetChar->GetDiaAbilitySystem());

	return ActiveEffectHandle.bPassedFiltersAndWasExecuted;
}

void UBarbarianFlay::DealBleedEffectToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
{
	FGameplayEffectSpecHandle BleedEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
        m_GETargetBleeding, GetAbilityLevel());
	
	float Rate = m_fLevelPerBleedingDamage * GetAbilityLevel();

	float PhysDmg=PlayerChar->GetAttributeSet()->GetPhysicalDamage();
	
	BleedEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookPhysDamage,PhysDmg*Rate);

	PlayerChar->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToTarget(*BleedEffectSpecHandle.Data,TargetChar->GetDiaAbilitySystem());

}
