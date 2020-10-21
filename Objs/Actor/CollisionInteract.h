// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Objs/Interfaces/Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "CollisionInteract.generated.h"

UCLASS()
class DIABLOM_API ACollisionInteract : public AActor,public IInteractable
{
	GENERATED_BODY()
	

public:
	ACollisionInteract(const FObjectInitializer& objInit);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	UStaticMeshComponent* m_MeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	USphereComponent* m_CollSphere;
	UPROPERTY(VisibleAnywhere, Category = "Item")
	UWidgetComponent* m_BillBoard;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Interact(AActor* instigator) override;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};


