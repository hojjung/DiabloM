#include "DiabloCharStatExec.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"

struct DiabloCharStatStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxMana);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxStamina);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxRage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MoveSpeed);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Str);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dex);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Int);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Vit);

	DiabloCharStatStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Str, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Dex, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Int, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Vit, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MaxMana, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MaxStamina, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MaxRage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MaxHealth, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, AttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, DefensePower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MoveSpeed, Source, true);
	}
};

static const DiabloCharStatStatics& GetCharStatStatics()
{
	static DiabloCharStatStatics CharStatStatics;
	return CharStatStatics;
}


UDiabloCharStatExec::UDiabloCharStatExec()
{
	RelevantAttributesToCapture.Add(GetCharStatStatics().StrDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().DexDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().IntDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().VitDef);

	RelevantAttributesToCapture.Add(GetCharStatStatics().MaxManaDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().MaxStaminaDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().MaxRageDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().MaxHealthDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().MoveSpeedDef);
}

void UDiabloCharStatExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, OUT FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor*  SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	AActor*  TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	const int AttackerLevel = Cast<AUnitPawn>(SourceActor)->GetLevel();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	//직업 구분 필요함

	//디아 스텟의 문제점?은 각 캐릭터의 주스텟 이외의 것은 모두 쓰레기 라는것임.

	//민첩에게 공속을 줘버리면 민캐만 유리해지지 않음?

	//주스텟 이외의 것이 작용하는게 장점으로 작용하려면
	//각 캐릭터가 자신의 주스텟만 찍는 케이스,주스텟과 체력, 주스텟과 부스텟, 오직 부스텟,부스텟과 체력이 필요함

	float StatStr = 0.f;
	float StatDex = 0.f;
	float StatInt = 0.f;
	float StatVit = 0.f;
	//
	float Health=0.f;
	float HealthRegen = 0.f;
	float Attack = 0.f;
	float Defense = 0.f;
	float MoveSpeed = 0.f;
	float Mana = 0.f;
	float Rage = 0.f;
	float Stamina = 0.f;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().StrDef, EvaluationParameters, StatStr);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().DexDef, EvaluationParameters, StatDex);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().IntDef, EvaluationParameters, StatInt);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().VitDef, EvaluationParameters, StatVit);

	//armor
	//Defense = StatStr
	//
	if (AttackerLevel < 35)
	{
		HealthRegen = 10 * StatVit;
	}
	else
	{
		HealthRegen = (AttackerLevel-25) * StatVit;
	}
	//체력
	if (AttackerLevel < 35)
	{
		Health = (10 * StatVit) + (AttackerLevel*4 )+ 36;
	}
	else
	{
		Health = (AttackerLevel - 25) * StatVit + (AttackerLevel * 4) + 36;
	}
	//output
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().MaxHealthProperty, EGameplayModOp::Additive, Health));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().AttackPowerProperty, EGameplayModOp::Additive, Attack));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().DefensePowerProperty, EGameplayModOp::Additive, Defense));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().MoveSpeedProperty, EGameplayModOp::Additive, MoveSpeed));
	//resource
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().MaxManaProperty, EGameplayModOp::Additive, Mana));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().MaxRageProperty, EGameplayModOp::Additive, Rage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().MaxStaminaProperty, EGameplayModOp::Additive, Stamina));

}
