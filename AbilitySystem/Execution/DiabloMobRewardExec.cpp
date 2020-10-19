#include "DiabloMobRewardExec.h"
#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/UnitPawn.h"

struct DiabloMobRewardStatics
{
public:
	DECLARE_ATTRIBUTE_CAPTUREDEF(XPBounty);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ExpBonusPer);
	//스킬들은 데미지를 매그니튜드로 보내ㄱ[
	DiabloMobRewardStatics()
	{
		//이제 이해했다
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, XPBounty, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, ExpBonusPer, Target, false);
		//이이펙트를 받는애가 타겟 
	}
};

static const DiabloMobRewardStatics& GetRewardStatic()
{
	static DiabloMobRewardStatics RewardStatic;
	
	return RewardStatic;
}


UDiabloMobRewardExec::UDiabloMobRewardExec()
{
	RelevantAttributesToCapture.Add(GetRewardStatic().XPBountyDef);
	RelevantAttributesToCapture.Add(GetRewardStatic().ExpBonusPerDef);
}

void UDiabloMobRewardExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                  FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	const int AttackerLevel = Cast<AUnitPawn>(SourceActor)->GetCharacterLevel();
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float XpBounty;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetRewardStatic().XPBountyDef, EvaluationParameters, XpBounty);

	float XpBonus;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetRewardStatic().ExpBonusPerDef, EvaluationParameters, XpBonus);
	
	float XpResult=XpBonus*XpBounty;

	ADiabloPlayerController::Get->GetPlayerPawn()->EarnExp(XpResult);

	PRINTF("XpBounty:%f",XpBounty);
	PRINTF("XpBonus:%f",XpBonus);
	PRINTF("XpResult:%f",XpResult);
}
