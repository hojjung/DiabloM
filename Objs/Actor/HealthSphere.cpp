// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthSphere.h"

#include "Characters/PlayerDiabloCharacter.h"
#include "Objs/Containers/QuadtreeNode.h"


AHealthSphere::AHealthSphere(const FObjectInitializer& objInit): Super(objInit)
{
    PrimaryActorTick.bCanEverTick=false;
    
    m_WidgetNameCard->SetHiddenInGame(true);
    m_WidgetNameCard->SetComponentTickEnabled(false);
    m_TagMagnitude = FGameplayTag::RequestGameplayTag(FName("Combat.Effect.HpHeal"));
    
    static ConstructorHelpers::FClassFinder<UGameplayEffect> FoundEffect(
            TEXT("Blueprint'/Game/Blueprints/Abilities/GameEffect/GE_PlayerHpHEal.GE_PlayerHpHeal_C'"));
    m_GEEffect = FoundEffect.Class;


    m_Particle=CreateDefaultSubobject<UParticleSystemComponent>("Particle00");
    m_Particle->SetupAttachment(RootComponent);
    m_Particle->SetRelativeLocation(FVector(0,0,25.f));
    m_Particle->SetRelativeScale3D(FVector(1.5f,1.5f,1.5f));

    static ConstructorHelpers::FObjectFinder<UParticleSystem> FoundParticle(
            TEXT("ParticleSystem'/Game/03_VisualEffect/FX/HpSphere/P_Summon_Parent_Startup.P_Summon_Parent_Startup'"));
    m_Particle->Template=FoundParticle.Object;
}

void AHealthSphere::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
    APlayerDiabloCharacter* Char = Cast<APlayerDiabloCharacter>(OtherActor);
    
    if (Char)
    {
        Interact(Char);
    }
}

void AHealthSphere::Interact(AActor* instigator)
{
    APlayerDiabloCharacter* Char = Cast<APlayerDiabloCharacter>(instigator);

    if (!Char)
    {
        return;
    }

    float HpHalf = Char->GetMaxHealth() / 2.f;

    FGameplayEffectContextHandle Context = Char->GetDiaAbilitySystem()->MakeEffectContext();
    FGameplayEffectSpecHandle EffectSpecHandle = Char->GetDiaAbilitySystem()->MakeOutgoingSpec(m_GEEffect, 1, Context);

    EffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(
        m_TagMagnitude, HpHalf);

    Char->GetDiaAbilitySystem()->ApplyGameplayEffectSpecToSelf(
        *EffectSpecHandle.Data);

    HideAll(true);

    if (GetCurrentNode())
    {
        GetCurrentNode()->RemoveElement(this);
    }

    if (m_OnTaskEnd.IsBound())
    {
        m_OnTaskEnd.Broadcast(this);
    }
    else
    {
        Destroy();
    }
}

void AHealthSphere::DropEnd()
{
    RegisterToQuadTreeBound();
}

void AHealthSphere::ShowAll(bool hasBeenShowed)
{
    if(m_bIsVisible)
    {
        return;	
    }
	
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    
    m_bIsVisible=true;
    m_Particle->SetComponentTickEnabled(true);
    m_Particle->Activate();
}

void AHealthSphere::HideAll(bool hasBeenShowed)
{
    if(!m_bIsVisible)
    {
        return;	
    }
    
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    
    m_bIsVisible=false;
    m_Particle->SetComponentTickEnabled(false);
    m_Particle->Deactivate();
}
