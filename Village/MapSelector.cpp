// My First Hack n Slash


#include "MapSelector.h"

#include "Characters/PlayerDiabloCharacter.h"

AMapSelector::AMapSelector()
{
    m_MeshMapSelector= CreateDefaultSubobject<UStaticMeshComponent>("Mesh00");
    m_MeshMapSelector->SetupAttachment(RootComponent);
}

void AMapSelector::Interact(AActor* instigator)
{
    Cast<ADiabloPlayerController>( ( Cast<APlayerDiabloCharacter>( instigator)->GetController()))->ShowWorldMap();
}
