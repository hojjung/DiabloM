// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Timer/DiaTimerAbility.h"
#include "PlayerHpRegenAbility.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerHpRegenAbility : public UDiaTimerAbility
{
	GENERATED_BODY()

	public:
	UPlayerHpRegenAbility();
protected:
	virtual void OnTimer()override;
	
	virtual float GetAttributeWantFrom() override;
	
};
