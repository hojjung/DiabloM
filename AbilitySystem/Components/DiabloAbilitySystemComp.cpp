#include "DiabloAbilitySystemComp.h"
#include "Characters/UnitPawn.h"
#include "AbilitySystem/Ability/DiabloAbility.h"
#include "AbilitySystemGlobals.h"

UDiabloAbilitySystemComp::UDiabloAbilitySystemComp()
{
	bCachedIsNetSimulated=true;
}

void UDiabloAbilitySystemComp::GetActiveAbilitiesWithTags(const FGameplayTagContainer & GameplayTagContainer, TArray<UDiabloAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);
	
	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UDiabloAbility>(ActiveAbility));
		}
	}
}

int32 UDiabloAbilitySystemComp::GetDefaultAbilityLevel() const
{
	AUnitPawn* OwningCharacter = Cast<AUnitPawn>(OwnerActor);

	if (OwningCharacter)
	{
		return OwningCharacter->GetCharacterLevel();
	}
	return 1;
}

UDiabloAbilitySystemComp * UDiabloAbilitySystemComp::GetAbilitySystemComponentFromActor(const AActor * Actor, bool LookForComponent)
{
	return Cast<UDiabloAbilitySystemComp>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}

FActiveGameplayEffectHandle UDiabloAbilitySystemComp::ApplyGameEffect(TSubclassOf<UGameplayEffect> gameEffect)
{
	FGameplayEffectContextHandle EffectContext = MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = MakeOutgoingSpec(gameEffect,GetDefaultAbilityLevel(), EffectContext);

	if (!NewHandle.IsValid())
	{
		PRINTF("Invalid Handle");
	}

	return ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), this);
}
