// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "PlayerDiabloAbility.generated.h"

class APlayerDiabloCharacter;
/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerDiabloAbility : public UDiabloAbility
{
	GENERATED_BODY()

public:
	UPlayerDiabloAbility(){};
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputID m_AbilityInputID = EAbilityInputID::None;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputID m_AbilityID = EAbilityInputID::None;
protected:
	UPROPERTY()
	APlayerDiabloCharacter* m_PlayerPawn; 
protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
