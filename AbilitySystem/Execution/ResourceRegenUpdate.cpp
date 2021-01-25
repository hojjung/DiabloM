// My First Hack n Slash


#include "ResourceRegenUpdate.h"

#include "AbilitySystemComponent.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/UnitPawn.h"

bool UResourceRegenUpdate::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                  FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	APlayerDiabloCharacter* Player = Cast<APlayerDiabloCharacter>(SourceActor);

	if(!Player)
	{
		return false;
	}

	Player->UpdateRegenAbility();

	return true;
}
