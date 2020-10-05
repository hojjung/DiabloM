// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "BaseDiabloAttribute.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


class AUnitPawn;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatChanged,AUnitPawn*);
UCLASS()
class DIABLOM_API UBaseDiabloAttribute : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UBaseDiabloAttribute();

public:
	FOnStatChanged m_OnStatChanged;
	
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData TookDamage;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, TookDamage)
	
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, Health)
	
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, MaxHealth)
	
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, AttackSpeed)
	
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData PhysicalDamage;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, PhysicalDamage)
	
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData PhysicalDefense;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, PhysicalDefense)
	
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, MoveSpeed)
	
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamagePer;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, DamagePer)

	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData DefensePer;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, DefensePer)
	//속성데미지
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData ResFire;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, ResFire)
	
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData ResCold;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, ResCold)
	
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData ResPoison;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, ResPoison)
	
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData ResElec;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, ResElec)
	//
	UPROPERTY(BlueprintReadOnly, Category = "Type")
	FGameplayAttributeData AtkFire;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, AtkFire)
	
	UPROPERTY(BlueprintReadOnly, Category = "Type")
	FGameplayAttributeData AtkCold;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, AtkCold)
	
	UPROPERTY(BlueprintReadOnly, Category = "Type")
	FGameplayAttributeData AtkElec;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, AtkElec)
	
	UPROPERTY(BlueprintReadOnly, Category = "Type")
	FGameplayAttributeData AtkPoison;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, AtkPoison)
	//
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData AvoidChance;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, AvoidChance)

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	FGameplayAttributeData Accuracy;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, Accuracy)
	//Minion Only
    UPROPERTY(BlueprintReadOnly, Category = "XP")
	FGameplayAttributeData XPBounty;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, XPBounty)
	
    UPROPERTY(BlueprintReadOnly, Category = "Gold")
	FGameplayAttributeData GoldBounty;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, GoldBounty)

protected:
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

};
