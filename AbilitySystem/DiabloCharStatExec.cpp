#include "DiabloCharStatExec.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"

struct DiabloCharStatStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDamagePer);

	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDefensePer);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxMana);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxStamina);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxRage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealthPer);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamagePer);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePer);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MoveSpeed);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Str);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Dex);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Int);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Vit);

	DiabloCharStatStatics(): PhysicalDamagePerProperty(nullptr), PhysicalDefensePerProperty(nullptr),
	                         MaxHealthPerProperty(nullptr)
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Str, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Dex, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Int, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Vit, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MaxMana, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MaxStamina, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MaxRage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MaxHealth, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MoveSpeed, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, PhysicalDamage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, PhysicalDefense, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, DamagePer, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, DefensePer, Source, true);
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
	RelevantAttributesToCapture.Add(GetCharStatStatics().MoveSpeedDef);
	
	RelevantAttributesToCapture.Add(GetCharStatStatics().PhysicalDamageDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().PhysicalDamagePerDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().PhysicalDefenseDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().PhysicalDefensePerDef);

	RelevantAttributesToCapture.Add(GetCharStatStatics().DamagePerDef);
	RelevantAttributesToCapture.Add(GetCharStatStatics().DefensePerDef);

	RelevantAttributesToCapture.Add(GetCharStatStatics().MaxHealthPerDef);
}

void UDiabloCharStatExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, OUT FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor*  SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	AActor*  TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	const int AttackerLevel = Cast<AUnitPawn>(SourceActor)->GetLevel();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();//�����
	//���ϱ� ���� �κе� �������� �������� ��.
	//
	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	//���� ���� �ʿ���

	//��� ������ ������?�� �� ĳ������ �ֽ��� �̿��� ���� ��� ������ ��°���.

	//��ø���� ������ ������� ��ĳ�� ���������� ����?

	//�ֽ��� �̿��� ���� �ۿ��ϴ°� �������� �ۿ��Ϸ���
	//�� ĳ���Ͱ� �ڽ��� �ֽ��ݸ� ��� ���̽�,�ֽ��ݰ� ü��, �ֽ��ݰ� �ν���, ���� �ν���,�ν��ݰ� ü���� �ʿ���

	float StatStr = 0.f;
	float StatDex = 0.f;
	float StatInt = 0.f;
	float StatVit = 0.f;
	//
	float Health=0.f;
	float HealthPer=0.f;
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
	//ShouldBe Erase, this class only for Status
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().MaxHealthPerDef, EvaluationParameters, HealthPer);

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
	//ü��
	if (AttackerLevel < 35)
	{
		Health = (10 * StatVit) + (AttackerLevel*4 )+ 36;
	}
	else
	{
		Health = (AttackerLevel - 25) * StatVit + (AttackerLevel * 4) + 36;
	}
	Health*=HealthPer;
	
	//output
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().MaxHealthProperty, EGameplayModOp::Additive, Health));
//	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().AttackPowerProperty, EGameplayModOp::Additive, Attack));
//	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().DefensePowerProperty, EGameplayModOp::Additive, Defense));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().MoveSpeedProperty, EGameplayModOp::Additive, MoveSpeed));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().MaxManaProperty, EGameplayModOp::Additive, Mana));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().MaxRageProperty, EGameplayModOp::Additive, Rage));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().MaxStaminaProperty, EGameplayModOp::Additive, Stamina));

}
