#include "AbilitySystem/Execution/DiabloDamageExec.h"
#include "AbilitySystem/Components/DiabloAbilitySystemComp.h"
#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "Characters/UnitPawn.h"


struct DiabloDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TookDamage);

	DiabloDamageStatics()
	{
		//이제 이해했다
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, PhysicalDefense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, TookDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, PhysicalDamage, Source, false);
	}
};

static const DiabloDamageStatics& GetDamageStatics()
{
	static DiabloDamageStatics DmgStatics;
	return DmgStatics;
}

UDiabloDamageExec::UDiabloDamageExec()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalDefenseDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalDamageDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().TookDamageDef);
}

void UDiabloDamageExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, OUT FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	const int AttackerLevel = Cast<AUnitPawn>(SourceActor)->GetCharacterLevel();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;


	float AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().PhysicalDamageDef, EvaluationParameters, AttackPower);

	float Damage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().TookDamageDef, EvaluationParameters, Damage);


	AttackPower*=Damage;

	
	float DefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().PhysicalDefenseDef, EvaluationParameters, DefensePower);

	float Reduction = DefensePower != 0.0f ? (DefensePower) / (50.f * AttackerLevel + DefensePower) : 1.f;

	float DamageDone = AttackPower * Reduction;
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Combat.TookDamage")), false, -1.0f), 0.0f);

	if (DamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageStatics().TookDamageProperty, EGameplayModOp::Additive, DamageDone));
	}

	// // Broadcast damages to Target ASC
	// UGDAbilitySystemComponent* TargetASC = Cast<UGDAbilitySystemComponent>(TargetAbilitySystemComponent);
	// if (TargetASC)
	// {
	// 	UGDAbilitySystemComponent* SourceASC = Cast<UGDAbilitySystemComponent>(SourceAbilitySystemComponent);
	// 	TargetASC->ReceiveDamage(SourceASC, UnmitigatedDamage, MitigatedDamage);
	// }
}
