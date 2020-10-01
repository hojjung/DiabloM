// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponMelee.h"

#include "NavArea_Obstacle.h"
#include "Characters/PlayerDiabloCharacter.h"

AWeaponMelee::AWeaponMelee()
{
    m_MeleeCollison =CreateDefaultSubobject<UCapsuleComponent>("Coll00");
    m_MeleeCollison->SetupAttachment(RootComponent);
    m_MeleeCollison->SetCapsuleRadius(22);
    m_MeleeCollison->SetCapsuleHalfHeight(64);
    m_MeleeCollison->AreaClass=UNavArea_Obstacle::StaticClass();
    m_MeleeCollison->SetCollisionProfileName("OverlapOnlyPawn");
    m_MeleeCollison->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //m_MeleeCollison->SetHiddenInGame(true);
    m_MeleeCollison->SetCastShadow(false);
    m_bIsAttacking=false;
    m_CachedAttackEvent=FGameplayTag::RequestGameplayTag("Ability.BaseAttack");
}

void AWeaponMelee::InitWeapon(AUnitPawn* pl, const FItemInstance* itemInst)
{   
    Super::InitWeapon(pl,itemInst);
    m_MeleeCollison->IgnoreActorWhenMoving(m_User.Get(),true);
    m_MeleeCollison->IgnoreComponentWhenMoving(m_User.Get()->GetCapsule(),true);
    
    m_StartDeleHandle=pl->m_OnStartAttack.AddUObject(this,&AWeaponMelee::BeginWeaponAttack);
    m_EndDeleHandle=pl->m_OnEndAttack.AddUObject(this,&AWeaponMelee::EndWeaponAttack);
    m_MeleeCollison->OnComponentBeginOverlap.AddDynamic(this,&AWeaponMelee::OnOverlapWeapon);
}

void AWeaponMelee::OnOverlapWeapon(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if(m_User.Get() == OtherActor)
    {
        return;
    }
    if (m_AlreadyHittenForIgnore.Contains(OtherActor))
    {
        return;
    }
    
    m_AlreadyHittenForIgnore.Add(OtherActor);

    FGameplayEventData EventData;
    EventData.Instigator = m_User.Get();
    EventData.Target = OtherActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(m_User.Get(),m_CachedAttackEvent,EventData);
}

void AWeaponMelee::RemoveWeapon(AUnitPawn* pl)
{
    pl->m_OnStartAttack.Remove(m_StartDeleHandle);
    pl->m_OnStartAttack.Remove(m_EndDeleHandle);
}

void AWeaponMelee::BeginWeaponAttack()
{
    m_bIsAttacking=true;
    m_MeleeCollison->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeaponMelee::EndWeaponAttack()
{
    m_bIsAttacking=false;
    m_MeleeCollison->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //PRINTF("Attack End %d",m_AlreadyHittenForIgnore.Num());
    m_AlreadyHittenForIgnore.Reset();
}
