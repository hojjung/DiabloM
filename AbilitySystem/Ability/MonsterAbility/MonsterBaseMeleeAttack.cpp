#include "MonsterBaseMeleeAttack.h"
#include "AbilitySystem/Task/PlayMontageAndWaitForEvent.h"
#include "Characters/MonsterPawn.h"
#include "Characters/PlayerDiabloCharacter.h"

UMonsterBaseMeleeAttack::UMonsterBaseMeleeAttack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Ability.BaseAttack"));
    AbilityTags.AddTag(Ability1Tag);
    ActivationOwnedTags.AddTag(Ability1Tag);
    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));
}

void UMonsterBaseMeleeAttack::PlayAbilityAnimation(UAnimMontage* MontageToPlay, FName playSection, float AttackSpeed)
{
    UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
        this, NAME_None, MontageToPlay, FGameplayTagContainer(), AttackSpeed,
        playSection, false, 1.0f);

    Task->OnBlendOut.AddDynamic(this, &UMonsterBaseMeleeAttack::OnCompleted);
    Task->OnCompleted.AddDynamic(this, &UMonsterBaseMeleeAttack::OnCompleted);
    Task->OnInterrupted.AddDynamic(this, &UMonsterBaseMeleeAttack::OnCancelled);
    Task->OnCancelled.AddDynamic(this, &UMonsterBaseMeleeAttack::OnCancelled);
    Task->EventReceived.AddDynamic(this, &UMonsterBaseMeleeAttack::EventReceived);

    Task->ReadyForActivation();

}

void UMonsterBaseMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    }

    PlayAbilityAnimation(m_BaseAttackMotion,NAME_None ,1);
}

void UMonsterBaseMeleeAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

}

void UMonsterBaseMeleeAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMonsterBaseMeleeAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMonsterBaseMeleeAttack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }
    
    if (EventTag == FGameplayTag::RequestGameplayTag(FName("Ability.BaseAttack")))
    {
        AMonsterPawn* MonsterAttacker = Cast<AMonsterPawn>(GetAvatarActorFromActorInfo());
        if (!MonsterAttacker)
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        }

        FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
            DamageGameplayEffect, GetAbilityLevel());

        DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(
            FGameplayTag::RequestGameplayTag(FName("Data.Combat.TookDamage")), MonsterAttacker->GetAttributeSet()->GetPhysicalDamage());

        Cast<AUnitPawn>(EventData.Target)->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToSelf(
            *DamageEffectSpecHandle.Data);
    }
}
