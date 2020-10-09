// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "PlayerDiabloAbility.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerDiabloAbility : public UDiabloAbility
{
	GENERATED_BODY()

public:
	UPlayerDiabloAbility(){};
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputID m_AbilityInputID = EAbilityInputID::None;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputID m_AbilityID = EAbilityInputID::None;
	
};
