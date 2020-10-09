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

    m_fAttackRange=150.f;
    m_fAttackAngle=50.f;
   
}
bool UMonsterBaseMeleeAttack::CheckAttackRange(const AActor* other) const
{
    if(!GetAvatarActorFromActorInfo())
    {
        PRINTF("Monster Avatr NULL");
        return false;
    }
    
    FVector const OtherLoc = other->GetActorLocation();
    FVector const MyLoc = GetAvatarActorFromActorInfo()->GetActorLocation();
    FVector const SelfToOther = OtherLoc - MyLoc;
    FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();
    FVector const MyFacingDir = GetAvatarActorFromActorInfo()->GetActorRotation().Vector();

    bool bAngle= (SelfToOtherDir | MyFacingDir) >= m_fAttackAngleCos;//벡터의 내적

    float DistSqr=FVector::DistSquared2D(OtherLoc,MyLoc);
    bool bDist =  DistSqr< m_fAttackRangeSqr;

    return bAngle&&bDist;
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
    m_fAttackRangeSqr=m_fAttackRange*m_fAttackRange;
    m_fAttackAngleCos=FMath::Cos(FMath::DegreesToRadians(m_fAttackAngle));
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
        if (!MonsterAttacker ||!EventData.Target||!CheckAttackRange(EventData.Target))
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
            return;
        }

        FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
            DamageGameplayEffect, GetAbilityLevel());

        DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(
            FGameplayTag::RequestGameplayTag(FName("Data.Combat.TookDamage")), MonsterAttacker->GetAttributeSet()->GetPhysicalDamage());

        Cast<AUnitPawn>(EventData.Target)->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToSelf(
            *DamageEffectSpecHandle.Data);
    }
}
