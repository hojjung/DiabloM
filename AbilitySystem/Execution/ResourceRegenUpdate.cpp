// My First Hack n Slash


#include "ResourceRegenUpdate.h"

#include "AbilitySystemComponent.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/UnitPawn.h"

bool UResourceRegenUpdate::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                  FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	PRINTF("ResourceRegen1");
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;

	if(!SourceActor)
	{
		return false;
	}

	PRINTF("ResourceRegen2");
	
	APlayerDiabloCharacter* Player = Cast<APlayerDiabloCharacter>(SourceActor);

	if(!Player)
	{
		return false;
	}

	PRINTF("ResourceRegen3");

	Player->UpdateRegenAbility();

	return true;
}
