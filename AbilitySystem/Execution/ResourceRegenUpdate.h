// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ResourceRegenUpdate.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UResourceRegenUpdate : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	virtual bool Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	
	
};
