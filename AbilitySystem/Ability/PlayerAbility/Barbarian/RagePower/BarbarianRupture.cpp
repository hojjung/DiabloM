#include "BarbarianRupture.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/UnitPawn.h"

void UBarbarianRupture::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	TryDashAttack(ActorInfo);
}

void UBarbarianRupture::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
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
	}
}


void UBarbarianRupture::DealBleedEffectToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
{
	FGameplayEffectSpecHandle BleedEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
        m_GETargetBleeding, GetAbilityLevel());
	
	float Rate = m_fLevelPerBleedingDamage * GetAbilityLevel();

	float PhysDmg=PlayerChar->GetAttributeSet()->GetPhysicalDamage();
	
	BleedEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookPhysDamage,PhysDmg*Rate);

	PlayerChar->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToTarget(*BleedEffectSpecHandle.Data,TargetChar->GetDiaAbilitySystem());

}
