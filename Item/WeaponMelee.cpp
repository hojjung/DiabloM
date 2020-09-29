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
}

void AWeaponMelee::InitWeapon(APlayerDiabloCharacter* pl, const FItemInstance* itemInst)
{   
    Super::InitWeapon(pl,itemInst);
    m_MeleeCollison->IgnoreActorWhenMoving(m_User,true);
}

void AWeaponMelee::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    
    if (m_AlreadyHittenForIgnore.Contains(OtherActor))
    {
        return;
    }
    m_AlreadyHittenForIgnore.Add(OtherActor);

    FGameplayEventData EventData;
    EventData.Instigator = m_User;
    EventData.Target = OtherActor;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(m_User,m_CachedAttackEvent,EventData);
}

void AWeaponMelee::BeginWeaponAttack(FGameplayTag tag)
{
    m_bIsAttacking=true;
    m_MeleeCollison->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    m_CachedAttackEvent=tag;
}

void AWeaponMelee::EndWeaponAttack()
{
    m_bIsAttacking=false;
    m_MeleeCollison->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_AlreadyHittenForIgnore.Reset();
}
