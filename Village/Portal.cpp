
#include "Portal.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"


APortal::APortal(const FObjectInitializer& objInit):Super(objInit)
{
    m_ParticleEffect=CreateDefaultSubobject<UParticleSystemComponent>("ParticleEffect00");
    m_ParticleEffect->SetupAttachment(RootComponent);
    
}

void APortal::BeginPlay()
{
    Super::BeginPlay();

    UDungeonManager* DungeonM = UDiabloGameInstance::Get->GetDungeonManager();

    DungeonM->GetOnDungeonCreate().AddUObject(this,&APortal::SetPortal);
    SetPortal(false);
}

void APortal::SetPortal(bool bIsOn)
{
    if(bIsOn)
    {
        PRINTF("IsOn");
        m_ParticleEffect->SetHiddenInGame(false);
        m_ParticleEffect->Activate();
        m_Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }
    else
    {
        PRINTF("IsOff");
        m_ParticleEffect->SetHiddenInGame(true);
        m_ParticleEffect->Deactivate();
        m_Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void APortal::Interact(AActor* instigator)
{
    UDungeonManager* DungeonM = UDiabloGameInstance::Get->GetDungeonManager();

    if(!DungeonM->IsDungeonOpened())
    {
        return;
    }

    DungeonM->PortalToRecentDungeon();
}
