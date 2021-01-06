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

		DealDamageToTarget(TargetChar, PlayerChar);

		TryGiveBashEffect(TargetChar, PlayerChar);
	}
}

void UBarbarianBash::DealDamageToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
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