#include "AbilitySystem/Execution/DiabloDamageExec.h"
#include "AbilitySystem/Components/DiabloAbilitySystemComp.h"
#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "Characters/UnitPawn.h"


struct DiabloDamageStatics
{
public:
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePer);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TookPhysDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResFire);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TookFireDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResPoison);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TookPoisonDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResElec);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TookElecDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResCold);
	DECLARE_ATTRIBUTE_CAPTUREDEF(TookIceDamage);
	

	DiabloDamageStatics()
	{
		//이제 이해했다
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, DefensePer, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, PhysicalDefense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, TookPhysDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, ResFire, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, TookFireDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, ResPoison, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, TookPoisonDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, ResElec, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, TookElecDamage, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, ResCold, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, TookIceDamage, Target, false);
	}

	FGameplayTag TagTookPhysDamage= FGameplayTag::RequestGameplayTag(FName("Data.Combat.TookPhysDmg"));
	FGameplayTag TagTookColdDamage= FGameplayTag::RequestGameplayTag(FName("Data.Combat.TookColdDmg"));
	FGameplayTag TagTookFireDamage= FGameplayTag::RequestGameplayTag(FName("Data.Combat.TookFireDmg"));
	FGameplayTag TagTookPoisonDamage= FGameplayTag::RequestGameplayTag(FName("Data.Combat.TookPoisonDmg"));
	FGameplayTag TagTookElecDamage= FGameplayTag::RequestGameplayTag(FName("Data.Combat.TookElecDmg"));
};

static const DiabloDamageStatics& GetDamageStatics()
{
	static DiabloDamageStatics DmgStatics;
	
	return DmgStatics;
}

UDiabloDamageExec::UDiabloDamageExec()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().DefensePerDef);
	
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalDefenseDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().TookPhysDamageDef);

	RelevantAttributesToCapture.Add(GetDamageStatics().ResFireDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().TookFireDamageDef);

	RelevantAttributesToCapture.Add(GetDamageStatics().ResPoisonDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().TookPoisonDamageDef);

	RelevantAttributesToCapture.Add(GetDamageStatics().ResElecDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().TookElecDamageDef);

	RelevantAttributesToCapture.Add(GetDamageStatics().ResColdDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().TookIceDamageDef);
}

void UDiabloDamageExec::GetTookPhysDmg(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters, float& TookPhysDmg) const
{
	TookPhysDmg = FMath::Max<float>(Spec.GetSetByCallerMagnitude(GetDamageStatics().TagTookPhysDamage, false, -1.0f), 0.0f);
}

float UDiabloDamageExec::CalcPhysReduction(int DefenseTargetLevel,const int AttackerLevel, float PhysDefense) const
{
	if(PhysDefense != 0.0f)
	{
		int LevelDiff= DefenseTargetLevel-AttackerLevel;
		
		if(LevelDiff<=0)
		{
			LevelDiff=1;
		}

		float DamageReduceConstant= (50.f/LevelDiff);//작아질수록 뎀지 감소율이 높아짐
		
		float ReduceRatio= PhysDefense / ((DamageReduceConstant*AttackerLevel) + PhysDefense);
		
		return 1.f - ReduceRatio;//0.7 3//0.9 // 0.1
	}
	else
	{
		return 1.f;
	}
}

void UDiabloDamageExec::GetAllDefensePer(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec,FAggregatorEvaluateParameters EvaluationParameters, float& AllDefensePer) const
{
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().DefensePerDef, EvaluationParameters, AllDefensePer);
}

void UDiabloDamageExec::GetPhysDefense(const FGameplayEffectCustomExecutionParameters& ExecutionParams,const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters, float& PhysDefense) const
{
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().PhysicalDefenseDef, EvaluationParameters, PhysDefense);
}

void UDiabloDamageExec::GetTookFireDmg(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec,FAggregatorEvaluateParameters EvaluationParameters, float& TookFireDmg) const
{
	TookFireDmg = FMath::Max<float>(Spec.GetSetByCallerMagnitude(GetDamageStatics().TagTookFireDamage, false, -1.0f), 0.0f);
}

void UDiabloDamageExec::GetFireRes(const FGameplayEffectCustomExecutionParameters& ExecutionParams,const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters, float& FireResist) const
{
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ResFireDef, EvaluationParameters, FireResist);
}

void UDiabloDamageExec::GetTookColdDmg(const FGameplayEffectCustomExecutionParameters& ExecutionParams,const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters, float& TookColdDmg) const
{
	TookColdDmg = FMath::Max<float>(Spec.GetSetByCallerMagnitude(GetDamageStatics().TagTookColdDamage, false, -1.0f), 0.0f);
}

void UDiabloDamageExec::GetColdRes(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec,FAggregatorEvaluateParameters EvaluationParameters, float& ColdResist) const
{
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ResColdDef, EvaluationParameters, ColdResist);
}

void UDiabloDamageExec::GetTookElecDmg(const FGameplayEffectCustomExecutionParameters& ExecutionParams,const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters, float& TookElecDmg) const
{
	TookElecDmg = FMath::Max<float>(Spec.GetSetByCallerMagnitude(GetDamageStatics().TagTookElecDamage, false, -1.0f), 0.0f);
}

