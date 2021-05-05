// My First Hack n Slash


#include "EquipmentActor.h"


#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

// Sets default values
AEquipmentActor::AEquipmentActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetActorEnableCollision(false);
	
}
