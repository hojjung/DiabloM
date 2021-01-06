#include "MonsterBaseMeleeAttack.h"
#include "AbilitySystem/Task/PlayMontageAndWaitForEvent.h"
#include "Characters/MonsterPawn.h"
#include "Characters/PlayerDiabloCharacter.h"

UMonsterBaseMeleeAttack::UMonsterBaseMeleeAttack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    m_TagTookPhysDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookPhysDmg"));
    m_TagTookFireDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookFireDmg"));
    m_TagTookElecDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookElecDmg"));
    m_TagTookIceDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookIceDmg"));
    m_TagTookPoisonDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookPoisonDmg"));
    m_TagEventEndAbility = FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility"));
    m_TagEventBaseAttack = FGameplayTag::RequestGameplayTag(FName("Combat.Ability.Skill.BaseAttack"));

    AbilityTags.AddTag(m_TagEventBaseAttack);
    ActivationOwnedTags.AddTag(m_TagEventBaseAttack);

    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Combat.Ability.Skill")));


    m_fAttackRange = 150.f;
    m_fAttackAngle = 50.f;
}

bool UMonsterBaseMeleeAttack::CheckAttackRange(const AActor* other) const
{
    if (!GetAvatarActorFromActorInfo())
    {
        PRINTF("Monster Avatr NULL");
        return false;
    }

    FVector const OtherLoc = other->GetActorLocation();
    FVector const MyLoc = GetAvatarActorFromActorInfo()->GetActorLocation();
    FVector const SelfToOther = OtherLoc - MyLoc;
    FVector const SelfToOtherDir = SelfToOther.GetSafeNormal();
    FVector const MyFacingDir = GetAvatarActorFromActorInfo()->GetActorRotation().Vector();

    bool bAngle = (SelfToOtherDir | MyFacingDir) >= m_fAttackAngleCos; //벡터의 내적

    float DistSqr = FVector::DistSquared2D(OtherLoc, MyLoc);
    bool bDist = DistSqr < m_fAttackRangeSqr;

    return bAngle && bDist;
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

    PlayAbilityAnimation(m_BaseAttackMotion, NAME_None, 1);
}

void UMonsterBaseMeleeAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);
    m_fAttackRange = m_OwnerUnit->GetAttackRange();
    m_fAttackRangeSqr = m_fAttackRange * m_fAttackRange;
    m_fAttackAngleCos = FMath::Cos(FMath::DegreesToRadians(m_fAttackAngle));
}

void UMonsterBaseMeleeAttack::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);
    m_MonsterPawn = Cast<AMonsterPawn>(m_OwnerUnit);
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
    if (EventTag == m_TagEventEndAbility)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }

    if (EventTag == m_TagEventBaseAttack)
    {
        const AUnitPawn* TargetChar=Cast<AUnitPawn>( EventData.Target);
        
        if (!m_OwnerUnit || !TargetChar ||!TargetChar->IsAlive() ||!CheckAttackRange(TargetChar))
        {
            //EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
            return;
        }

        FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
            DamageGameplayEffect, GetAbilityLevel());


        float PhysDmg = m_OwnerUnit->GetAttributeSet()->GetPhysicalDamage();
        DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookPhysDamage, PhysDmg);

        float FireDmg = m_OwnerUnit->GetAttributeSet()->GetAtkFire();
        DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookFireDamage, FireDmg);

        float ElecDmg = m_OwnerUnit->GetAttributeSet()->GetAtkElec();
        DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookElecDamage, ElecDmg);

        float PoisonDmg = m_OwnerUnit->GetAttributeSet()->GetAtkPoison();
        DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookPoisonDamage, PoisonDmg);

        float IceDmg = m_OwnerUnit->GetAttributeSet()->GetAtkCold();
        DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookIceDamage, IceDmg);

        auto* SourceAbili = m_OwnerUnit->GetDiaAbilitySystem();
        auto* TargetAbili = Cast<AUnitPawn>(EventData.Target)->GetDiaAbilitySystem();

        SourceAbili->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data, TargetAbili);
    }
}
