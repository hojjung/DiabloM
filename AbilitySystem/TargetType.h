// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/AbilityTypes.h"
#include "TargetType.generated.h"

/**
 * 
 */

class AUnitPawn;
class AActor;
struct FGameplayEventData;

/**
 * Class that is used to determine targeting for abilities
 * It is meant to be blueprinted to run target logic
 * This does not subclass GameplayAbilityTargetActor because this class is never instanced into the world
 * This can be used as a basis for a game-specific targeting blueprint
 * If your targeting is more complicated you may need to instance into the world once or as a pooled actor
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class DIABLOM_API UTargetType : public UObject
{
	GENERATED_BODY()

public:
	UTargetType() {}

	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(AUnitPawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};


/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class DIABLOM_API UTargetType_UseSelf : public UTargetType
{
	GENERATED_BODY()

public:
	UTargetType_UseSelf() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(AUnitPawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class DIABLOM_API UTargetType_UseEventData : public UTargetType
{
	GENERATED_BODY()

public:
	UTargetType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(AUnitPawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};
