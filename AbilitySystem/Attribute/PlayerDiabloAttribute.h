// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "PlayerDiabloAttribute.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerDiabloAttribute : public UBaseDiabloAttribute
{
	GENERATED_BODY()
	
public:
	UPlayerDiabloAttribute();
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, HealthRegen)
	FOnStatChanged m_OnChangedHealthRegen;
	
	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Mana)
	FOnStatChanged m_OnManaGaugeChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MaxMana)
	FOnStatChanged m_OnChangedMaxMana;

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, ManaRegen)
	FOnStatChanged m_OnChangedManaRegen;
	
	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Stamina)
	FOnStatChanged m_OnStaminaGaugeChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MaxStamina)
	FOnStatChanged m_OnChangedMaxStamina;

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, StaminaRegen)
	FOnStatChanged m_OnChangedStaminaRegen;
	
	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData Rage;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Rage)
	FOnStatChanged m_OnRageGaugeChanged;
	
	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MaxRage)
	FOnStatChanged m_OnChangedMaxRage;

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData RageRegen;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, RageRegen)
	FOnStatChanged m_OnChangedRageRegen;

	UPROPERTY(BlueprintReadOnly, Category = "CoreStats")
	FGameplayAttributeData Str;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Str)
	FOnStatChanged m_OnChangedStr;

	UPROPERTY(BlueprintReadOnly, Category = "CoreStats")
	FGameplayAttributeData Dex;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Dex)
	FOnStatChanged m_OnChangedDex;

	UPROPERTY(BlueprintReadOnly, Category = "CoreStats")
	FGameplayAttributeData Int;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Int)
	FOnStatChanged m_OnChangedInt;

	UPROPERTY(BlueprintReadOnly, Category = "CoreStats")
	FGameplayAttributeData Vit;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Vit)
	FOnStatChanged m_OnChangedVit;
	//
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	FGameplayAttributeData CoolDownReduce;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, CoolDownReduce)
	FOnStatChanged m_OnChangedCoolDownReduce;

	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	FGameplayAttributeData CastingSpeed;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, CastingSpeed)
	FOnStatChanged m_OnChangedCastingSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	FGameplayAttributeData AllSkillBonus;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, AllSkillBonus)
	FOnStatChanged m_OnChangedAllSkillBonus;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGameplayAttributeData MagicItemDropBonus;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MagicItemDropBonus)
	FOnStatChanged m_OnChangedMagicItemDropBonus;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGameplayAttributeData PotionBonusPer;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, PotionBonusPer)
	FOnStatChanged m_OnChangedPotionBonusPer;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGameplayAttributeData PotionCoolDownReduce;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, PotionCoolDownReduce)
	FOnStatChanged m_OnChangedPotionCoolDownReduce;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGameplayAttributeData GoldBonusPer;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, GoldBonusPer)
	FOnStatChanged m_OnChangedGoldBonusPer;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGameplayAttributeData ExpBonusPer;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, ExpBonusPer)
	FOnStatChanged m_OnChangedExpBonusPer;
	//
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData LifeSteal;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, LifeSteal)
	FOnStatChanged m_OnChangedLifeSteal;

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DoubleAttackChance;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, DoubleAttackChance)
	FOnStatChanged m_OnChangedDoubleAttackChance;

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData BashChance;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, BashChance)
	FOnStatChanged m_OnChangedBashChance;

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, CriticalChance)
	FOnStatChanged m_OnChangedCriticalChance;

	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, BlockChance)
	FOnStatChanged m_OnChangedBlockChance;

	UPROPERTY(BlueprintReadOnly, Category = "Grow")
	FGameplayAttributeData MaxExpForLevelUp;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MaxExpForLevelUp)
protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	

};
