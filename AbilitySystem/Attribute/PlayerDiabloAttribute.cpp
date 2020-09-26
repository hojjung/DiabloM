// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDiabloAttribute.h"

#include "GameplayEffectExtension.h"
#include "Characters/UnitPawn.h"

UPlayerDiabloAttribute::UPlayerDiabloAttribute()
{
	
}


void UPlayerDiabloAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetMaxStaminaAttribute());
	}
	else if (Attribute == GetMaxRageAttribute())
	{
		//rage start from 0
		//AdjustAttributeForMaxChange(Rage, MaxRage, NewValue, GetMaxRageAttribute());
	}

}
void UPlayerDiabloAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);

	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AUnitPawn* TargetCharacter = nullptr;
	
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AUnitPawn>(TargetActor);
	}


	if (Data.EvaluatedData.Attribute == GetHealthRegenAttribute())
	{
		m_OnChangedHealthRegen.Broadcast(GetHealthRegen());
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		m_OnManaGaugeChanged.Broadcast(GetMana()/GetMaxMana());
		//필요있나? 게이지만 보여주면 됨
	}
	else if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
		m_OnChangedMaxMana.Broadcast(GetMaxMana());
	}
	else if (Data.EvaluatedData.Attribute == GetManaRegenAttribute())
	{
		m_OnChangedManaRegen.Broadcast(GetManaRegen());
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		m_OnStaminaGaugeChanged.Broadcast(GetStamina()/GetMaxStamina());
	}
	else if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
	{
		m_OnChangedMaxStamina.Broadcast(GetMaxStamina());
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaRegenAttribute())
	{
		m_OnChangedStaminaRegen.Broadcast(GetStaminaRegen());
	}
	else if (Data.EvaluatedData.Attribute == GetRageAttribute())
	{
		//NO
	}
	else if (Data.EvaluatedData.Attribute == GetMaxRageAttribute())
	{
		m_OnChangedMaxRage.Broadcast(GetMaxRage());
	}
	else if (Data.EvaluatedData.Attribute == GetRageRegenAttribute())
	{
		m_OnChangedRageRegen.Broadcast(GetRageRegen());
	}
	else if (Data.EvaluatedData.Attribute == GetStrAttribute())
	{
		m_OnChangedStr.Broadcast(GetStr());
	}
	else if (Data.EvaluatedData.Attribute == GetDexAttribute())
	{
		m_OnChangedDex.Broadcast(GetDex());
	}
	else if (Data.EvaluatedData.Attribute == GetIntAttribute())
	{
		m_OnChangedInt.Broadcast(GetInt());
	}
	else if (Data.EvaluatedData.Attribute == GetVitAttribute())
	{
		m_OnChangedVit.Broadcast(GetVit());
	}
	else if (Data.EvaluatedData.Attribute == GetCoolDownReduceAttribute())
	{
		m_OnChangedCoolDownReduce.Broadcast(GetCoolDownReduce());
	}
	else if (Data.EvaluatedData.Attribute == GetCastingSpeedAttribute())
	{
		m_OnChangedCastingSpeed.Broadcast(GetCastingSpeed());
	}
	else if (Data.EvaluatedData.Attribute == GetAllSkillBonusAttribute())
	{
		m_OnChangedAllSkillBonus.Broadcast(GetAllSkillBonus());
	}
	else if (Data.EvaluatedData.Attribute == GetMagicItemDropBonusAttribute())
	{
		m_OnChangedMagicItemDropBonus.Broadcast(GetMagicItemDropBonus());
	}
	else if (Data.EvaluatedData.Attribute == GetPotionBonusPerAttribute())
	{
		m_OnChangedPotionBonusPer.Broadcast(GetPotionBonusPer());
	}
	else if (Data.EvaluatedData.Attribute == GetPotionCoolDownReduceAttribute())
	{
		m_OnChangedPotionCoolDownReduce.Broadcast(GetPotionCoolDownReduce());
	}
	else if (Data.EvaluatedData.Attribute == GetGoldBonusPerAttribute())
	{
		m_OnChangedGoldBonusPer.Broadcast(GetGoldBonusPer());
	}
	else if (Data.EvaluatedData.Attribute == GetExpBonusPerAttribute())
	{
		m_OnChangedExpBonusPer.Broadcast(GetExpBonusPer());
	}
	else if (Data.EvaluatedData.Attribute == GetLifeStealAttribute())
	{
		m_OnChangedLifeSteal.Broadcast(GetLifeSteal());
	}
	else if (Data.EvaluatedData.Attribute == GetDoubleAttackChanceAttribute())
	{
		m_OnChangedDoubleAttackChance.Broadcast(GetDoubleAttackChance());
	}
	else if (Data.EvaluatedData.Attribute == GetBashChanceAttribute())
	{
		m_OnChangedBashChance.Broadcast(GetBashChance());
	}
	else if (Data.EvaluatedData.Attribute == GetCriticalChanceAttribute())
	{
		m_OnChangedCriticalChance.Broadcast(GetCriticalChance());
	}
	else if (Data.EvaluatedData.Attribute == GetBlockChanceAttribute())
	{
		m_OnChangedBlockChance.Broadcast(GetBlockChance());	
	}
	
}

