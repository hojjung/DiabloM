#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "AbilitySystem/DiabloAbilitySystemComp.h"
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
}

void UBaseDiabloAttribute::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    AActor* TargetActor = nullptr; //여기서 타겟은 나다.그럼 버프 디버프를 아군 적군이 걸어주는것은?
    AController* TargetController = nullptr;
    AUnitPawn* TargetCharacter = nullptr;
    AActor* SourceActor = nullptr; //여기서 타겟은 나다.그럼 버프 디버프를 아군 적군이 걸어주는것은?
    AController* SourceController = nullptr;
    AUnitPawn* SourceCharacter = nullptr;
    FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
    float Value = 0.f;

    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        TargetCharacter = Cast<AUnitPawn>(TargetActor);
    }

    if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
    {
        SourceActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        SourceController = Data.Target.AbilityActorInfo->PlayerController.Get();
        SourceCharacter = Cast<AUnitPawn>(SourceActor);
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
                TargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);

                TargetCharacter->HandleHealthChanged(GetHealth(), GetMaxHealth(), SourceTags);
            }

            APlayerDiabloCharacter* PC = nullptr;
            if (SourceActor != TargetActor)
            {
                PC = Cast<APlayerDiabloCharacter>(SourceController);

                if (PC)
                {
                    PC->ShowDamageNumber(LocalDamageDone, TargetCharacter);
                }
            }

            if (!TargetCharacter->IsAlive())
            {
                if (PC)
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
            }
        }
    }
    else if (Data.EvaluatedData.Attribute == GetPhysicalDamageAttribute())
    {
        m_OnChangedPhysicalDamage.Broadcast(GetPhysicalDamage());
    }
    else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        Value = FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth());
        SetHealth(Value);
        m_OnHealthChangePer.Broadcast(GetHealth() / GetMaxHealth());
    }
    else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
    {
        m_OnChangedMaxHealth.Broadcast(GetMaxHealth());
        m_OnHealthChangePer.Broadcast(GetHealth() / GetMaxHealth());
    }
    else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
    {
        if (TargetCharacter)
        {
            PRINTF("MoveSpeed:%f",GetMoveSpeed());
            Cast<UUnitMovement>(TargetCharacter->GetMovementComponent())->SetMoveSpeed(GetMoveSpeed());
            m_OnChangedMoveSpeed.Broadcast(GetMoveSpeed() / 600.f); //need get base Movement,//초기값저장?
        }
    }
    else if (Data.EvaluatedData.Attribute == GetAttackSpeedAttribute())
    {
        //float attack speed multiple
        TargetCharacter->SetAttackSpeed(GetAttackSpeed());
        m_OnChangedAttackSpeed.Broadcast(GetAttackSpeed());
    }
    else if (Data.EvaluatedData.Attribute == GetPhysicalDefenseAttribute())
    {
        m_OnChangedPhysicalDefense.Broadcast(GetPhysicalDefense());
    }
    else if (Data.EvaluatedData.Attribute == GetAccuracyAttribute())
    {
        float AccuRacy = FMath::Clamp(GetAccuracy(), 0.0f, 99.9f);
        SetAccuracy(AccuRacy);
        m_OnChangedAccuracy.Broadcast(GetAccuracy());
    }
    else if (Data.EvaluatedData.Attribute == GetAvoidChanceAttribute())
    {
        m_OnChangedAvoidChance.Broadcast(GetAvoidChance());
    }
    else if (Data.EvaluatedData.Attribute == GetDamagePerAttribute())
    {
        m_OnChangedDamagePer.Broadcast(GetDamagePer());
    }
    else if (Data.EvaluatedData.Attribute == GetDefensePerAttribute())
    {
        m_OnChangedDefensePer.Broadcast(GetDefensePer());
    }
    else if (Data.EvaluatedData.Attribute == GetAtkColdAttribute())
    {
        m_OnChangedAtkCold.Broadcast(GetAtkCold());
    }
    else if (Data.EvaluatedData.Attribute == GetAtkElecAttribute())
    {
        m_OnChangedAtkElec.Broadcast(GetAtkElec());
    }
    else if (Data.EvaluatedData.Attribute == GetAtkFireAttribute())
    {
        m_OnChangedAtkFire.Broadcast(GetAtkFire());
    }
    else if (Data.EvaluatedData.Attribute == GetAtkPoisonAttribute())
    {
        m_OnChangedAtkPoison.Broadcast(GetAtkPoison());
    }
    else if (Data.EvaluatedData.Attribute == GetResColdAttribute())
    {
        m_OnChangedResCold.Broadcast(GetResCold());
    }
    else if (Data.EvaluatedData.Attribute == GetResElecAttribute())
    {
        m_OnChangedResElec.Broadcast(GetResElec());
    }
    else if (Data.EvaluatedData.Attribute == GetResFireAttribute())
    {
        m_OnChangedResFire.Broadcast(GetResFire());
    }
    else if (Data.EvaluatedData.Attribute == GetResPoisonAttribute())
    {
        m_OnChangedResPoison.Broadcast(GetResPoison());
    }
}
