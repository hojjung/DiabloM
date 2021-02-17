// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "Objs/Actor/CollisionInteract.h"
#include "DestroyActorJar.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API ADestroyActorJar : public ACollisionInteract
{
	GENERATED_BODY()
public:
	virtual FVector GetActorLocation() override ;

	virtual void RegisterToQuadTreeBound() override ;

	virtual void ShowAll() override ;
	
	virtual void HideAll() override ;

	virtual void SetNode(QuadtreeNode* quadtree_node) override;

	virtual QuadtreeNode* GetCurrentNode() override;
};
