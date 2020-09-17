// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDiabloAttribute.h"

#include "GameplayEffectExtension.h"

UPlayerDiabloAttribute::UPlayerDiabloAttribute()
{

}


void UPlayerDiabloAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (NewValue <= 0.f)
	{
		return;
	}

	if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetMaxStaminaAttribute());
	}

	//rage start from 0
}
void UPlayerDiabloAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);
	
}

void UPlayerDiabloAttribute::PrintStats()
{
	Super::PrintStats();
	if (GetMaxMana()!=0.f)
	{
		PRINTF("MaxMana: %f", GetMaxMana());
		PRINTF("Mana: %f", GetMana());
	}
	if (GetMaxStamina() != 0.f)
	{
		PRINTF("MaxStamina: %f", GetMaxStamina());
		PRINTF("Stamina: %f", GetStamina());
	}

	if (GetMaxRage() != 0.f)
	{
		PRINTF("MaxRage: %f", GetMaxRage());
		PRINTF("Rage: %f", GetRage());
	}

	PRINTF("STR: %f", GetStr());
	PRINTF("DEX: %f", GetDex());
	PRINTF("INT: %f", GetInt());
	PRINTF("VIT: %f", GetVit());
}
