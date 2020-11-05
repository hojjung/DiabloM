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
        NewValue = FMath::Clamp<float>(NewValue, 150, 10000);
    }
    else if (Attribute == GetAttackSpeedAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.1, 3);
    }
    else if (Attribute == GetDamagePerAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.f, 3.f);
    }
    else if (Attribute == GetDefensePerAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.0f, 0.90f);
    }//마이너스 저항력으로 추가 데미지 받을수 있어야함
    else if (Attribute == GetResColdAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.0f, 0.90f);
    }
    else if (Attribute == GetResPoisonAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.0f, 0.90f);
    }
    else if (Attribute == GetResFireAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.0f, 0.90f);
    }
    else if (Attribute == GetResElecAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 0.0f, 0.90f);
    }
    else if (Attribute == GetPhysicalDamageAttribute())
    {
        PRINTF("NewPhysDmg:%f",NewValue);

    }
    else if (Attribute == GetPhysicalDefenseAttribute())
    {
        PRINTF("NewPhysDef:%f",NewValue);
    }


    
}

void UBaseDiabloAttribute::HandleDamage(AUnitPawn* TargetUnit, AUnitPawn* SourceUnit,
                                        AController* SourceController,
                                        float TookDamageCache)
{
    float RandDmg = FMath::RandRange(0.95f, 1.05f);
    
    const float LocalDamageDone = TookDamageCache * RandDmg;

    if (LocalDamageDone > 1.f)
    {
        const float OldHealth = GetHealth();

        SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

        TargetUnit->PlayTookHitMontage();
        
        if (SourceUnit == ADiabloPlayerController::Get->GetPlayerPawn())
        {
            ADiabloPlayerController* PC = ADiabloPlayerController::Get;

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
    //
    FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
    FGameplayTagContainer SpecAssetTags;
    Data.EffectSpec.GetAllAssetTags(SpecAssetTags);
    //
    AActor* TargetActor = nullptr;
    AUnitPawn* TargetCharacter = nullptr;
    AActor* SourceActor = nullptr;
    AController* SourceController = nullptr;
    AUnitPawn* SourceCharacter = nullptr;
    //
    GetTargetSourceActors(Data, Context, Source, TargetActor,
                          TargetCharacter, SourceActor, SourceController, SourceCharacter);

    if(TargetCharacter==nullptr || SourceCharacter==nullptr)
    {
        return;
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

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
    else if (Data.EvaluatedData.Attribute == GetTookElecDamageAttribute()) //내가 맞았다
    {
        float TookDamageCache = GetTookElecDamage();
        SetTookElecDamage(0.f);
        HandleDamage(TargetCharacter, SourceCharacter, SourceController, TookDamageCache);
    }
    else if (Data.EvaluatedData.Attribute == GetTookPoisonDamageAttribute()) //내가 맞았다
    {
        float TookDamageCache = GetTookPoisonDamage();
        SetTookPoisonDamage(0.f);
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

    else if (Data.EvaluatedData.Attribute == GetPhysicalDamageAttribute())
    {
      PRINTF("PhysDmg:%f",GetPhysicalDamage());
    }
    else if (Data.EvaluatedData.Attribute == GetPhysicalDefenseAttribute())
    {
        PRINTF("PhysDEf:%f",GetPhysicalDefense());
    }

    //속도 측정해볼것
    m_OnStatChanged.Broadcast(TargetCharacter);
}



//계산들도 다 여기서 해야하는거아닌가
