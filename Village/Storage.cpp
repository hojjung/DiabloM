#include "Storage.h"

#include "Characters/DiabloPlayerController.h"

AStorage::AStorage(const FObjectInitializer& objInit):Super(objInit)
{
    m_MeshStorage = CreateDefaultSubobject<UStaticMeshComponent>("Mesh00");
    FAttachmentTransformRules Trans = FAttachmentTransformRules(EAttachmentRule::KeepRelative,false);
    m_MeshStorage->AttachToComponent(m_Capsule,Trans);
}

void AStorage::Interact(AActor* instigator)
{
    TWeakObjectPtr<ADiabloPlayerController> DiaPC = ADiabloPlayerController::Get;
    DiaPC->GetMainCanvas()->OpenStorageMenu();

}

