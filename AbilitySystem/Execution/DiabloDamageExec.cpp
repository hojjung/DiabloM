#include "AbilitySystem/Execution/DiabloDamageExec.h"
#include "AbilitySystem/Components/DiabloAbilitySystemComp.h"
#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/UnitPawn.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"


struct DiabloDamageStatics
{
public:
    DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePer);
    DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDefense);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ResFire);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ResPoison);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ResElec);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ResCold);
    //
    DECLARE_ATTRIBUTE_CAPTUREDEF(TookPhysDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(TookFireDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(TookElecDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(TookIceDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(TookPoisonDamage);
    //7
    DECLARE_ATTRIBUTE_CAPTUREDEF(Avoid);
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(DmgReduction);
    //3
    DECLARE_ATTRIBUTE_CAPTUREDEF(DamagePer);
    //DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalDamage);
    //DECLARE_ATTRIBUTE_CAPTUREDEF(AtkFire);
    //DECLARE_ATTRIBUTE_CAPTUREDEF(AtkCold);
    //DECLARE_ATTRIBUTE_CAPTUREDEF(AtkElec);
    //DECLARE_ATTRIBUTE_CAPTUREDEF(AtkPoison);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Accuracy);
    //7
    DECLARE_ATTRIBUTE_CAPTUREDEF(LifeSteal);
    DECLARE_ATTRIBUTE_CAPTUREDEF(DoubleAttackChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(BashChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalDamage);
    //5

    //스킬들은 데미지를 매그니튜드로 보내ㄱ[
    DiabloDamageStatics()
    {
        //이제 이해했다
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, DefensePer, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, PhysicalDefense, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, ResFire, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, ResPoison, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, ResElec, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, ResCold, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, TookPhysDamage, Target, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, TookFireDamage, Target, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, TookElecDamage, Target, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, TookIceDamage, Target, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, TookPoisonDamage, Target, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, Avoid, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, BlockChance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, DmgReduction, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, DamagePer, Source, true);
        //DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, 	PhysicalDamage	    		, Source, true);
        //DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, 	AtkFire			    		, Source, true);
        //DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, 	AtkCold			    		, Source, true);
        //DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, 	AtkElec			    		, Source, true);
        //DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, 	AtkPoison		    		, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseDiabloAttribute, Accuracy, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, LifeSteal, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, DoubleAttackChance, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, BashChance, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, CriticalChance, Source, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerDiabloAttribute, CriticalDamage, Source, true);
        //5
    }

    FGameplayTag TagTookPhysDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookPhysDmg"));
    FGameplayTag TagTookFireDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookFireDmg"));
    FGameplayTag TagTookElecDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookElecDmg"));
    FGameplayTag TagTookIceDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookIceDmg"));
    FGameplayTag TagTookPoisonDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookPoisonDmg"));
    FGameplayTag TagLifeStealHeal = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.HpHeal"));

    //이방식의 문제점은 무엇인가
    //스킬의 상수가 문제다.
    //속성공격의 상수 수치 어떻게 전달시킴?
    //결국 옛날방식이랑 같잔아 본질적으로
    //다시 속성데미지 존재하게?
    //대신 데미지 보너슨 여기서 계산하자
    //회피와 블록킹도 여기서 가능해보임
    //받은데미지 0 만들어주면 되잔아
    //어트리뷰트 생김새도 현행유지
    //초기화도 여기서

    //어트리뷰트는 브로드캐스팅과 클램핑만 하면됨
    //브로드 캐스팅도 사실상 끝났다.
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
    RelevantAttributesToCapture.Add(GetDamageStatics().ResFireDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().ResPoisonDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().ResElecDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().ResColdDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().TookPhysDamageDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().TookFireDamageDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().TookIceDamageDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().TookElecDamageDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().TookPoisonDamageDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().AvoidDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().BlockChanceDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().DmgReductionDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().DamagePerDef);
    //RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalDamageDef);
    //RelevantAttributesToCapture.Add(GetDamageStatics().AtkFireDef);
    //RelevantAttributesToCapture.Add(GetDamageStatics().AtkColdDef);
    //RelevantAttributesToCapture.Add(GetDamageStatics().AtkElecDef);
    //RelevantAttributesToCapture.Add(GetDamageStatics().AtkPoisonDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().AccuracyDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().LifeStealDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().DoubleAttackChanceDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().BashChanceDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().CriticalChanceDef);
    RelevantAttributesToCapture.Add(GetDamageStatics().CriticalDamageDef);
    //
    
}

void UDiabloDamageExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                               OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
    UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
    AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
    AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;
    AUnitPawn* SourceUnit = Cast<AUnitPawn>(SourceActor);
    AUnitPawn* TargetUnit = Cast<AUnitPawn>(TargetActor);
    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    if(SourceUnit==nullptr ||TargetUnit==nullptr)
    {
        return;
    }

    const int AttackerLevel = SourceUnit->GetCharacterLevel();
    const int DefenseTargetLevel = TargetUnit->GetCharacterLevel();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float LAvoid = 0.f;
    float LBlockChance = 0.f;
    float LAccuracy = 0.f;
    //
    float LTookPhysDamage = 0.f;
    float LTookFireDamage = 0.f;
    float LTookIceDamage = 0.f;
    float LTookElecDamage = 0.f;
    float LTookPoisonDamage = 0.f;
    //
    float LDefensePer = 0.f;
    float LPhysicalDefense = 0.f;
    float LResFire = 0.f;
    float LResPoison = 0.f;
    float LResElec = 0.f;
    float LResCold = 0.f;
    //
    float LBashChance = 0.f;
    float LCriticalChance = 0.f;
    float LCriticalDamage = 0.f;
    float LDmgReduction = 0.f;
    float LDamagePer = 0.f;
    float LLifeSteal = 0.f;
    float LDoubleAttackChance = 0.f;
    //회피와 블락킹 먼저,팝업으로 해당 상태들에대해 다 만들어줘야함
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().AvoidDef, EvaluationParameters,
                                                               LAvoid);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().AccuracyDef, EvaluationParameters,
                                                               LAccuracy);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BlockChanceDef, EvaluationParameters,
                                                               LBlockChance);
    //
    if (!CanHitBaseAttack(LAvoid, LAccuracy, AttackerLevel))
    {
        PRINTF("Avoid!");
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(GetDamageStatics().TookPhysDamageProperty, EGameplayModOp::Override, 0.f));
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(GetDamageStatics().TookFireDamageProperty, EGameplayModOp::Override, 0.f));
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(GetDamageStatics().TookIceDamageProperty, EGameplayModOp::Override, 0.f));
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(GetDamageStatics().TookElecDamageProperty, EGameplayModOp::Override, 0.f));
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(GetDamageStatics().TookPoisonDamageProperty, EGameplayModOp::Override, 0.f));

        if (Cast<APlayerDiabloCharacter>(SourceUnit))
        {
            float SuccessPer100 = CalcuSameLevelAvgAccuracy(LAvoid, SourceUnit);
            ADiabloPlayerController::Get->ShowDamageNumber(100.f - SuccessPer100, TargetUnit, EDamagePopup::Miss);
        }
        return;
    }
    //
    if (CheckOnerPercentRand(LBlockChance))
    {
        PRINTF("Blocked!");
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(GetDamageStatics().TookPhysDamageProperty, EGameplayModOp::Override, 0.f));
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(GetDamageStatics().TookFireDamageProperty, EGameplayModOp::Override, 0.f));
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(GetDamageStatics().TookIceDamageProperty, EGameplayModOp::Override, 0.f));
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(GetDamageStatics().TookElecDamageProperty, EGameplayModOp::Override, 0.f));
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(GetDamageStatics().TookPoisonDamageProperty, EGameplayModOp::Override, 0.f));

        return;
    }
    //
    LTookPhysDamage   = Spec.GetSetByCallerMagnitude(GetDamageStatics().TagTookPhysDamage, false, 0.0f);
    LTookFireDamage   = Spec.GetSetByCallerMagnitude(GetDamageStatics().TagTookFireDamage, false, 0.0f);
    LTookIceDamage    = Spec.GetSetByCallerMagnitude(GetDamageStatics().TagTookIceDamage, false, 0.0f);
    LTookElecDamage   = Spec.GetSetByCallerMagnitude(GetDamageStatics().TagTookElecDamage, false, 0.0f);
    LTookPoisonDamage = Spec.GetSetByCallerMagnitude(GetDamageStatics().TagTookPoisonDamage, false, 0.0f);
    //
    float LTookPhysDamageCache   =LTookPhysDamage;  
    float LTookFireDamageCache   =LTookFireDamage;  
    float LTookIceDamageCache    =LTookIceDamage;   
    float LTookElecDamageCache   =LTookElecDamage;  
    float LTookPoisonDamageCache =LTookPoisonDamage;
    //
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().DefensePerDef, EvaluationParameters,
                                                               LDefensePer);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().PhysicalDefenseDef,
                                                               EvaluationParameters, LPhysicalDefense);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ResFireDef, EvaluationParameters,
                                                               LResFire);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ResPoisonDef, EvaluationParameters,
                                                               LResPoison);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ResElecDef, EvaluationParameters,
                                                               LResElec);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ResColdDef, EvaluationParameters,
                                                               LResCold);
    //
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalChanceDef,
                                                               EvaluationParameters, LCriticalChance);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalDamageDef,
                                                               EvaluationParameters, LCriticalDamage);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BashChanceDef, EvaluationParameters,
                                                               LBashChance);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().DmgReductionDef, EvaluationParameters,
                                                               LDmgReduction);
    //
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().DamagePerDef, EvaluationParameters,
                                                               LDamagePer);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().LifeStealDef, EvaluationParameters,
                                                               LLifeSteal);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().DoubleAttackChanceDef,
                                                               EvaluationParameters, LDoubleAttackChance);
    //
    LTookPhysDamage *= 1.f + LDamagePer;
    LTookFireDamage *= 1.f + LDamagePer;
    LTookElecDamage *= 1.f + LDamagePer;
    LTookIceDamage *= 1.f + LDamagePer;
    LTookPoisonDamage *= 1.f + LDamagePer;
    //
    LTookPhysDamage *= 1.f - LDefensePer; //1이면 안받음,0.9 ,
    LTookFireDamage *= 1.f - LDefensePer;
    LTookElecDamage *= 1.f - LDefensePer;
    LTookIceDamage *= 1.f - LDefensePer;
    LTookPoisonDamage *= 1.f - LDefensePer;
    //
    LTookFireDamage *= 1.f - LResFire; //저항력 마이너스로 추가 피해 입힐때 데미지 팝업으로 표시해줘야함
    LTookElecDamage *= 1.f - LResElec;
    LTookIceDamage *= 1.f - LResCold;
    LTookPoisonDamage *= 1.f - LResPoison;
    //
    LTookPhysDamage -= LDmgReduction;
    //
    if (LTookPhysDamage>0.f&&CheckOnerPercentRand(LCriticalChance))
    {
        float Cache = LTookPhysDamage;
        LTookPhysDamage *= LCriticalDamage;
        PRINTF("Critical! Dmg %f -> %f", Cache, LTookPhysDamage);
    }

    LTookPhysDamage *= CalcPhysReduction(DefenseTargetLevel, AttackerLevel, LPhysicalDefense);

    if (LTookPhysDamage>0.f&&CheckOnerPercentRand(LBashChance))
    {
        PRINTF("BaSher! Stun!");
        FGameplayEffectContextHandle Context = SourceAbilitySystemComponent->MakeEffectContext();
        FGameplayEffectSpecHandle EffectSpecHandle =SourceAbilitySystemComponent->MakeOutgoingSpec(
          m_GEBasherStun, SourceUnit->GetCharacterLevel(),Context);
        SourceAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, TargetAbilitySystemComponent);
        //Basher Need
    }

    if (LTookPhysDamage>0.f&&LLifeSteal > 0.f)
    {
        float HealthGain = LTookPhysDamage * LLifeSteal;
        PRINTF("LifeSteal:%f", HealthGain);
        FGameplayEffectContextHandle Context = SourceAbilitySystemComponent->MakeEffectContext();
        FGameplayEffectSpecHandle EffectSpecHandle =SourceAbilitySystemComponent->MakeOutgoingSpec(
          m_GELifeSteal, SourceUnit->GetCharacterLevel(),Context);
        EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(GetDamageStatics().TagLifeStealHeal, HealthGain);
        SourceAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, SourceAbilitySystemComponent);
        //LifeStealEffect Need
    }

    //
    OutExecutionOutput.AddOutputModifier(
        FGameplayModifierEvaluatedData(GetDamageStatics().TookPhysDamageProperty, EGameplayModOp::Additive,
                                       LTookPhysDamage));
    OutExecutionOutput.AddOutputModifier(
        FGameplayModifierEvaluatedData(GetDamageStatics().TookFireDamageProperty, EGameplayModOp::Additive,
                                       LTookFireDamage));
    OutExecutionOutput.AddOutputModifier(
        FGameplayModifierEvaluatedData(GetDamageStatics().TookIceDamageProperty, EGameplayModOp::Additive,
                                       LTookIceDamage));
    OutExecutionOutput.AddOutputModifier(
        FGameplayModifierEvaluatedData(GetDamageStatics().TookElecDamageProperty, EGameplayModOp::Additive,
                                       LTookElecDamage));
    OutExecutionOutput.AddOutputModifier(
        FGameplayModifierEvaluatedData(GetDamageStatics().TookPoisonDamageProperty, EGameplayModOp::Additive,
                                       LTookPoisonDamage));

    if (CheckOnerPercentRand(LDoubleAttackChance))
    {
        static int DoubleAttackCount=0;
        PRINTF("DoubleAttack!:%d",++DoubleAttackCount);

        if(DoubleAttackCount>5)
        {
            DoubleAttackCount=0;

            return;
        }
        FGameplayEffectContextHandle Context = SourceAbilitySystemComponent->MakeEffectContext();
        FGameplayEffectSpecHandle EffectSpecHandle =SourceAbilitySystemComponent->MakeOutgoingSpec(
          m_GEDoubleAttack, SourceUnit->GetCharacterLevel(),Context);
        //
        EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(GetDamageStatics().TagTookPhysDamage, LTookPhysDamage);

        EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(GetDamageStatics().TagTookFireDamage, LTookFireDamage);

        EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(GetDamageStatics().TagTookElecDamage, LTookElecDamage);

        EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(GetDamageStatics().TagTookPoisonDamage, LTookPoisonDamage);

        EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(GetDamageStatics().TagTookIceDamage, LTookIceDamage);
        //
        SourceAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, TargetAbilitySystemComponent);
        //호출 순서때문에 문제 생길수도 있을듯
    }
}


