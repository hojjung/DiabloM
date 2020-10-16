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
protected:
	void GetTookPhysDmg(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                    const FGameplayEffectSpec& Spec,
	                    FAggregatorEvaluateParameters EvaluationParameters, float& TookPhysDmg) const;
	float CalcPhysReduction(int DefenseTargetLevel,int AttackerLevel, float PhysDefense) const;
	void GetAllDefensePer(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
					      const FGameplayEffectSpec& Spec,
	                      FAggregatorEvaluateParameters EvaluationParameters, float& AllDefensePer) const;
	void GetPhysDefense(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
						const FGameplayEffectSpec& Spec,
	                    FAggregatorEvaluateParameters EvaluationParameters, float& PhysDefense) const;
	void GetTookFireDmg(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
						const FGameplayEffectSpec& Spec,
	                    FAggregatorEvaluateParameters EvaluationParameters, float& TookFireDmg) const;
	void GetFireRes(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
					const FGameplayEffectSpec& Spec,
	                FAggregatorEvaluateParameters EvaluationParameters, float& FireResist) const;
	void GetTookColdDmg(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
						const FGameplayEffectSpec& Spec,
	                    FAggregatorEvaluateParameters EvaluationParameters, float& TookColdDmg) const;
	void GetColdRes(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
					const FGameplayEffectSpec& Spec,
	                FAggregatorEvaluateParameters EvaluationParameters, float& ColdResist) const;
	void GetTookElecDmg(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
						const FGameplayEffectSpec& Spec,
	                    FAggregatorEvaluateParameters EvaluationParameters, float& TookElecDmg) const;
	void GetElecRes(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
					const FGameplayEffectSpec& Spec,
	                FAggregatorEvaluateParameters EvaluationParameters, float& ElecResist) const;
	void GetTookPoisonDmg(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
							const FGameplayEffectSpec& Spec,
	                      FAggregatorEvaluateParameters EvaluationParameters, float& TookPoisonDmg) const;
	void GetPoisonRes(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
						const FGameplayEffectSpec& Spec,
	                  FAggregatorEvaluateParameters EvaluationParameters, float& PoisonResist) const;
	//
	void ApplyPhysDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, int AttackerLevel,
	                     float AllDefensePer,
	                     float PhysDefense, float TookPhysDmg) const;
	void ApplyFireDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float AllDefensePer,
	                     float FireResist,
	                     float TookFireDmg) const;
	void ApplyColdDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float AllDefensePer,
	                     float ColdResist,
	                     float TookColdDmg) const;
	void ApplyPoisonDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float AllDefensePer,
	                       float PoisonResist, float TookPoisonDmg) const;
	void ApplyElecDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float AllDefensePer,
	                     float ElecResist,
	                     float TookElecDmg) const;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
