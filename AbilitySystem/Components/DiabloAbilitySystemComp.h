// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "AbilitySystemComponent.h"
#include "DiabloAbilitySystemComp.generated.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	// 0 None
	None			,
	// 1 Confirm,
	Confirm			,
	// 2 Cancel,
	Cancel			,
	// 3 LMB,
	BaseAttack		,

	Ability1        ,
	// 4 RMB,
	Ability2		,
	// 5 Q,
	Ability3		,
	// 6 E,
	Ability4		,
	// 7 R,
	Ability5		,
};
class UDiabloAbility;

UCLASS()
class DIABLOM_API UDiabloAbilitySystemComp : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Constructors and overrides
	UDiabloAbilitySystemComp();

	/** Returns a list of currently active ability instances that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UDiabloAbility*>& ActiveAbilities);

	/** Returns the default level used for ability activations, derived from the character */
	int32 GetDefaultAbilityLevel() const;

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static UDiabloAbilitySystemComp* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);


	FActiveGameplayEffectHandle ApplyGameEffect(TSubclassOf<UGameplayEffect> gameEffect);
};
