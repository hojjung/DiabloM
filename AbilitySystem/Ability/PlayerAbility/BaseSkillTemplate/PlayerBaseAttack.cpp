#include "PlayerBaseAttack.h"

#include "AbilitySystem/Task/PlayMontageAndWaitForEvent.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/UnitPawn.h"

UPlayerBaseAttack::UPlayerBaseAttack()
{
    m_fDuringMoveSpeedModifier=0.25f;
    
    m_fDashLimitSqr=0.f;
    
    m_nSectionIndex = -1;

    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    
    m_TagTookPhysDamage  = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookPhysDmg"));
    m_TagTookFireDamage  = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookFireDmg"));
    m_TagTookElecDamage  = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookElecDmg"));
    m_TagTookIceDamage   = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookIceDmg"));
    m_TagTookPoisonDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookPoisonDmg"));
    
    m_TagEventEndAbility= FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility"));
    m_TagEventBaseAttack= FGameplayTag::RequestGameplayTag(FName("Combat.Ability.Skill.BaseAttack"));
    
    AbilityTags.AddTag(m_TagEventBaseAttack);
    
    ActivationOwnedTags.AddTag(m_TagEventBaseAttack);

    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Combat.Ability.Skill")));

    m_fDashTime=0.5f;
    
    m_AbilityInputID = EAbilityInputID::BaseAttack;

    m_AbilityID = EAbilityInputID::BaseAttack;

    m_fAttackAngle = 45.f;

    m_fAttackRange = 250.f;    
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

void UPlayerBaseAttack::TryDashAttack(const FGameplayAbilityActorInfo* ActorInfo)
{
    float AttackSpeed = m_PlayerPawn->GetAttackSpeed();
    PRINTF("AttackSpeed:%f",AttackSpeed);
    
    auto* Movement=GetMovement(m_PlayerPawn);
    
    Movement->SetMoveSpeedRatio(m_fDuringMoveSpeedModifier);
    
    float DistSqred;
    
    FVector DashNormal;
    
    if(IsDashable(ActorInfo,DistSqred,DashNormal))
    {
        PlayAbilityAnimation(m_BaseAttackMotion,"DashAttack" ,2);

        float DashLength = DistSqred - m_fDashAcceptSqr;

        DashLength+=22500.f;

        float DashPercent = DashLength/DistSqred;

        DashAttack(Movement,DashNormal,FMath::Sqrt(DashLength),(m_fDashTime*DashPercent));
    }
    else
    {
        PlayAbilityAnimation(m_BaseAttackMotion,GetSectionName() ,AttackSpeed);
    }
}

void UPlayerBaseAttack::TryNormalAttack()
{
    float AttackSpeed = m_PlayerPawn->GetAttackSpeed();
    
    auto* Movement=GetMovement(m_PlayerPawn);
    
    Movement->SetMoveSpeedRatio(m_fDuringMoveSpeedModifier);
    
    PlayAbilityAnimation(m_BaseAttackMotion,GetSectionName() ,AttackSpeed);
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
    if(!m_BaseAttackMotion)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }

}

void UPlayerBaseAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    if(m_BaseAttackMotion)
        m_AryMontageSections = m_BaseAttackMotion->GetArySections();
    
    ResetComboSection();
    
    //m_fAttackRange = m_OwnerUnit->GetAttackRange();
    m_fAttackAngleCos = FMath::Cos(FMath::DegreesToRadians(m_fAttackAngle));
    
    m_fAttackRangeSqr = m_fAttackRange * m_fAttackRange;
    
    float Accept = m_PlayerPawn->GetAcceptRadiusToOther() + m_fAttackRange;

    m_fDashAcceptSqr = m_fAttackRangeSqr;//너무 가까우면 안함
    
    m_fDashLimitSqr = Accept*Accept;
}

void UPlayerBaseAttack::ResetComboSection()
{
    m_nSectionIndex = 0;
}

void UPlayerBaseAttack::OnCancelledWrapper()
{
    OnCancelled(FGameplayTag(),FGameplayEventData());
}

void UPlayerBaseAttack::OnCompletedWrapper()
{
    OnCompleted(FGameplayTag(),FGameplayEventData());
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
    const AUnitPawn* TargetChar=Cast<AUnitPawn>( EventData.Target);
    APlayerDiabloCharacter* PlayerChar=Cast<APlayerDiabloCharacter>(GetAvatarActorFromActorInfo());
    
    if (EventTag == m_TagEventEndAbility)
    {
        ResetComboSection();
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }
    
    if (EventTag == m_TagEventBaseAttack)
    {
       
        if (!PlayerChar||!TargetChar||!TargetChar->IsAlive())
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
            return;
        }

        //SandBox
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

UUnitMovement* UPlayerBaseAttack::GetMovement(APawn* want)
{
    return Cast<UUnitMovement>(want->GetMovementComponent());
}

void UPlayerBaseAttack::DashAttack(UUnitMovement* movementComp,FVector dashNormal, float dashLength, float dashTime)
{
    FVector DeltaDash=dashNormal * (dashLength/m_fDashTime);
    
    movementComp->SetDash(DeltaDash,dashTime);
}

bool UPlayerBaseAttack::IsDashable(const FGameplayAbilityActorInfo* ActorInfo,float& outDistSqr,FVector& outDashNormal)
{
    auto* PlayerChar =Cast<APlayerDiabloCharacter>(ActorInfo->AvatarActor.Get());
    
    if(!PlayerChar->GetFocusedTarget())
    {
        return false;
    }
    
    FVector Location1 = PlayerChar->GetActorLocation();
    
    FVector Location2 = PlayerChar->GetFocusedTarget()->GetActorLocation();

    float DistSqr2D=FVector::DistSquared2D(Location1,Location2);
    
    outDistSqr=DistSqr2D;
    
    outDashNormal=(Location2 -Location1).GetSafeNormal();
    
    return DistSqr2D >m_fDashLimitSqr; 
}

bool UPlayerBaseAttack::CheckAttackRange(const AActor* other) const
{
    if (!GetAvatarActorFromActorInfo())
    {
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

bool UPlayerBaseAttack::DealDamageToTarget(const AUnitPawn* TargetChar, APlayerDiabloCharacter* PlayerChar)
{
    FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
            m_GETargetDamage, GetAbilityLevel());

    float Rate = m_fLevelPerDamageRate * GetAbilityLevel();

    float PhysDmg=PlayerChar->GetAttributeSet()->GetPhysicalDamage();
    DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookPhysDamage,PhysDmg*Rate);

    float FireDmg=PlayerChar->GetAttributeSet()->GetAtkFire();
    DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookFireDamage,FireDmg*Rate);

    float ElecDmg=PlayerChar->GetAttributeSet()->GetAtkElec();
    DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookElecDamage,ElecDmg*Rate);

    float PoisonDmg=PlayerChar->GetAttributeSet()->GetAtkPoison();
    DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookPoisonDamage,PoisonDmg*Rate);

    float IceDmg=PlayerChar->GetAttributeSet()->GetAtkCold();
    DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(m_TagTookIceDamage,IceDmg*Rate);

    FActiveGameplayEffectHandle Result = PlayerChar->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToTarget(
        *DamageEffectSpecHandle.Data, TargetChar->GetDiaAbilitySystem());

    return Result.bPassedFiltersAndWasExecuted;
}

void UPlayerBaseAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    GetMovement(Cast<APawn>( CurrentActorInfo->AvatarActor.Get()))->SetMoveSpeedRatio(1.f);
}
