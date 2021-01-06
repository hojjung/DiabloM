#include "DiabloCharStatExec.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/PlayerDiabloCharacter.h"

//이 소스는 캐릭터의 차스텟과 연관된 로직만 작성되야한다.
//어차피 숫자 올리고 내리고는 게임 이펙트가 다해준다.
//

struct DiabloCharStatStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDefense);

    DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
    DECLARE_ATTRIBUTE_CAPTUREDEF(HealthRegen);

    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackSpeed);
    DECLARE_ATTRIBUTE_CAPTUREDEF(MoveSpeed);

    DECLARE_ATTRIBUTE_CAPTUREDEF(DamagePer);
    DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePer);

    DECLARE_ATTRIBUTE_CAPTUREDEF(ResFire);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ResCold);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ResPoison);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ResElec);

    DECLARE_ATTRIBUTE_CAPTUREDEF(AtkFire);
    DECLARE_ATTRIBUTE_CAPTUREDEF(AtkCold);
    DECLARE_ATTRIBUTE_CAPTUREDEF(AtkElec);
    DECLARE_ATTRIBUTE_CAPTUREDEF(AtkPoison);

    DECLARE_ATTRIBUTE_CAPTUREDEF(Avoid);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Accuracy);
    //
    DECLARE_ATTRIBUTE_CAPTUREDEF(Str);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Dex);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Int);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Vit);

    DECLARE_ATTRIBUTE_CAPTUREDEF(MaxMana);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ManaRegen);

    DECLARE_ATTRIBUTE_CAPTUREDEF(MaxStamina);
    DECLARE_ATTRIBUTE_CAPTUREDEF(StaminaRegen);

    DECLARE_ATTRIBUTE_CAPTUREDEF(MaxRage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(RageRegen);

    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);

    DECLARE_ATTRIBUTE_CAPTUREDEF(PotionCoolDownReduce);
    DECLARE_ATTRIBUTE_CAPTUREDEF(PotionBonusPer);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CoolDownReduce);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CastingSpeed);

    DECLARE_ATTRIBUTE_CAPTUREDEF(AllSkillBonus);

    DECLARE_ATTRIBUTE_CAPTUREDEF(LifeSteal);
   
    DECLARE_ATTRIBUTE_CAPTUREDEF(BashChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);

    DECLARE_ATTRIBUTE_CAPTUREDEF(MagicItemDropBonus);
    DECLARE_ATTRIBUTE_CAPTUREDEF(GoldBonusPer);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ExpBonusPer);


    DiabloCharStatStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, PhysicalDamage, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, PhysicalDefense, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MaxHealth, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, HealthRegen, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, AttackSpeed, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MoveSpeed, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, DamagePer, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, DefensePer, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, ResFire, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, ResCold, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, ResElec, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, ResPoison, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, AtkFire, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, AtkCold, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, AtkElec, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, AtkPoison, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Avoid, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Accuracy, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Str, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Dex, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Int, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, Vit, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MaxMana, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, ManaRegen, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MaxStamina, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, StaminaRegen, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MaxRage, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, RageRegen, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, BlockChance, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, PotionCoolDownReduce, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, PotionBonusPer, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, CoolDownReduce, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, CastingSpeed, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, AllSkillBonus, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, LifeSteal, Source, true);
      
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, CriticalChance, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, MagicItemDropBonus, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, GoldBonusPer, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, ExpBonusPer, Source,  true);
    }
};

static const DiabloCharStatStatics& GetCharStatStatics()
{
    static DiabloCharStatStatics CharStatStatics;
    return CharStatStatics;
}

UDiabloCharStatExec::UDiabloCharStatExec()
{
    RelevantAttributesToCapture.Add(GetCharStatStatics().PhysicalDamageDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().PhysicalDefenseDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().MaxHealthDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().HealthRegenDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().AttackSpeedDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().MoveSpeedDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().DamagePerDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().DefensePerDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().ResFireDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().ResColdDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().ResElecDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().ResPoisonDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().AtkFireDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().AtkColdDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().AtkElecDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().AtkPoisonDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().AvoidDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().AccuracyDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().StrDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().DexDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().IntDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().VitDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().MaxManaDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().ManaRegenDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().MaxStaminaDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().StaminaRegenDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().MaxRageDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().RageRegenDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().BlockChanceDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().PotionCoolDownReduceDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().PotionBonusPerDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().CoolDownReduceDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().CastingSpeedDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().AllSkillBonusDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().LifeStealDef);
  
    RelevantAttributesToCapture.Add(GetCharStatStatics().BashChanceDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().CriticalChanceDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().MagicItemDropBonusDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().GoldBonusPerDef);
    RelevantAttributesToCapture.Add(GetCharStatStatics().ExpBonusPerDef);
}

void UDiabloCharStatExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                 OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
    UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;
    AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
    AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

    const int PlayerLevel = Cast<AUnitPawn>(SourceActor)->GetCharacterLevel();

    //
    float PhysicalDamage=0.f;
    float PhysicalDefense=0.f;
    float MaxHealth=0.f;
    float HealthRegen=0.f;
    float AttackSpeed=1.0f;
    float MoveSpeed=1.0f;
    float DamagePer=1.0f;
    float DefensePer=1.0f;
    float ResFire=1.0f;
    float ResCold=1.0f;
    float ResElec=1.0f;
    float ResPoison=1.0f;
    float AtkFire=1.0f;
    float AtkCold=1.0f;
    float AtkElec=1.0f;
    float AtkPoison=1.0f;
    float AvoidChance=1.0f;
    float Accuracy=1.0f;
    float Str=0.f;
    float Dex=0.f;
    float Int=0.f;
    float Vit=0.f;
    float MaxMana=0.f;
    float ManaRegen=0.f;
    float MaxStamina=0.f;
    float StaminaRegen=0.f;
    float MaxRage=0.f;
    float RageRegen=0.f;
    float BlockChance=1.0f;
    float PotionCoolDownReduce=0.f;
    float PotionBonusPer=1.0f;
    float CoolDownReduce=1.0f;
    float CastingSpeed=1.0f;
    float AllSkillBonus=0.f;
    float LifeSteal=1.0f;
   
    float CriticalChance=1.0f;
    float MagicItemDropBonus=1.0f;
    float GoldBonusPer=1.0f;
    float ExpBonusPer=1.0f;


    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().PhysicalDamageDef,
                                                               EvaluationParameters, PhysicalDamage);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().PhysicalDefenseDef,
                                                               EvaluationParameters, PhysicalDefense);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().MaxHealthDef, EvaluationParameters,
                                                               MaxHealth);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().HealthRegenDef,
                                                               EvaluationParameters, HealthRegen);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().AttackSpeedDef,
                                                               EvaluationParameters, AttackSpeed);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().MoveSpeedDef, EvaluationParameters,
                                                               MoveSpeed);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().DamagePerDef, EvaluationParameters,
                                                               DamagePer);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().DefensePerDef, EvaluationParameters,
                                                               DefensePer);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().ResFireDef, EvaluationParameters,
                                                               ResFire);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().ResColdDef, EvaluationParameters,
                                                               ResCold);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().ResElecDef, EvaluationParameters,
                                                               ResElec);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().ResPoisonDef, EvaluationParameters,
                                                               ResPoison);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().AtkFireDef, EvaluationParameters,
                                                               AtkFire);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().AtkColdDef, EvaluationParameters,
                                                               AtkCold);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().AtkElecDef, EvaluationParameters,
                                                               AtkElec);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().AtkPoisonDef, EvaluationParameters,
                                                               AtkPoison);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().AvoidDef,
                                                               EvaluationParameters, AvoidChance);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().AccuracyDef, EvaluationParameters,
                                                               Accuracy);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().StrDef, EvaluationParameters, Str);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().DexDef, EvaluationParameters, Dex);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().IntDef, EvaluationParameters, Int);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().VitDef, EvaluationParameters, Vit);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().MaxManaDef, EvaluationParameters,
                                                               MaxMana);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().ManaRegenDef, EvaluationParameters,
                                                               ManaRegen);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().MaxStaminaDef, EvaluationParameters,
                                                               MaxStamina);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().StaminaRegenDef,
                                                               EvaluationParameters, StaminaRegen);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().MaxRageDef, EvaluationParameters,
                                                               MaxRage);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().RageRegenDef, EvaluationParameters,
                                                               RageRegen);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().BlockChanceDef,
                                                               EvaluationParameters, BlockChance);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().PotionCoolDownReduceDef,
                                                               EvaluationParameters, PotionCoolDownReduce);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().PotionBonusPerDef,
                                                               EvaluationParameters, PotionBonusPer);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().CoolDownReduceDef,
                                                               EvaluationParameters, CoolDownReduce);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().CastingSpeedDef,
                                                               EvaluationParameters, CastingSpeed);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().AllSkillBonusDef,
                                                               EvaluationParameters, AllSkillBonus);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().LifeStealDef, EvaluationParameters,
                                                               LifeSteal);
 
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().CriticalChanceDef,
                                                               EvaluationParameters, CriticalChance);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().MagicItemDropBonusDef,
                                                               EvaluationParameters, MagicItemDropBonus);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().GoldBonusPerDef,
                                                               EvaluationParameters, GoldBonusPer);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCharStatStatics().ExpBonusPerDef,
                                                               EvaluationParameters, ExpBonusPer);

    //ShouldBe Erase, this class only for Status

    //armor
    //Defense = StatStr
    //
    HealthRegen = (Vit+PlayerLevel)/1.5f;
    
    if (PlayerLevel < 35)
    {
        MaxHealth = (10 * Vit) + (PlayerLevel * 4) + 36;
    }
    else
    {
        MaxHealth = (PlayerLevel - 25) * Vit + (PlayerLevel * 4) + 36;
    }

    //output
    OutExecutionOutput.AddOutputModifier(
        FGameplayModifierEvaluatedData(GetCharStatStatics().MaxHealthProperty, EGameplayModOp::Additive, MaxHealth));
    OutExecutionOutput.AddOutputModifier(
      FGameplayModifierEvaluatedData(GetCharStatStatics().HealthRegenProperty, EGameplayModOp::Additive, HealthRegen));
    
    //	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().AttackPowerProperty, EGameplayModOp::Additive, Attack));
    //	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCharStatStatics().DefensePowerProperty, EGameplayModOp::Additive, Defense));
    // OutExecutionOutput.AddOutputModifier(
    //     FGameplayModifierEvaluatedData(GetCharStatStatics().MoveSpeedProperty, EGameplayModOp::Additive, MoveSpeed));
    // OutExecutionOutput.AddOutputModifier(
    //     FGameplayModifierEvaluatedData(GetCharStatStatics().MaxManaProperty, EGameplayModOp::Additive, MaxMana));
    // OutExecutionOutput.AddOutputModifier(
    //     FGameplayModifierEvaluatedData(GetCharStatStatics().MaxRageProperty, EGameplayModOp::Additive, MaxRage));
    // OutExecutionOutput.AddOutputModifier(
    //     FGameplayModifierEvaluatedData(GetCharStatStatics().MaxStaminaProperty, EGameplayModOp::Additive, MaxStamina));
}
