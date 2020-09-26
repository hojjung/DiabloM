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


DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatChanged,float);
UCLASS()
class DIABLOM_API UBaseDiabloAttribute : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UBaseDiabloAttribute();

public:
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData TookDamage;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, TookDamage)
	//FOnStatChanged m_OnChangedTookDamage;
	//need broadcast
	
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, Health)
	FOnStatChanged m_OnHealthChangePer;
	//FOnStatChanged m_OnChangedHealth;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, MaxHealth)
	FOnStatChanged m_OnChangedMaxHealth;
	
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, AttackSpeed)
	FOnStatChanged m_OnChangedAttackSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData PhysicalDamage;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, PhysicalDamage)
	FOnStatChanged m_OnChangedPhysicalDamage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData PhysicalDefense;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, PhysicalDefense)
	FOnStatChanged m_OnChangedPhysicalDefense;
	
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, MoveSpeed)
	FOnStatChanged m_OnChangedMoveSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamagePer;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, DamagePer)
	FOnStatChanged m_OnChangedDamagePer;

	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData DefensePer;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, DefensePer)
	FOnStatChanged m_OnChangedDefensePer;
	//속성데미지
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData ResFire;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, ResFire)
	FOnStatChanged m_OnChangedResFire;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData ResCold;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, ResCold)
	FOnStatChanged m_OnChangedResCold;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData ResPoison;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, ResPoison)
	FOnStatChanged m_OnChangedResPoison;
	
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData ResElec;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, ResElec)
	FOnStatChanged m_OnChangedResElec;
	//
	UPROPERTY(BlueprintReadOnly, Category = "Type")
	FGameplayAttributeData AtkFire;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, AtkFire)
	FOnStatChanged m_OnChangedAtkFire;
	
	UPROPERTY(BlueprintReadOnly, Category = "Type")
	FGameplayAttributeData AtkCold;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, AtkCold)
	FOnStatChanged m_OnChangedAtkCold;
	
	UPROPERTY(BlueprintReadOnly, Category = "Type")
	FGameplayAttributeData AtkElec;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, AtkElec)
	FOnStatChanged m_OnChangedAtkElec;
	
	UPROPERTY(BlueprintReadOnly, Category = "Type")
	FGameplayAttributeData AtkPoison;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, AtkPoison)
	FOnStatChanged m_OnChangedAtkPoison;
	//
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData AvoidChance;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, AvoidChance)
	FOnStatChanged m_OnChangedAvoidChance;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	FGameplayAttributeData Accuracy;
	ATTRIBUTE_ACCESSORS(UBaseDiabloAttribute, Accuracy)
	FOnStatChanged m_OnChangedAccuracy;
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
