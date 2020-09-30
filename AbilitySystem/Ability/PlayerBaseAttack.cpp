#include "PlayerBaseAttack.h"

#include "AbilitySystem/Task/PlayMontageAndWaitForEvent.h"

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
    PRINTF("Activated BaseAttack");
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
       // m_nSectionIndex=0;
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    }

    UAnimMontage* MontageToPlay = m_BaseAttackMotion;

    
    //if (GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.AimDownSights"))) &&
     //   !GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.AimDownSights.Removal"))))
    {
        //MontageToPlay = FireIronsightsMontage;
    }

    // Play fire montage and wait for event telling us to spawn the projectile
    UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this,NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f,  GetSectionName(), false, 1.0f);
    Task->OnBlendOut.AddDynamic(this, &UPlayerBaseAttack::OnCompleted);
    Task->OnCompleted.AddDynamic(this, &UPlayerBaseAttack::OnCompleted);
    Task->OnInterrupted.AddDynamic(this, &UPlayerBaseAttack::OnCancelled);
    Task->OnCancelled.AddDynamic(this, &UPlayerBaseAttack::OnCancelled);
    Task->EventReceived.AddDynamic(this, &UPlayerBaseAttack::EventReceived);
    // ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
    Task->ReadyForActivation();
}

void UPlayerBaseAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    m_AryMontageSections = m_BaseAttackMotion->GetArySections();
    m_nSectionIndex=0;
}

void UPlayerBaseAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
  //  m_nSectionIndex=0;
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UPlayerBaseAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
  //  m_nSectionIndex=0;
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPlayerBaseAttack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
    {
      //  m_nSectionIndex=0;
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }
    PRINTF("EventReceive");
    // Only spawn projectiles on the Server.
    // Predicting projectiles is an advanced topic not covered in this example.
    // if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.SpawnProjectile")))
    // {
    //     AGDHeroCharacter* Hero = Cast<AGDHeroCharacter>(GetAvatarActorFromActorInfo());
    //     if (!Hero)
    //     {
    //         EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    //     }
    //
    //     FVector Start = Hero->GetGunComponent()->GetSocketLocation(FName("Muzzle"));
    //     FVector End = Hero->GetCameraBoom()->GetComponentLocation() + Hero->GetFollowCamera()->GetForwardVector() * Range;
    //     FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
    //
    //     FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, GetAbilityLevel());
		  //
    //     // Pass the damage to the Damage Execution Calculation through a SetByCaller value on the GameplayEffectSpec
    //     DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage);
    //
    //     FTransform MuzzleTransform = Hero->GetGunComponent()->GetSocketTransform(FName("Muzzle"));
    //     MuzzleTransform.SetRotation(Rotation.Quaternion());
    //     MuzzleTransform.SetScale3D(FVector(1.0f));
    //
    //     FActorSpawnParameters SpawnParameters;
    //     SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    //
    //     AGDProjectile* Projectile = GetWorld()->SpawnActorDeferred<AGDProjectile>(ProjectileClass, MuzzleTransform, GetOwningActorFromActorInfo(),
    //         Hero, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    //     Projectile->DamageEffectSpecHandle = DamageEffectSpecHandle;
    //     Projectile->Range = Range;
    //     Projectile->FinishSpawning(MuzzleTransform);
    // }
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
