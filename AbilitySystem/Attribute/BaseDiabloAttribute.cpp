#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "AbilitySystem/DiabloAbilitySystemComp.h"
#include "Characters/UnitPawn.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UBaseDiabloAttribute::UBaseDiabloAttribute()
{

}

void UBaseDiabloAttribute::AdjustAttributeForMaxChange(
	FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, 
	float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}
void UBaseDiabloAttribute::PreAttributeChange(const FGameplayAttribute & Attribute, float & NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
}

void UBaseDiabloAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	// Compute the delta between old and new, if it is available
	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// If this was additive, store the raw delta value to be passed along later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AUnitPawn* TargetCharacter = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AUnitPawn>(TargetActor);
	}
	if (Data.EvaluatedData.Attribute == GetPhysicalDamageAttribute())
	{
		// ������
		AActor* SourceActor = nullptr;
		AUnitPawn* SourceCharacter = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();

			SourceCharacter = Cast<AUnitPawn>(SourceActor);

			// Set the causer actor based on context if it's set
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetPhysicalDamage() * GetDamagePer();

		if (LocalDamageDone > 0)
		{
			// Apply the health change and then clamp it
			const float OldHealth = GetHealth();
			SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

			if (TargetCharacter)
			{
				// This is proper damage
				//TargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);

				// Call for all health changes
				//TargetCharacter->HandleHealthChanged(-LocalDamageDone, SourceTags);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes such as from healing or direct modifiers
		// First clamp it
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (TargetCharacter)
		{
			// Call for all health changes
			//TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
		}
	}
	//else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	//{
	//	// Clamp mana
	//	SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));

	//	if (TargetCharacter)
	//	{
	//		// Call for all mana changes
	//		TargetCharacter->HandleManaChanged(DeltaValue, SourceTags);
	//	}
	//}
	else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		if (TargetCharacter)
		{
			// Call for all movespeed changes
			//TargetCharacter->HandleMoveSpeedChanged(DeltaValue, SourceTags);
			Cast<UUnitMovement>( TargetCharacter->GetMovementComponent())->SetMoveSpeed(GetMoveSpeed());
		}
	}
}

void UBaseDiabloAttribute::PrintStats()
{
	PRINTF("MaxHealthPer: %f",GetMaxHealthPer());
	PRINTF("MaxHealth: %f",GetMaxHealth());
	PRINTF("Health: %f", GetHealth());
	PRINTF("AttackPower: %f", GetPhysicalDamage());
	PRINTF("AttackPowerPer: %f", GetDamagePer());
	PRINTF("DefensePower: %f", GetPhysicalDefense());
	PRINTF("DefensePowerPer: %f", GetDefensePer());
	PRINTF("MoveSpeed: %f", GetMoveSpeed());
}
