
#include "AbilitySystem/DiabloAbility.h"
#include "AbilitySystem/TargetType.h"
#include "AbilitySystem/DiabloAbilitySystemComp.h"
#include "Characters/UnitPawn.h"

UDiabloAbility::UDiabloAbility() {}


FDiaGameplayEffectContainerSpec UDiabloAbility::MakeEffectContainerSpecFromContainer(const FDiaGameplayEffectContainer & Container, const FGameplayEventData & EventData, int32 OverrideGameplayLevel)
{
	// First figure out our actor info
	FDiaGameplayEffectContainerSpec ReturnSpec;
	AActor* OwningActor = GetOwningActorFromActorInfo();
	AUnitPawn* OwningCharacter = Cast<AUnitPawn>(OwningActor);
	UDiabloAbilitySystemComp* OwningASC = UDiabloAbilitySystemComp::GetAbilitySystemComponentFromActor(OwningActor);

	if (OwningASC)
	{
		// If we have a target type, run the targeting logic. This is optional, targets can be added later
		if (Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			const UTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
			ReturnSpec.AddTargets(HitResults, TargetActors);
		}

		// If we don't have an override level, use the default on the ability itself
		if (OverrideGameplayLevel == INDEX_NONE)
		{
			OverrideGameplayLevel = OverrideGameplayLevel = this->GetAbilityLevel(); //OwningASC->GetDefaultAbilityLevel();
		}

		// Build GameplayEffectSpecs for each applied effect
		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{
			ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
		}
	}
	return ReturnSpec;
}

FDiaGameplayEffectContainerSpec UDiabloAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData & EventData, int32 OverrideGameplayLevel)
{
	FDiaGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

	if (FoundContainer)
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FDiaGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> UDiabloAbility::ApplyEffectContainerSpec(const FDiaGameplayEffectContainerSpec & ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

TArray<FActiveGameplayEffectHandle> UDiabloAbility::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData & EventData, int32 OverrideGameplayLevel)
{
	FDiaGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	return ApplyEffectContainerSpec(Spec);
}
