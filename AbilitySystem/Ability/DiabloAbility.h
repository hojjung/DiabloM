// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/Components/DiabloAbilitySystemComp.h"

#include "DiabloAbility.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiabloAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UDiabloAbility();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool ActivateAbilityOnGranted = false;
	
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};