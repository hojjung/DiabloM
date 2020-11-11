#include "Storage.h"

#include "Characters/DiabloPlayerController.h"

AStorage::AStorage(const FObjectInitializer& objInit):Super(objInit)
{
    m_MeshStorage = CreateDefaultSubobject<UStaticMeshComponent>("Mesh00");
    m_MeshStorage->SetupAttachment(m_Capsule);
}

void AStorage::Interact(AActor* instigator)
{
    TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
    DiaPC->ShowStorageMenu();
}

