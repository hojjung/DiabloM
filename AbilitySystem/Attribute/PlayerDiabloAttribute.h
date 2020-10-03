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
	
	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, ManaRegen)
	
	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, StaminaRegen)
	
	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData Rage;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Rage)
	
	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MaxRage)

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData RageRegen;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, RageRegen)

	UPROPERTY(BlueprintReadOnly, Category = "CoreStats")
	FGameplayAttributeData Str;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Str)

	UPROPERTY(BlueprintReadOnly, Category = "CoreStats")
	FGameplayAttributeData Dex;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Dex)

	UPROPERTY(BlueprintReadOnly, Category = "CoreStats")
	FGameplayAttributeData Int;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Int)

	UPROPERTY(BlueprintReadOnly, Category = "CoreStats")
	FGameplayAttributeData Vit;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Vit)
	//
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	FGameplayAttributeData CoolDownReduce;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, CoolDownReduce)

	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	FGameplayAttributeData CastingSpeed;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, CastingSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	FGameplayAttributeData AllSkillBonus;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, AllSkillBonus)

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGameplayAttributeData MagicItemDropBonus;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MagicItemDropBonus)

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGameplayAttributeData PotionBonusPer;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, PotionBonusPer)

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGameplayAttributeData PotionCoolDownReduce;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, PotionCoolDownReduce)

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGameplayAttributeData GoldBonusPer;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, GoldBonusPer)

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGameplayAttributeData ExpBonusPer;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, ExpBonusPer)
	//
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData LifeSteal;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, LifeSteal)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DoubleAttackChance;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, DoubleAttackChance)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData BashChance;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, BashChance)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, CriticalChance)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData CriticalDamage;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, CriticalDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, BlockChance)

	UPROPERTY(BlueprintReadOnly, Category = "Grow")
	FGameplayAttributeData MaxExpForLevelUp;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MaxExpForLevelUp)

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	

};
