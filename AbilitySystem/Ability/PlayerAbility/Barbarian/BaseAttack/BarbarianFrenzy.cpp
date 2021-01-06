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
		if (!PlayerChar||!TargetChar||!TargetChar->IsAlive()||!CheckAttackRange(EventData.Target))
		{
			return;
		}

		DealDamageToTarget(TargetChar, PlayerChar);

		GiveAttackSpeedBonusEffect(TargetChar, PlayerChar);
	}	
}

void UBarbarianFrenzy::DealDamageToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
{
	FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
        m_GETargetDamage, GetAbilityLevel());

	float PhysDmg=PlayerChar->GetAttributeSet()->GetPhysicalDamage()*PlayerChar->GetBonusDamage();
	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookPhysDamage,PhysDmg);

	float FireDmg=PlayerChar->GetAttributeSet()->GetAtkFire()*PlayerChar->GetBonusDamage();
	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookFireDamage,FireDmg);

	float ElecDmg=PlayerChar->GetAttributeSet()->GetAtkElec()*PlayerChar->GetBonusDamage();
	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookElecDamage,ElecDmg);

	float PoisonDmg=PlayerChar->GetAttributeSet()->GetAtkPoison()*PlayerChar->GetBonusDamage();
	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookPoisonDamage,PoisonDmg);

	float IceDmg=PlayerChar->GetAttributeSet()->GetAtkCold()*PlayerChar->GetBonusDamage();
	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookIceDamage,IceDmg);

	PlayerChar->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data,TargetChar->GetDiaAbilitySystem());
}

void UBarbarianFrenzy::GiveAttackSpeedBonusEffect(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
{
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(m_GEInstigatorAttackSpeedBuff,1);
	
	PlayerChar->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, TargetChar->GetDiaAbilitySystem());
}
