// My First Hack n Slash


#include "MoveSpeedUpdate.h"

#include "Characters/PlayerDiabloCharacter.h"

bool UMoveSpeedUpdate::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	APlayerDiabloCharacter* Player = Cast<APlayerDiabloCharacter>(SourceActor);

	if(!Player)
	{
		return false;
	}

	Player->UpdateMoveSpeed();

	return true;
}
