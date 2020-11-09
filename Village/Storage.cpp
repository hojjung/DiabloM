#include "Storage.h"

#include "Characters/DiabloPlayerController.h"

AStorage::AStorage()
{
    m_MeshStorage = CreateDefaultSubobject<UStaticMeshComponent>("Mesh00");
    m_MeshStorage->SetupAttachment(RootComponent);
}

void AStorage::Interact(AActor* instigator)
{
    TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
    DiaPC->ShowStorageMenu();
}

