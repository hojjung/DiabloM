// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/Timer/DiaTimerAbility.h"
#include "PlayerRageRegenAbility.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API UPlayerRageRegenAbility : public UDiaTimerAbility
{
	GENERATED_BODY()
	public:
	UPlayerRageRegenAbility();
	protected:
	virtual float GetAttributeWantFrom() override;
};
