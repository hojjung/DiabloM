#pragma once

#include "DiabloM.h"
#include "GameplayModMagnitudeCalculation.h"
#include "PotionCooldown.generated.h"

UCLASS()
class DIABLOM_API UPotionCooldown : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UPotionCooldown();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float m_fPotionDefaultCooldown;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float m_fPotionMinCooldown;
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec & Spec) const override;
	
};
