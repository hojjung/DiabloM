// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "QuadtreeNode.h"
#include "SharedPointer.h"

/**
 * 
 */
class DIABLOM_API Quadtree
{
private:
    Quadtree();
public:
    Quadtree(const int maxDistance, FVector2D min, FVector2D max);
    ~Quadtree();

    TSharedPtr<QuadtreeNode> GetRootNode();

    void AddElement(ITickHideable* platform);

    void DrawBoxes(UWorld* world);

    TSharedPtr<QuadtreeNode> GetNode(FVector position);

    TSharedPtr<QuadtreeNode> GetMinNode(FVector2D position);


    int GetMaxDistance() const;

    void TickTryShowActors(FVector&& position);

    
private:
    void InitialiseNodes(TSharedPtr<QuadtreeNode> parentNode, FVector2D min, FVector2D max);

    TSharedPtr<QuadtreeNode> CreateNode(TSharedPtr<QuadtreeNode> parent, FVector2D min, FVector2D max);
    
private:
    TSharedPtr<QuadtreeNode> m_RootNode;

    int m_MaxDistance;

    TSharedPtr<QuadtreeNode> m_CurrentNode;

    TSharedPtr<QuadtreeNode> m_OldNode;
};
