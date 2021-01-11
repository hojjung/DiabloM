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

		if(DealDamageToTarget(TargetChar, PlayerChar))
		{
			TryGiveBashEffect(TargetChar, PlayerChar);
		}
	}
}

bool UBarbarianBash::DealDamageToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
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

	FActiveGameplayEffectHandle Result = PlayerChar->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToTarget(
		*DamageEffectSpecHandle.Data, TargetChar->GetDiaAbilitySystem());

	return Result.bPassedFiltersAndWasExecuted;
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