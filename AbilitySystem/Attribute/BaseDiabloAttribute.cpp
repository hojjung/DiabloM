#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "AbilitySystem/Components/DiabloAbilitySystemComp.h"
#include "Characters/UnitPawn.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Lib/DiaBlueprintFunctionLibrary.h"

UBaseDiabloAttribute::UBaseDiabloAttribute()
{
}

//스태미너 등 맥스값 바뀔때 퍼센트로 수치 따라감
void UBaseDiabloAttribute::AdjustAttributeForMaxChange(
    FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute,
    float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
    UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
    const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
    if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
    {
        const float CurrentValue = AffectedAttribute.GetCurrentValue();
        float NewDelta = (CurrentMaxValue > 0.f)
                             ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue
                             : NewMaxValue;

        AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
    }
}

void UBaseDiabloAttribute::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    if (NewValue <= 0.f)
    {
        return;
    }

    if (Attribute == GetMaxHealthAttribute())
    {
        AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
    }
    else if (Attribute == GetMoveSpeedAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
    }
    else if (Attribute == GetAttackSpeedAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.1, 3);
    }
    else if (Attribute == GetDamagePerAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 1.f, 3.f);
    }
    else if (Attribute == GetDefensePerAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.0f, 0.99f);
    }
    else if (Attribute == GetResColdAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.0f, 0.99f);
    }
    else if (Attribute == GetResPoisonAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.0f, 0.99f);
    }
    else if (Attribute == GetResFireAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.0f, 0.99f);
    }
    else if (Attribute == GetResElecAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.0f, 0.99f);
    }
    
}

void UBaseDiabloAttribute::HandleDamage(AUnitPawn* TargetUnit, AUnitPawn* SourceUnit,
                                        AController* SourceController,
                                        float TookDamageCache)
{
    float RandDmg = FMath::RandRange(0.95f, 1.05f);
    const float LocalDamageDone = TookDamageCache * RandDmg;

    if (LocalDamageDone > 0)
    {
        bool WasAlive = true;

        if (TargetUnit)
        {
            WasAlive = TargetUnit->IsAlive();
        }

        const float OldHealth = GetHealth();

        SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

        if (TargetUnit == ADiabloPlayerController::Get->GetPlayerPawn())
        {
            ADiabloPlayerController::Get->ShowDamageNumber(LocalDamageDone, TargetUnit, EDamagePopup::PlayerHurt);
        }
        else if (SourceUnit != TargetUnit)
        {
            ADiabloPlayerController* PC = nullptr;
            PC = Cast<ADiabloPlayerController>(SourceController);

            if (PC)
            {
                bool IsCritical = false; //TODO :Critical Strike

                EDamagePopup DmgDir = EDamagePopup::NormalLeft;

                bool IsLeft = FMath::RandBool();

                if (IsCritical)
                {
                    DmgDir = IsLeft ? EDamagePopup::CritcalLeft : EDamagePopup::CritcalRight;
                }
                else
                {
                    DmgDir = IsLeft ? EDamagePopup::NormalLeft : EDamagePopup::NormalRight;
                }
                //1.중첩되지 않게 번갈아가며 왼오
                //2. 몬스터가 왼쪽이면 왼쪽 오른쪽이면 오른쪽
                PC->ShowDamageNumber(LocalDamageDone, TargetUnit, DmgDir);
            }
        }

        if (!TargetUnit->IsAlive())
        {
            //if (PC && TargetCharacter->HasDropItem())
            {
                //?Earn Exp,Gold
                //
                // Create a dynamic instant Gameplay Effect to give the bounties
                // UGameplayEffect* GEBounty = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
                // GEBounty->DurationPolicy = EGameplayEffectDurationType::Instant;
                //
                // int32 Idx = GEBounty->Modifiers.Num();
                // GEBounty->Modifiers.SetNum(Idx + 2);
                //
                // FGameplayModifierInfo& InfoXP = GEBounty->Modifiers[Idx];
                // InfoXP.ModifierMagnitude = FScalableFloat(GetXPBounty());
                // InfoXP.ModifierOp = EGameplayModOp::Additive;
                // InfoXP.Attribute = UBaseDiabloAttribute::GetXPAttribute();
                //
                // FGameplayModifierInfo& InfoGold = GEBounty->Modifiers[Idx + 1];
                // InfoGold.ModifierMagnitude = FScalableFloat(GetGoldBounty());
                // InfoGold.ModifierOp = EGameplayModOp::Additive;
                // InfoGold.Attribute = UBaseDiabloAttribute::GetGoldAttribute();
                //
                // Source->ApplyGameplayEffectToSelf(GEBounty, 1.0f, Source->MakeEffectContext());
            }

            TargetUnit->Die();
        }
    }
}

