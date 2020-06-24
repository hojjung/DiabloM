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
	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData Rage;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, Rage)

	UPROPERTY(BlueprintReadOnly, Category = "Energy")
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UPlayerDiabloAttribute, MaxRage)

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

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
public:
	virtual void PrintStats() override;
};
