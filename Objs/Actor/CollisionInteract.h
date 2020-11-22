// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Objs/Interfaces/Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Objs/Interfaces/TickHideable.h"

#include "CollisionInteract.generated.h"

class ACollisionInteract;
DECLARE_MULTICAST_DELEGATE_OneParam(FCollTaskEnd,ACollisionInteract*);
UCLASS()
class DIABLOM_API ACollisionInteract : public AActor,public IInteractable, public ITickHideable
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

	FCollTaskEnd m_OnTaskEnd;

	QuadtreeNode* m_CurrentNode;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Interact(AActor* instigator) override;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	FORCEINLINE FCollTaskEnd& GetOnTaskEnd()
	{
		return m_OnTaskEnd;
	}

public:
	virtual FVector GetActorLocation() override ;

	virtual void RegisterToQuadTreeBound() override ;

	virtual void ShowAll(bool hasBeenShowed) override ;
	
	virtual void HideAll(bool hasBeenShowed) override ;

	virtual void SetNode(QuadtreeNode* quadtree_node) override;

	virtual QuadtreeNode* GetCurrentNode() override;
};


