// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Timer/DiaTimerAbility.h"
#include "PlayerManaRegenAbility.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerManaRegenAbility : public UDiaTimerAbility
{
	GENERATED_BODY()
    public:
    UPlayerManaRegenAbility();
protected:
    virtual float GetAttributeWantFrom() override;
};