float UDiabloDamageExec::CalcPhysReduction(int DefenseTargetLevel, const int AttackerLevel, float PhysDefense) const
{
    if (PhysDefense != 0.0f)
    {
        int LevelDiff = DefenseTargetLevel - AttackerLevel;

        if (LevelDiff <= 0)
        {
            LevelDiff = 1;
        }

        float DamageReduceConstant = (50.f / LevelDiff); //작아질수록 뎀지 감소율이 높아짐

        float ReduceRatio = PhysDefense / ((DamageReduceConstant * AttackerLevel) + PhysDefense);

        return 1.f - ReduceRatio; //0.7 3//0.9 // 0.1
    }
    else
    {
        return 1.f;
    }
}

bool UDiabloDamageExec::CanHitBaseAttack(float targetAvoid, float instiAccuracy, int instiLevel) const
{
    float TargetAvoid = targetAvoid;
    float BlockRate = FMath::RandRange(1.f, TargetAvoid); //21을 높이면 회피확률이 는다.
    float InstigatorAccuracy = instiAccuracy;
    float HitRate = (10.f + instiLevel + FMath::RandRange(0.f, InstigatorAccuracy)) -
        FMath::RandRange(1.f, TargetAvoid + 1.f); //여기서 방관 적용 가능

    if (BlockRate < HitRate)
    {
        //아무리 높아도 5%확률로 빗나감
        return 1 != FMath::RandRange(1, 20);
    }

    //아무리 낮아도 5%확률로 맞음
    return 1 == FMath::RandRange(1, 20);
}


