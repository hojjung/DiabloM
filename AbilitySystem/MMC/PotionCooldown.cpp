#include "PotionCooldown.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"


UPotionCooldown::UPotionCooldown()
{
    m_fPotionDefaultCooldown=10.f;
    m_fPotionMinCooldown=1.2f;
    
    FGameplayEffectAttributeCaptureDefinition HpPotionCDReduceDef;
    HpPotionCDReduceDef.AttributeToCapture = UPlayerDiabloAttribute::GetPotionCoolDownReduceAttribute();
    HpPotionCDReduceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    HpPotionCDReduceDef.bSnapshot = false;
    
    RelevantAttributesToCapture.Reset();
    RelevantAttributesToCapture.Add(HpPotionCDReduceDef);
}
//if (TargetTags->HasTagExact(FGameplayTag::RequestGameplayTag(FName("Status.WeakToPoisonMana"))))

float UPotionCooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    float CdReduce = 0.f;
    GetCapturedAttributeMagnitude(RelevantAttributesToCapture[0], Spec, EvaluationParameters, CdReduce);
    CdReduce = FMath::Clamp<float>(CdReduce, 0.0f,0.95f);

    float PotionCD = m_fPotionDefaultCooldown*(1.f-CdReduce);

    PotionCD= FMath::Max<float>(PotionCD,m_fPotionMinCooldown);
	
    return PotionCD;
}