void UBaseDiabloAttribute::GetTargetSourceActors(const FGameplayEffectModCallbackData& Data,
                                                 FGameplayEffectContextHandle& Context, UAbilitySystemComponent* Source,
                                                 AActor*& TargetActor, AUnitPawn*& TargetCharacter,
                                                 AActor*& SourceActor, AController*& SourceController,
                                                 AUnitPawn*& SourceCharacter)
{
    if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
    {
        SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
        SourceController = Source->AbilityActorInfo->PlayerController.Get();
        if (SourceController == nullptr && SourceActor != nullptr)
        {
            if (APawn* Pawn = Cast<APawn>(SourceActor))
            {
                SourceController = Pawn->GetController();
            }
        }

        if (SourceController)
        {
            SourceCharacter = Cast<AUnitPawn>(SourceController->GetPawn());
        }
        else
        {
            SourceCharacter = Cast<AUnitPawn>(SourceActor);
        }

        if (Context.GetEffectCauser())
        {
            SourceActor = Context.GetEffectCauser();
        }
    }

    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        TargetCharacter = Cast<AUnitPawn>(TargetActor);
    }
}

void UBaseDiabloAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
    FGameplayTagContainer SpecAssetTags;
    Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

    AActor* TargetActor = nullptr;
    AUnitPawn* TargetCharacter = nullptr;
    AActor* SourceActor = nullptr;
    AController* SourceController = nullptr;
    AUnitPawn* SourceCharacter = nullptr;

    GetTargetSourceActors(Data, Context, Source, TargetActor, TargetCharacter, SourceActor, SourceController,
                          SourceCharacter);

    /////////////////////////////
    /////치명타 등 연산 처리 모두 여기서 하는게 맞다 생각한다.

    if (Data.EvaluatedData.Attribute == GetTookPhysDamageAttribute()) //내가 맞았다
    {
        
        float TookDamageCache = GetTookPhysDamage();
        SetTookPhysDamage(0.f);
        HandleDamage(TargetCharacter, SourceCharacter, SourceController, TookDamageCache);
    }
    else if (Data.EvaluatedData.Attribute == GetTookFireDamageAttribute()) //내가 맞았다
    {
        float TookDamageCache = GetTookFireDamage();
        SetTookFireDamage(0.f);
        HandleDamage(TargetCharacter, SourceCharacter, SourceController, TookDamageCache);
    }
    else if (Data.EvaluatedData.Attribute == GetTookIceDamageAttribute()) //내가 맞았다
    {
        float TookDamageCache = GetTookIceDamage();
        SetTookIceDamage(0.f);

        HandleDamage(TargetCharacter, SourceCharacter, SourceController, TookDamageCache);
    }
    else if (Data.EvaluatedData.Attribute == GetTookPoisonDamageAttribute()) //내가 맞았다
    {
        float TookDamageCache = GetTookPoisonDamage();
        SetTookPoisonDamage(0.f);

        HandleDamage(TargetCharacter, SourceCharacter, SourceController, TookDamageCache);
    }

    else if (Data.EvaluatedData.Attribute == GetTookElecDamageAttribute()) //내가 맞았다
    {
        float TookDamageCache = GetTookElecDamage();
        SetTookElecDamage(0.f);

        HandleDamage(TargetCharacter, SourceCharacter, SourceController, TookDamageCache);
    }
    else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
    }
    else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
    {
        if (TargetCharacter)
        {
            Cast<UUnitMovement>(TargetCharacter->GetMovementComponent())->SetMoveSpeed(GetMoveSpeed());
        }
    }

    //속도 측정해볼것
    m_OnStatChanged.Broadcast(TargetCharacter);
}

bool UBaseDiabloAttribute::CanHitBaseAttack(const AUnitPawn* target, const AUnitPawn* instigator)
{
    float TargetAvoid = target->GetAttributeSet()->GetAvoid();
    float BlockRate = FMath::RandRange(1.f, TargetAvoid); //21을 높이면 회피확률이 는다.
    float InstigatorAccuracy = instigator->GetAttributeSet()->GetAccuracy();
    float HitRate = (10.f + instigator->GetCharacterLevel() + FMath::RandRange(0.f, InstigatorAccuracy)) -
        FMath::RandRange(1.f, TargetAvoid + 1.f); //여기서 방관 적용 가능

    if (BlockRate < HitRate)
    {
        //아무리 높아도 5%확률로 빗나감
        return 1 != FMath::RandRange(1, 20);
    }

    //아무리 낮아도 5%확률로 맞음
    return 1 == FMath::RandRange(1, 20);
}

float UBaseDiabloAttribute::CalcuSameLevelAvgAccuracy(float targetAvoidChance, AUnitPawn* instigator) //맞을 확률
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

//계산들도 다 여기서 해야하는거아닌가
