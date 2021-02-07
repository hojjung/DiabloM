// My First Hack n Slash


#include "EquipmentActor.h"


#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

// Sets default values
AEquipmentActor::AEquipmentActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorEnableCollision(false);
	
}

// Called when the game starts or when spawned
void AEquipmentActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEquipmentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AEquipmentActor* AEquipmentActor::SpawnToMesh(USkeletalMeshComponent* skBody, TSubclassOf<AEquipmentActor> equipActor, FName socket)
{
	if(!equipActor)
	{
		return nullptr;
	}

	FActorSpawnParameters Param;

	Param.bNoFail = true;

	AEquipmentActor* SpawnedActor = skBody->GetWorld()->SpawnActor<AEquipmentActor>(equipActor, Param);

	FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
                                   EAttachmentRule::SnapToTarget, false);

	SpawnedActor->AttachToComponent(skBody, Rule, socket);
	
	return SpawnedActor;
}

