#include "AbilitySystem/Attribute/BaseDiabloAttribute.h"
#include "AbilitySystem/DiabloAbilitySystemComp.h"
#include "Characters/UnitPawn.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

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
        // Change current value to maintain the current Val / Max percent
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
    
    AActor* TargetActor = nullptr;
    AController* TargetController = nullptr;
    AUnitPawn* TargetCharacter = nullptr;
    FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        TargetCharacter = Cast<AUnitPawn>(TargetActor);
    }
    //
    if (Data.EvaluatedData.Attribute == GetPhysicalDamageAttribute())
    {
        AActor* SourceActor = nullptr;
        AUnitPawn* SourceCharacter = nullptr;
        if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
        {
            SourceActor = Source->AbilityActorInfo->AvatarActor.Get();

            SourceCharacter = Cast<AUnitPawn>(SourceActor);

            if (Context.GetEffectCauser())
            {
                SourceActor = Context.GetEffectCauser();
            }
        }

        FHitResult HitResult;
        if (Context.GetHitResult())
        {
            HitResult = *Context.GetHitResult();
        }

        const float LocalDamageDone = GetPhysicalDamage() * GetDamagePer();

        if (LocalDamageDone > 0)
        {
            const float OldHealth = GetHealth();
            SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

            if (TargetCharacter)
            {
                // This is proper damage
                //TargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);

                // Call for all health changes
                //TargetCharacter->HandleHealthChanged(-LocalDamageDone, SourceTags);
            }
        }
    }
    else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        // Handle other health changes such as from healing or direct modifiers
        // First clamp it
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

        if (TargetCharacter)
        {
            // Call for all health changes
            //TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
        }
    }


        //else if (Data.EvaluatedData.Attribute == GetManaAttribute())
        //{
        //	// Clamp mana
        //	SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));

        //	if (TargetCharacter)
        //	{
        //		// Call for all mana changes
        //		TargetCharacter->HandleManaChanged(DeltaValue, SourceTags);
        //	}
        //}
    else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
    {
        if (TargetCharacter)
        {
            Cast<UUnitMovement>(TargetCharacter->GetMovementComponent())->SetMoveSpeed(GetMoveSpeed());
        }
    }

}

void UBaseDiabloAttribute::PrintStats()
{
    PRINTF("MaxHealth: %f", GetMaxHealth());
    PRINTF("Health: %f", GetHealth());
    PRINTF("AttackPower: %f", GetPhysicalDamage());
    PRINTF("AttackPowerPer: %f", GetDamagePer());
    PRINTF("DefensePower: %f", GetPhysicalDefense());
    PRINTF("DefensePowerPer: %f", GetDefensePer());
    PRINTF("MoveSpeed: %f", GetMoveSpeed());
}
