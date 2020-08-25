// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Animation/AnimInstance.h"
#include "DiaAniminstance.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiaAniminstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	float GetVelocitySqr() const;
	UFUNCTION(BlueprintCallable)
	float GetVeloPercentOne() const;
};
