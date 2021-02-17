// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Characters/UnitPawn.h"

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
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> m_GELifeSteal;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> m_GEDoubleAttack;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> m_GEBasherStun;
protected:
	float CalcPhysReduction(int DefenseTargetLevel,int AttackerLevel, float PhysDefense) const;

	bool CheckOnerPercentRand(float chanceMaxOne) const;

	float CalcuSameLevelAvgAccuracy(float targetAvoidChance, AUnitPawn* instigator) const; //맞을 확률

	virtual bool Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};


