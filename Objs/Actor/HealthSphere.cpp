// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthSphere.h"

#include "Characters/PlayerDiabloCharacter.h"


AHealthSphere::AHealthSphere(const FObjectInitializer& objInit):Super(objInit)
{
    m_BillBoard->SetVisibility(false);
    m_TagMagnitude = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.HpHeal"));
}

void AHealthSphere::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
    APlayerDiabloCharacter* Char = Cast<APlayerDiabloCharacter>(OtherActor);

    if (!Char)
    {
        return;
    }

    float HpHalf=Char->GetMaxHealth()/2.f;

    FGameplayEffectContextHandle Context = Char->GetDiaAbilitySystem()->MakeEffectContext();
    FGameplayEffectSpecHandle EffectSpecHandle = Char->GetDiaAbilitySystem()->MakeOutgoingSpec(m_GEEffect, 1,Context);

    EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(
        m_TagMagnitude, HpHalf);

   Char->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToSelf(
        *EffectSpecHandle.Data);


    SetActorHiddenInGame(true);
    Destroy();
}
