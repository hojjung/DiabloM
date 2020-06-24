#include "AbilitySystem/DiabloDamageExec.h"
#include "AbilitySystem/DiabloAbilitySystemComp.h"
#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "Characters/UnitPawn.h"


struct DiabloDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	DiabloDamageStatics()
	{
		// Capture the Target's DefensePower attribute. Do not snapshot it, because we want to use the health value at the moment we apply the execution.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, DefensePower, Target, false);

		// Capture the Source's AttackPower. We do want to snapshot this at the moment we create the GameplayEffectSpec that will execute the damage.
		// (imagine we fire a projectile: we create the GE Spec when the projectile is fired. When it hits the target, we want to use the AttackPower at the moment
		// the projectile was launched, not when it hits).
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, AttackPower, Source, true);

		// Also capture the source's raw Damage, which is normally passed in directly via the execution
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, Damage, Source, true);
	}
};

static const DiabloDamageStatics& GetDamageStatics()
{
	static DiabloDamageStatics DmgStatics;
	return DmgStatics;
}

UDiabloDamageExec::UDiabloDamageExec()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().DamageDef);
}

void UDiabloDamageExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, OUT FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	const int AttackerLevel = Cast<AUnitPawn>(SourceActor)->GetLevel();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;


	float AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().AttackPowerDef, EvaluationParameters, AttackPower);

	float Damage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().DamageDef, EvaluationParameters, Damage);


	float DefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().DefensePowerDef, EvaluationParameters, DefensePower);

	float Reduction = DefensePower != 0.0f ? (DefensePower) / (50.f * AttackerLevel + DefensePower) : 1.f;

	float DamageDone = Damage * Reduction;
	if (DamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageStatics().DamageProperty, EGameplayModOp::Additive, DamageDone));
	}
}