void UDiabloDamageExec::GetElecRes(const FGameplayEffectCustomExecutionParameters& ExecutionParams,const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters, float& ElecResist) const
{
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ResElecDef, EvaluationParameters, ElecResist);
}

void UDiabloDamageExec::GetTookPoisonDmg(const FGameplayEffectCustomExecutionParameters& ExecutionParams,const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters, float& TookPoisonDmg) const
{
	TookPoisonDmg = FMath::Max<float>(Spec.GetSetByCallerMagnitude(GetDamageStatics().TagTookPoisonDamage, false, -1.0f), 0.0f);
}

void UDiabloDamageExec::GetPoisonRes(const FGameplayEffectCustomExecutionParameters& ExecutionParams,const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters, float& PoisonResist) const
{
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ResPoisonDef, EvaluationParameters, PoisonResist);
}

void UDiabloDamageExec::ApplyPhysDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const int AttackerLevel, float AllDefensePer, float PhysDefense, float TookPhysDmg) const
{
	float PhysReduction = CalcPhysReduction(22,24, PhysDefense);
	TookPhysDmg = (TookPhysDmg*(1.f-AllDefensePer)) * PhysReduction;

	PRINTF("Reduction:%f",PhysReduction);
	if (TookPhysDmg > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageStatics().TookPhysDamageProperty, EGameplayModOp::Additive, TookPhysDmg));
	}
}

void UDiabloDamageExec::ApplyFireDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float AllDefensePer, float FireResist, float TookFireDmg) const
{
	TookFireDmg = (TookFireDmg*(1.f - AllDefensePer))*(1.f-FireResist);

	if (TookFireDmg > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageStatics().TookFireDamageProperty, EGameplayModOp::Additive, TookFireDmg));
	}
}

void UDiabloDamageExec::ApplyColdDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float AllDefensePer, float ColdResist, float TookColdDmg) const
{
	TookColdDmg = (TookColdDmg*(1.f-AllDefensePer))*(1.f-ColdResist);

	if (TookColdDmg > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageStatics().TookIceDamageProperty, EGameplayModOp::Additive, TookColdDmg));
	}
}

void UDiabloDamageExec::ApplyPoisonDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float AllDefensePer, float PoisonResist, float TookPoisonDmg) const
{
	TookPoisonDmg = (TookPoisonDmg*(1.f-AllDefensePer))/(1.f-PoisonResist);

	if (TookPoisonDmg > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageStatics().TookPoisonDamageProperty, EGameplayModOp::Additive, TookPoisonDmg));
	}
}

void UDiabloDamageExec::ApplyElecDamage(FGameplayEffectCustomExecutionOutput& OutExecutionOutput, float AllDefensePer, float ElecResist, float TookElecDmg) const
{
	TookElecDmg = (TookElecDmg*(1.f-AllDefensePer))/(1.f-ElecResist);

	if (TookElecDmg > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetDamageStatics().TookElecDamageProperty, EGameplayModOp::Additive, TookElecDmg));
	}
}

void UDiabloDamageExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, OUT FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const
{
	//어트리뷰트에서 하자
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	const int AttackerLevel = Cast<AUnitPawn>(SourceActor)->GetCharacterLevel();
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	//////////////////////////////////////////////////////

	float AllDefensePer=0.f;//30% 이런식일텐데],1.3 이렇게 놓고 나누자
	float PhysDefense = 0.f;
	float TookPhysDmg = 0.f;
	float FireResist = 0.f;
	float TookFireDmg = 0.f;
	float ColdResist = 0.f;
	float TookColdDmg = 0.f;
	float ElecResist = 0.f;
	float TookElecDmg = 0.f;
	float PoisonResist = 0.f;
	float TookPoisonDmg = 0.f;
	
	GetAllDefensePer(ExecutionParams, Spec,EvaluationParameters, AllDefensePer);
	
	GetTookPhysDmg(ExecutionParams, Spec, EvaluationParameters, TookPhysDmg);
	GetPhysDefense(ExecutionParams, Spec,EvaluationParameters, PhysDefense);

	GetTookFireDmg(ExecutionParams, Spec,EvaluationParameters, TookFireDmg);
	GetFireRes(ExecutionParams,Spec, EvaluationParameters, FireResist);

	GetTookColdDmg(ExecutionParams,Spec, EvaluationParameters, TookColdDmg);
	GetColdRes(ExecutionParams,Spec, EvaluationParameters, ColdResist);

	GetTookElecDmg(ExecutionParams,Spec, EvaluationParameters, TookElecDmg);
	GetElecRes(ExecutionParams,Spec, EvaluationParameters, ElecResist);

	GetTookPoisonDmg(ExecutionParams, Spec,EvaluationParameters, TookPoisonDmg);
	GetPoisonRes(ExecutionParams, Spec,EvaluationParameters, PoisonResist);

	ApplyPhysDamage(OutExecutionOutput, AttackerLevel, AllDefensePer, PhysDefense, TookPhysDmg);
	ApplyFireDamage(OutExecutionOutput, AllDefensePer, FireResist, TookFireDmg);
	ApplyColdDamage(OutExecutionOutput, AllDefensePer, ColdResist, TookColdDmg);
	ApplyPoisonDamage(OutExecutionOutput, AllDefensePer, PoisonResist, TookPoisonDmg);
	ApplyElecDamage(OutExecutionOutput, AllDefensePer, ElecResist, TookElecDmg);
}
