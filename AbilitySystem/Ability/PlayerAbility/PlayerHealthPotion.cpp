// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealthPotion.h"


#include "AbilitySystem/Attribute/PlayerDiabloAttribute.h"
#include "AbilitySystem/Task/PlayMontageAndWaitForEvent.h"
#include "Characters/PlayerDiabloCharacter.h"

UPlayerHealthPotion::UPlayerHealthPotion()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    m_TagHealthHeal = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.HpHeal"));

    AbilityTags.AddTag(m_TagHealthHeal);

    ActivationOwnedTags.AddTag(m_TagHealthHeal);

    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Combat.Ability.Skill")));

    m_AbilityInputID = EAbilityInputID::DrinkHpPotion;

    m_AbilityID = EAbilityInputID::DrinkHpPotion;

    m_TagEventEndAbility = FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility"));

    m_TagEventReceive = FGameplayTag::RequestGameplayTag(FName("Combat.Ability.DrinkPotion"));
}

void UPlayerHealthPotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    }
    
    float CastSpeed = m_PlayerPawn->GetCastSpeed();

    PlayAbilityAnimation(m_PotionDrinkMotion, NAME_None, 0.2f);
}

void UPlayerHealthPotion::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);
}

void UPlayerHealthPotion::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UPlayerHealthPotion::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPlayerHealthPotion::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    if (EventTag == m_TagEventEndAbility)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }
    
    if (EventTag == m_TagEventReceive)
    {
        if (!m_PlayerPawn)
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        }

        FGameplayEffectSpecHandle PotionEffectSpec = MakeOutgoingGameplayEffectSpec(m_GEPotionHeal, GetAbilityLevel());

        float HealHp = m_PlayerPawn->GetMaxHealth() * (m_fHealPercent + m_PlayerPawn->GetPlayerAttribute()->GetPotionBonusPer());

        PotionEffectSpec.Data.Get()->SetSetByCallerMagnitude(m_TagHealthHeal, HealHp);

        m_PlayerPawn->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToSelf(*PotionEffectSpec.Data);

        PRINTF("Health HEal");
    }
}

void UPlayerHealthPotion::PlayAbilityAnimation(UAnimMontage* MontageToPlay, FName playSection, float AttackSpeed)
{
    UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
        this, NAME_None, MontageToPlay, FGameplayTagContainer(), AttackSpeed,
        playSection, false, 1.0f);

    Task->OnBlendOut.AddDynamic(this, &UPlayerHealthPotion::OnCompleted);
    Task->OnCompleted.AddDynamic(this, &UPlayerHealthPotion::OnCompleted);
    Task->OnInterrupted.AddDynamic(this, &UPlayerHealthPotion::OnCancelled);
    Task->OnCancelled.AddDynamic(this, &UPlayerHealthPotion::OnCancelled);
    Task->EventReceived.AddDynamic(this, &UPlayerHealthPotion::EventReceived);

    Task->ReadyForActivation();

}
