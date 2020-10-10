#include "PlayerBaseAttack.h"

#include "AbilitySystem/Task/PlayMontageAndWaitForEvent.h"
#include "Characters/PlayerDiabloCharacter.h"

UPlayerBaseAttack::UPlayerBaseAttack()
{
    m_nSectionIndex = -1;

    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Ability.BaseAttack"));
    AbilityTags.AddTag(Ability1Tag);
    ActivationOwnedTags.AddTag(Ability1Tag);

    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));

    m_fDashLimitRange = 700.0f;
    m_fDashTime=0.5f;
    m_AbilityInputID = EAbilityInputID::BaseAttack;

    m_AbilityID = EAbilityInputID::BaseAttack;

}

void UPlayerBaseAttack::PlayAbilityAnimation(UAnimMontage* MontageToPlay, FName playSection, float AttackSpeed)
{
    UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
        this, NAME_None, MontageToPlay, FGameplayTagContainer(), AttackSpeed,
        playSection, false, 1.0f);

    Task->OnBlendOut.AddDynamic(this, &UPlayerBaseAttack::OnCompleted);
    Task->OnCompleted.AddDynamic(this, &UPlayerBaseAttack::OnCompleted);
    Task->OnInterrupted.AddDynamic(this, &UPlayerBaseAttack::OnCancelled);
    Task->OnCancelled.AddDynamic(this, &UPlayerBaseAttack::OnCancelled);
    Task->EventReceived.AddDynamic(this, &UPlayerBaseAttack::EventReceived);

    Task->ReadyForActivation();

}

void UPlayerBaseAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    }

    float AttackSpeed = Cast<APlayerDiabloCharacter>(ActorInfo->AvatarActor.Get())->GetAttackSpeed();

    GetMovement(ActorInfo->AvatarActor.Get())->SetMoveSpeedRatio(0.2f);

    float DistSqred;
    FVector DashNormal;
    
    if(IsDashable(ActorInfo,DistSqred,DashNormal))
    {
        PlayAbilityAnimation(m_BaseAttackMotion,"DashAttack" ,2);

        DashAttack(ActorInfo->AvatarActor.Get(),DashNormal,FMath::Sqrt(DistSqred-5400),m_fDashTime);
    }
    else
    {
        PlayAbilityAnimation(m_BaseAttackMotion,GetSectionName() ,AttackSpeed);
    }
}

void UPlayerBaseAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    m_AryMontageSections = m_BaseAttackMotion->GetArySections();
    ResetComboSection();
}

void UPlayerBaseAttack::ResetComboSection()
{
    m_nSectionIndex = 0;
}

void UPlayerBaseAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
    ResetComboSection();
    GetMovement(CurrentActorInfo->AvatarActor.Get())->SetMoveSpeedRatio(1.f);
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UPlayerBaseAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
    //Each one base attack
    GetMovement(CurrentActorInfo->AvatarActor.Get())->SetMoveSpeedRatio(1.f);
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPlayerBaseAttack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
    {
        ResetComboSection();
        GetMovement(CurrentActorInfo->AvatarActor.Get())->SetMoveSpeedRatio(1.f);
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }
    
    if (EventTag == FGameplayTag::RequestGameplayTag(FName("Ability.BaseAttack")))
    {
        APlayerDiabloCharacter* Hero = Cast<APlayerDiabloCharacter>(GetAvatarActorFromActorInfo());
        if (!Hero)
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        }

        FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
            DamageGameplayEffect, GetAbilityLevel());

        DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(
            FGameplayTag::RequestGameplayTag(FName("Data.Combat.TookDamage")), Hero->GetAttributeSet()->GetPhysicalDamage()*Hero->GetBonusDamage());

        Cast<AUnitPawn>(EventData.Target)->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToSelf(
            *DamageEffectSpecHandle.Data);
    }
}

FName UPlayerBaseAttack::GetSectionName()
{
    FName WantReturn = m_AryMontageSections[m_nSectionIndex++].SectionName;

    if (m_nSectionIndex >= m_AryMontageSections.Num()-1)//- Skip Dash Section
    {
        m_nSectionIndex = 0;
    }

    return WantReturn;
}

UUnitMovement* UPlayerBaseAttack::GetMovement(AActor* want)
{
    auto* MM = Cast<APlayerDiabloCharacter>(want)->GetMovementComponent();
    auto* AA = Cast<UUnitMovement>(MM);
    return AA;
}

void UPlayerBaseAttack::DashAttack(AActor* want,FVector dashNormal, float dashLength, float dashTime)
{
    auto* PlayerChar =Cast<APlayerDiabloCharacter>(want);
    FVector DeltaDash=dashNormal * (dashLength/m_fDashTime);
    GetMovement(want)->SetDash(DeltaDash,dashTime);
}

bool UPlayerBaseAttack::IsDashable(const FGameplayAbilityActorInfo* ActorInfo,float& outDistSqr,FVector& outDashNormal)
{
    auto* PlayerChar =Cast<APlayerDiabloCharacter>(ActorInfo->AvatarActor.Get());
    
    if(!PlayerChar->GetFocusedTarget())
    {
        return false;
    }
    float AcceptRadius=70.f;
    
    FVector Location1 = PlayerChar->GetActorLocation();
    FVector Location2 = PlayerChar->GetFocusedTarget()->GetActorLocation();

    float DistSqr2D=FVector::DistSquared2D(Location1,Location2);
    outDistSqr=DistSqr2D;
    outDashNormal=(Location2 -Location1).GetSafeNormal();
    return DistSqr2D >m_fDashLimitRange*m_fDashLimitRange; 
}
