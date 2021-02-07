// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipmentActor.generated.h"

UCLASS()
class DIABLOM_API AEquipmentActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquipmentActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static AEquipmentActor*  SpawnToMesh(USkeletalMeshComponent* skBody,TSubclassOf<AEquipmentActor> equipActor,FName socket);
};