float UDiabloDamageExec::CalcuSameLevelAvgAccuracy(float targetAvoidChance, AUnitPawn* instigator) const //맞을 확률
{
    //스텟에 표기되는건? 평균적 동레벨 상대 얘기
    //MinBlock
    float TargetAvoid = targetAvoidChance;
    float A0 = (10 + instigator->GetCharacterLevel()) - TargetAvoid; //10
    float A1 = (9 + instigator->GetCharacterLevel()) + instigator->GetAttributeSet()->GetAccuracy(); //9
    float B0 = 1.f; //1
    float B1 = 20.f + TargetAvoid; //20

    float OuterRight = FMath::Max(B1, A1);
    float OuterLeft = FMath::Min(B0, A0);
    float InnerRight = FMath::Min(B1, A1);
    float InnerLeft = FMath::Max(B0, A0);
    float Percentage = FMath::Max(0.f, FMath::Min(
                                      1.f, (InnerLeft - B0 + (InnerRight - InnerLeft) * 0.5f + OuterRight - B1) / (
                                          OuterRight - OuterLeft)));

    float Result = UDiaBlueprintFunctionLibrary::SetFloatPrecision((Percentage * 100), 1);
    return FMath::Clamp(Result, 5.f, 95.f);
}

bool UDiabloDamageExec::CheckOnerPercentRand(float chanceMaxOne) const
{
    int RandBlock = FMath::RandRange(0, 100);
    int BlockChance100 = chanceMaxOne * 100.f;

    return BlockChance100 > RandBlock;
}
