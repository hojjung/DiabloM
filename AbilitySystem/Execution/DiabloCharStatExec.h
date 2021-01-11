// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DiabloCharStatExec.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UDiabloCharStatExec : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UDiabloCharStatExec();

	virtual bool Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	
};
