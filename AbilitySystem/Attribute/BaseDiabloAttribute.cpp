#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "AbilitySystem/Components/DiabloAbilitySystemComp.h"
#include "Characters/UnitPawn.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Characters/PlayerDiabloCharacter.h"

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
}

void UBaseDiabloAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    // AActor* TargetActor = nullptr; //여기서 타겟은 나다.그럼 버프 디버프를 아군 적군이 걸어주는것은?
    // AController* TargetController = nullptr;
    // AUnitPawn* TargetCharacter = nullptr;
    // AActor* SourceActor = nullptr; //여기서 타겟은 나다.그럼 버프 디버프를 아군 적군이 걸어주는것은?
    // AController* SourceController = nullptr;
    // AUnitPawn* SourceCharacter = nullptr;
    // FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    // UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
    // const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
    // float Value = 0.f;
    FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
    FGameplayTagContainer SpecAssetTags;
    Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

    AActor* TargetActor = nullptr;
    AController* TargetController = nullptr;
    AUnitPawn* TargetCharacter = nullptr;
    // Get the Source actor
    AActor* SourceActor = nullptr;
    AController* SourceController = nullptr;
    AUnitPawn* SourceCharacter = nullptr;

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
        TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        TargetCharacter = Cast<AUnitPawn>(TargetActor);
    }
  
    if (Data.EvaluatedData.Attribute == GetTookDamageAttribute()) //내가 맞았다
    {
        if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
        {
            SourceActor = Source->AbilityActorInfo->AvatarActor.Get();

            SourceCharacter = Cast<AUnitPawn>(SourceActor);

            if (Context.GetEffectCauser())
            {
                SourceActor = Context.GetEffectCauser();
            }
        }


        const float LocalDamageDone = GetTookDamage();
        SetTookDamage(0.f);

        if (LocalDamageDone > 0)
        {
            bool WasAlive = true;

            if (TargetCharacter)
            {
                WasAlive = TargetCharacter->IsAlive();
            }

            const float OldHealth = GetHealth();

            SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

         

            if (TargetCharacter && WasAlive) //데메지가 닳는것은 나임, 여기서 소스는 적군임
            {
                // This is proper damage
                FHitResult HitResult;
                if (Context.GetHitResult())
                {
                    HitResult = *Context.GetHitResult();
                }
            }

            ADiabloPlayerController* PC = nullptr;
            PC = Cast<ADiabloPlayerController>(TargetController);
            
            if(PC)
            {
                PC->ShowDamageNumber(LocalDamageDone, TargetCharacter,EDamagePopup::PlayerHurt);
                PC=nullptr;
            }
            else if (SourceActor != TargetActor)
            {
                PC = Cast<ADiabloPlayerController>(SourceController);

                if (PC)
                {
                    bool IsCritical=false;//TODO :Critical Strike
                    
                    EDamagePopup DmgDir=EDamagePopup::NormalLeft;
                    
                    bool IsLeft = FMath::RandBool();
                    
                    if(IsCritical)
                    {
                        DmgDir = IsLeft ? EDamagePopup::CritcalLeft :EDamagePopup::CritcalRight; 
                    }
                    else
                    {
                        DmgDir = IsLeft ? EDamagePopup::NormalLeft :EDamagePopup::NormalRight;
                    }
                    //1.중첩되지 않게 번갈아가며 왼오
                    //2. 몬스터가 왼쪽이면 왼쪽 오른쪽이면 오른쪽
                    PC->ShowDamageNumber(LocalDamageDone, TargetCharacter,DmgDir);
                }
            }

            if (!TargetCharacter->IsAlive())
            {
                if (PC && TargetCharacter->HasDropItem())
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

                TargetCharacter->Die();
            }
        }
    }
    else if (Data.EvaluatedData.Attribute == GetPhysicalDamageAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
    }
    else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
    {
        if (TargetCharacter)
        {
            Cast<UUnitMovement>(TargetCharacter->GetMovementComponent())->SetMoveSpeed(GetMoveSpeed());
        }
    }
    else if (Data.EvaluatedData.Attribute == GetAttackSpeedAttribute())
    {
        //float attack speed multiple
    }
    else if (Data.EvaluatedData.Attribute == GetPhysicalDefenseAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetAccuracyAttribute())
    {
        float AccuRacy = FMath::Clamp(GetAccuracy(), 0.0f, 99.9f);
        SetAccuracy(AccuRacy);
    }
    else if (Data.EvaluatedData.Attribute == GetAvoidChanceAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetDamagePerAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetDefensePerAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetAtkColdAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetAtkElecAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetAtkFireAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetAtkPoisonAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetResColdAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetResElecAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetResFireAttribute())
    {
    }
    else if (Data.EvaluatedData.Attribute == GetResPoisonAttribute())
    {
    }

    //속도 측정해볼것
    m_OnStatChanged.Broadcast(TargetCharacter);
}
