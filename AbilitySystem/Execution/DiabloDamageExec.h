// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DiabloDamageExec.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiabloDamageExec : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UDiabloDamageExec();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
