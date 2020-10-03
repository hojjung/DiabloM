#include "PlayerBaseAttack.h"

#include "AbilitySystem/Task/PlayMontageAndWaitForEvent.h"
#include "Characters/PlayerDiabloCharacter.h"

UPlayerBaseAttack::UPlayerBaseAttack()
{
    m_nSectionIndex=-1;
    
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Ability.BaseAttack"));
    AbilityTags.AddTag(Ability1Tag);
    ActivationOwnedTags.AddTag(Ability1Tag);

    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));

    Range = 1000.0f;
    Damage = 12.0f;

    m_AbilityInputID =EAbilityInputID::BaseAttack;
    
    m_AbilityID =EAbilityInputID::BaseAttack;
}

void UPlayerBaseAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
       // m_nSectionIndex=0;
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    }

    UAnimMontage* MontageToPlay = m_BaseAttackMotion;

    float AttackSpeed=Cast<APlayerDiabloCharacter>( ActorInfo->AvatarActor.Get())->GetAttackSpeed();
    
    UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
        this,NAME_None, MontageToPlay, FGameplayTagContainer(), AttackSpeed,
        GetSectionName(), false, 1.0f);
    
    Task->OnBlendOut.AddDynamic(this, &UPlayerBaseAttack::OnCompleted);
    Task->OnCompleted.AddDynamic(this, &UPlayerBaseAttack::OnCompleted);
    Task->OnInterrupted.AddDynamic(this, &UPlayerBaseAttack::OnCancelled);
    Task->OnCancelled.AddDynamic(this, &UPlayerBaseAttack::OnCancelled);
    Task->EventReceived.AddDynamic(this, &UPlayerBaseAttack::EventReceived);
    
    Task->ReadyForActivation();

    // test
    // auto* MM = Cast<APlayerDiabloCharacter>( ActorInfo->AvatarActor.Get())->GetMovementComponent();
    // auto* AA = Cast<UUnitMovement>(MM);
    //
    // FVector TestDelta=ActorInfo->AvatarActor.Get()->GetActorForwardVector() * 1500.f;
    // AA->SetDash(TestDelta,0.5f);
}

void UPlayerBaseAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    m_AryMontageSections = m_BaseAttackMotion->GetArySections();
    ResetComboSection();
}

void UPlayerBaseAttack::ResetComboSection()
{
    m_nSectionIndex=0;
}

void UPlayerBaseAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
    ResetComboSection();
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UPlayerBaseAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
    //Each one base attack
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPlayerBaseAttack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
    {
        ResetComboSection();
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }
    //Ability.BaseAttack
    PRINTF("EventReceive");
    // Only spawn projectiles on the Server.
    // Predicting projectiles is an advanced topic not covered in this example.
    if (EventTag == FGameplayTag::RequestGameplayTag(FName("Ability.BaseAttack")))
    {
        APlayerDiabloCharacter* Hero = Cast<APlayerDiabloCharacter>(GetAvatarActorFromActorInfo());
        if (!Hero)
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        }
    
        FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
		  
        // Pass the damage to the Damage Execution Calculation through a SetByCaller value on the GameplayEffectSpec
        DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Combat.TookDamage")), Damage);
    
        Cast<AUnitPawn>(EventData.Target)->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);
    }
}

FName UPlayerBaseAttack::GetSectionName()
{
    FName WantReturn= m_AryMontageSections[m_nSectionIndex++].SectionName;

    if(m_nSectionIndex>=m_AryMontageSections.Num())
    {
        m_nSectionIndex=0;
    }

    return WantReturn;
}
