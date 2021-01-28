// My First Hack n Slash


#include "MoveSpeedUpdate.h"

#include "Characters/PlayerDiabloCharacter.h"

bool UMoveSpeedUpdate::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	PRINTF("MoveSpeedUpdate1");
	if(!TargetActor)
	{
		return false;
	}

	PRINTF("MoveSpeedUpdate2");
	
	AUnitPawn* Unit = Cast<AUnitPawn>(TargetActor);

	if(!Unit)
	{
		return false;
	}
	PRINTF("MoveSpeedUpdate3");
	Unit->UpdateMoveSpeed();

	return true;
}
