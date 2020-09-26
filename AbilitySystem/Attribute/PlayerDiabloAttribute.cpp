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
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		//필요있나? 게이지만 보여주면 됨
	}
	else if (Data.EvaluatedData.Attribute == GetMaxManaAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetManaRegenAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaRegenAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetRageAttribute())
	{
		//NO
	}
	else if (Data.EvaluatedData.Attribute == GetMaxRageAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetRageRegenAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetStrAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetDexAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetIntAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetVitAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetCoolDownReduceAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetCastingSpeedAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetAllSkillBonusAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetMagicItemDropBonusAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetPotionBonusPerAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetPotionCoolDownReduceAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetGoldBonusPerAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetExpBonusPerAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetLifeStealAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetDoubleAttackChanceAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetBashChanceAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetCriticalChanceAttribute())
	{
	}
	else if (Data.EvaluatedData.Attribute == GetBlockChanceAttribute())
	{
	}
	
	m_OnStatChanged.Broadcast();
}

