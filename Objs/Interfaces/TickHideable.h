// My First Hack n Slash

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TickHideable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,Blueprintable)
class UTickHideable : public UInterface
{
	GENERATED_BODY()
};

class QuadtreeNode;

class DIABLOM_API ITickHideable
{
	GENERATED_BODY()

public:
	virtual FVector GetActorLocation() =0;

	virtual void RegisterToQuadTreeBound()=0;

	virtual void ShowAll() =0;
	
	virtual void HideAll() =0;
	
	virtual void SetNode(QuadtreeNode* quadtree_node)=0;

	virtual QuadtreeNode* GetCurrentNode()=0;

};
