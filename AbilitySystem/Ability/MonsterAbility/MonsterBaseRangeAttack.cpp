#include "MonsterBaseRangeAttack.h"

#include "AbilitySystem/Ability/AbilitySkillActors/AbilityProjectile.h"
#include "AbilitySystem/Task/PlayMontageAndWaitForEvent.h"
#include "Characters/MonsterPawn.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"

UMonsterBaseRangeAttack::UMonsterBaseRangeAttack()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    m_TagTookPhysDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookPhysDmg"));
    m_TagTookFireDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookFireDmg"));
    m_TagTookElecDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookElecDmg"));
    m_TagTookIceDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookIceDmg"));
    m_TagTookPoisonDamage = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.TookPoisonDmg"));
    m_TagEventEndAbility = FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility"));
    m_TagEventBaseAttack = FGameplayTag::RequestGameplayTag(FName("Event.Montage.SpawnProjectile"));

    AbilityTags.AddTag(m_TagEventBaseAttack);
    ActivationOwnedTags.AddTag(m_TagEventBaseAttack);

    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Combat.Ability.Skill")));


    m_fAttackRange = 150.f;
    m_fAttackAngle = 50.f;
}

bool UMonsterBaseRangeAttack::CheckAttackRange(const AActor* other) const
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

void UMonsterBaseRangeAttack::PlayAbilityAnimation(UAnimMontage* MontageToPlay, FName playSection, float AttackSpeed)
{
    UPlayMontageAndWaitForEvent* Task = UPlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
        this, NAME_None, MontageToPlay, FGameplayTagContainer(), AttackSpeed,
        playSection, false, 1.0f);

    Task->OnBlendOut.AddDynamic(this, &UMonsterBaseRangeAttack::OnCompleted);
    Task->OnCompleted.AddDynamic(this, &UMonsterBaseRangeAttack::OnCompleted);
    Task->OnInterrupted.AddDynamic(this, &UMonsterBaseRangeAttack::OnCancelled);
    Task->OnCancelled.AddDynamic(this, &UMonsterBaseRangeAttack::OnCancelled);
    Task->EventReceived.AddDynamic(this, &UMonsterBaseRangeAttack::EventReceived);

    Task->ReadyForActivation();
}

void UMonsterBaseRangeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    }

    float AttackSpeed = m_MonsterPawn->GetAttackSpeed();
    
    PlayAbilityAnimation(m_BaseAttackMotion, NAME_None, AttackSpeed);
}

void UMonsterBaseRangeAttack::CreateBulletPool(const FGameplayAbilityActorInfo* ActorInfo)
{
    m_nBulletIndexForPull=0;
    m_AryMissle.Reset();
    int i=0;
    FVector Location = ActorInfo->OwnerActor->GetActorLocation();
    
    FRotator Rot(0.f,0.f,0.f);
    FActorSpawnParameters Param;
    Param.bNoFail = true;
    Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    while(i++<3)
    {
        AAbilityProjectile* SpawnedBullet = m_MonsterPawn->GetWorld()->SpawnActor<AAbilityProjectile>(
            m_ClassBullet, Location, Rot, Param);
        
        m_AryMissle.Emplace(SpawnedBullet);
    }
}

void UMonsterBaseRangeAttack::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);
    m_fAttackRange = m_OwnerUnit->GetAttackRange();
    m_fAttackRangeSqr = m_fAttackRange * m_fAttackRange;
    m_fAttackAngleCos = FMath::Cos(FMath::DegreesToRadians(m_fAttackAngle));
    //
    CreateBulletPool(ActorInfo);
}

void UMonsterBaseRangeAttack::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);
    m_MonsterPawn = Cast<AMonsterPawn>(m_OwnerUnit);
}

void UMonsterBaseRangeAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMonsterBaseRangeAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMonsterBaseRangeAttack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
    if (EventTag ==m_TagEventEndAbility)
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
        return;
    }
  
    if ( EventTag == m_TagEventBaseAttack)
    {
        AMonsterPawn* Mob = Cast<AMonsterPawn>(GetAvatarActorFromActorInfo());
        
        if (!Mob)
        {
            EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
            return;
        }
        
        FTransform MuzzleTransform = Mob->GetActorTransform();

        FRotator Rotation = Mob->GetActorForwardVector().Rotation();
        
        MuzzleTransform.SetRotation(Rotation.Quaternion());
        
        MuzzleTransform.SetScale3D(FVector(1.0f));
    
        AAbilityProjectile* Projectile = GetBullet();

        Projectile->SetActorRotation(Rotation);
        
        Projectile->SetActorLocation(Mob->GetActorLocation());

        //Projectile->SetActorTransform(MuzzleTransform);
        
        FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, Mob->GetCharacterLevel());

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

        FGameplayEffectContextHandle Context = DamageEffectSpecHandle.Data->GetEffectContext();
	
        Context.AddInstigator(GetActorInfo().AvatarActor.Get(),Projectile);

        DamageEffectSpecHandle.Data->SetContext(Context);

        Projectile->ShootStart(DamageEffectSpecHandle);
        
    }
}

AAbilityProjectile* UMonsterBaseRangeAttack::GetBullet()
{
    if(m_nBulletIndexForPull>=m_AryMissle.Num())
    {
        m_nBulletIndexForPull=0;
    }
    
    return m_AryMissle[m_nBulletIndexForPull++];
}
