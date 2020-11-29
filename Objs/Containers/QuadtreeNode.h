// My First Hack n Slash

#pragma once

#include "DiabloM.h"
#include "SharedPointer.h"
#include "WeakObjectPtr.h"
#include "Containers/Array.h"
#include "Objs/Interfaces/TickHideable.h"

enum ENodePosition
{
    BottomRight = 0,
    BottomLeft = 1,
    TopRight = 2,
    TopLeft = 3,
    Root = 4
};

class DIABLOM_API QuadtreeNode
{
public:
    QuadtreeNode();
    QuadtreeNode(const QuadtreeNode& copy);
    ~QuadtreeNode();

    /** Set the bounding box for this node. */
    void SetBoundingBox(const FVector2D min, const FVector2D max);

    /** Get the bounding box for this node. */
    TSharedPtr<FBox2D> GetBoundingBox();

    /** Set the parent node for this node. */
    void SetParentNode(TSharedPtr<QuadtreeNode> parentNode);

    /** Get the parent node to this node. */
    TSharedPtr<QuadtreeNode> GetParentNode();

    /* Add a child node to this node. */
    void AddChildNode(TSharedPtr<QuadtreeNode> node);

    /** Get child nodes belonging to this node. */
    TArray<TSharedPtr<QuadtreeNode>> GetChildNodes();

    /** Add an element to this node. */
    void AddElement(ITickHideable* element);

    void RemoveElement(ITickHideable* element);

    /** Get elements belonging to this node. */
    TArray<ITickHideable*> GetElements();

    /** Draw a debug box to outline where the box is. */
    void DrawBoxAroundNode(UWorld* world, FColor colour);

    /** Get the distance from the root node of the tree. */
    int GetDistance();

    /** Find if a given position is inside this node. */
    bool IsPositionInsideNode(FVector position);

    /** Get the node which contains a given position. */
    TSharedPtr<QuadtreeNode> GetNode(FVector position);

    /** Get all elements belonging to this node and any elements which are contained within it's children. */
    TArray<ITickHideable*> GetAllElements();

    /** Get a copy of the node position member variable. */
    ENodePosition GetNodePosition() const;

    /** Set the value of a node position member variable. */
    void SetNodePosition(ENodePosition position);

    /** Check if this node has any children nodes. */
    bool HasChildNodes() const;

    /** Check if this node has any elements. */
    bool HasElements() const;
    
    void HideActors();
    
    void ShowActors();

    TSharedPtr<QuadtreeNode> GetMinNode(FVector2D position);
private:

    /** Get the root node of this quadtree. */
    TSharedPtr<QuadtreeNode> GetRootNode();

    /** Get the node which contains a given position. */

    /** Acquire the nearest platform to a position. */
    ITickHideable* GetNearestActor(FVector2D position);
private:
    /** The bounding box for this node. */
    TSharedPtr<FBox2D> m_BoundingBox;

    /** The parent node to this node */
    TSharedPtr<QuadtreeNode> m_ParentNode;

    /** Elements held within this node. */
    TArray<ITickHideable*> m_Elements;

    /** Child nodes held by this node. */
    TArray<TSharedPtr<QuadtreeNode>> m_ChildNodes;

    

    /** The position of this node relevant to it's parent. */
    ENodePosition m_eNodePosition;

    /** The chance that there is to spawn an enemy on a node. */

    bool m_bHasBeenShowed;

    bool m_bVisible;

public:
    TArray<TSharedPtr<QuadtreeNode>> m_Siblings;
};

