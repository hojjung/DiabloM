#include "QuadtreeNode.h"
#include "DrawDebugHelpers.h"
#include "GenericPlatformMath.h"

QuadtreeNode::QuadtreeNode(): m_eNodePosition(), m_bHasBeenShowed(false)
{
    m_Elements.Reserve(100);
}

/** Shallow copies a QuadtreeNode. */
QuadtreeNode::QuadtreeNode(const QuadtreeNode& copy)
{
    SetParentNode(copy.m_ParentNode);
    m_BoundingBox = copy.m_BoundingBox;
    m_Elements = copy.m_Elements;
    m_ChildNodes = copy.m_ChildNodes;
    m_bHasBeenShowed = false;
}

QuadtreeNode::~QuadtreeNode()
{
}

void QuadtreeNode::SetBoundingBox(const FVector2D min, const FVector2D max)
{
    m_BoundingBox = MakeShared<FBox2D>(min, max);
}

TSharedPtr<FBox2D> QuadtreeNode::GetBoundingBox()
{
    return m_BoundingBox;
}

void QuadtreeNode::SetParentNode(TWeakPtr<QuadtreeNode> parentNode)
{
    this->m_ParentNode = parentNode;
}

TWeakPtr<QuadtreeNode> QuadtreeNode::GetParentNode()
{
    return m_ParentNode;
}

void QuadtreeNode::AddChildNode(TSharedPtr<QuadtreeNode> node)
{
    m_ChildNodes.Add(node);
}

TArray<TSharedPtr<QuadtreeNode>> QuadtreeNode::GetChildNodes()
{
    return m_ChildNodes;
}

void QuadtreeNode::AddElement(ITickHideable* element)
{
    if(element->GetCurrentNode())
    {
        element->GetCurrentNode()->RemoveElement(element);
        element->SetNode(nullptr);
    }
    // Exlpore each child belonging to this node
    for (auto childNode : m_ChildNodes)
    {
        FVector2D position = {element->GetActorLocation().X, element->GetActorLocation().Y};
        // if the position lies within the bounding box of this child
        if (childNode->m_BoundingBox->IsInside(position))
        {
            // If the child has children of it's own
            if (childNode->HasChildNodes())
            {
                // Explore their children nodes to get the deepest position in the tree
                childNode->AddElement(element);
                return;
            }
                // If this node has no children, we can't be any more accurate
            else
            {
                childNode->m_Elements.Add(element);
                element->SetNode(childNode.Get());
                return;
            }
        }
    }

    // Wasn't inside a child node, probably on a boundary, must be in this node.
    m_Elements.Add(element);
    element->SetNode(this);
}

void QuadtreeNode::RemoveElement(ITickHideable* element)
{
    m_Elements.Remove(element);
}


TArray<ITickHideable*> QuadtreeNode::GetElements()
{
    return m_Elements;
}

void QuadtreeNode::DrawBoxAroundNode(UWorld* world, FColor colour)
{
    FVector centre = {m_BoundingBox->GetCenter().X, m_BoundingBox->GetCenter().Y, 0.0f};
    FVector extent = {m_BoundingBox->GetExtent().X, m_BoundingBox->GetExtent().Y, 0.0f};

    DrawDebugBox(world, centre, extent, colour);

    for (TSharedPtr<QuadtreeNode, ESPMode::Fast>& node : m_ChildNodes)
    {
        node->DrawBoxAroundNode(world, colour);
    }
}

int QuadtreeNode::GetDistance()
{
    int i = 0;

    TWeakPtr<QuadtreeNode> parent = GetParentNode();
    while (parent.IsValid() && parent != nullptr)
    {
        i++;
        TSharedPtr<QuadtreeNode> pinnedObserver(parent.Pin());
        parent = pinnedObserver->GetParentNode();
    }

    return i;
}

bool QuadtreeNode::PositionInsideNode(FVector position)
{
    FVector2D pos2d = {position.X, position.Y};
    for (auto childNode : m_ChildNodes)
    {
        if (childNode->m_BoundingBox->IsInside(pos2d))
        {
            return childNode->PositionInsideNode(position);
        }
    }

    if (m_BoundingBox->IsInside(pos2d))
    {
        return true;
    }

    return false;
}

TSharedPtr<QuadtreeNode> QuadtreeNode::GetNode(FVector position)
{
    for (auto childNode : m_ChildNodes)
    {
        if (childNode->PositionInsideNode(position))
        {
            return childNode;
        }
    }

    return nullptr;
}

TArray<ITickHideable*> QuadtreeNode::GetAllElements()
{
    TArray<ITickHideable*> result;

    for (auto childNode : GetChildNodes())
    {
        result.Append(childNode->GetAllElements());
    }

    result.Append(m_Elements);

    return result;
}

ENodePosition QuadtreeNode::GetNodePosition() const
{
    return m_eNodePosition;
}

void QuadtreeNode::SetNodePosition(ENodePosition position)
{
    m_eNodePosition = position;
}

bool QuadtreeNode::HasChildNodes() const
{
    return m_ChildNodes.Num() > 0;
}

bool QuadtreeNode::HasElements() const
{
    return m_Elements.Num() > 0;
}

void QuadtreeNode::HideActors()
{
    if (!HasChildNodes())
    {
        for (ITickHideable* Eles : GetAllElements())
        {
            Eles->HideAll(m_bHasBeenShowed);
        }
    }
    else
    {
        for (auto childNode : GetChildNodes())
        {
            childNode->HideActors();
        }
    }
}

void QuadtreeNode::ShowActors()
{
    if (!HasChildNodes())
    {
        
        if (!m_bHasBeenShowed)
        {
            m_bHasBeenShowed = true;
        }
        
        for (ITickHideable* Eles : GetAllElements())
        {
            Eles->ShowAll(m_bHasBeenShowed);
        }
    }
    else
    {
        for (auto childNode : GetChildNodes())
        {
            childNode->ShowActors();
        }
    }

}

TSharedPtr<QuadtreeNode> QuadtreeNode::GetRootNode()
{
    TSharedPtr<QuadtreeNode> parentNode = TSharedPtr<QuadtreeNode>(GetParentNode().Pin());

    // If the parent node isn't nullptr
    if (parentNode.IsValid())
    {
        // Grab an instance of the grandparent
        TSharedPtr<QuadtreeNode> parentOfParent = TSharedPtr<QuadtreeNode>(parentNode->GetParentNode().Pin());

        // If grandparent is not nullptr
        if (parentOfParent.IsValid())
        {
            // Recursively search the generation above
            return parentNode->GetRootNode();
        }
        else
        {
            // Found the root node, return the immediate parent to this node
            return parentNode;
        }
    }

    // Couldn't find a valid parent for whatever reason. Maybe only 2 layers?
    return nullptr;
}

TSharedPtr<QuadtreeNode> QuadtreeNode::GetMinNode(FVector2D position)
{
    // Exlpore each child belonging to this node
    for (auto childNode : m_ChildNodes)
    {
        // if the position lies within the bounding box of this child
        if (childNode->m_BoundingBox->IsInside(position))
        {
            // If the child has children of it's own
            if (childNode->HasChildNodes())
            {
                // Explore their children nodes to get the deepest position in the tree
                TSharedPtr<QuadtreeNode> result = childNode->GetMinNode(position);
                if (!result.IsValid())
                {
                    // We got a null ptr, something didn't go quite right. Return the child node we know has something in it, maybe it was on the boundary.
                    return childNode;
                }
                else
                {
                    return result;
                }
            }
                // If this node has no children, we can't be any more accurate
            else
            {
                // Return the child which has no children yet still contains the 
                return childNode;
            }
        }
    }
    return nullptr;
}


ITickHideable* QuadtreeNode::GetNearestActor(FVector2D position)
{
    TUniquePtr<FGenericPlatformMath> genericMaths(MakeUnique<FGenericPlatformMath>());

    ITickHideable* result = nullptr;

    // If the maths object we defined was valid
    if (genericMaths.IsValid())
    {
        // Explore each platform element
        for (auto platform : GetRootNode()->GetAllElements())
        {
            // If result hasn't been initialised yet, don't bother processing, just assume this is the closest platform
            if (result == nullptr)
            {
                result = platform;
            }
            else
            {
                // Calculate the difference in distance with the current platform
                FVector2D platformPos = {platform->GetActorLocation().X, platform->GetActorLocation().Y};
                const float x = genericMaths->Abs(position.X - platformPos.X);
                const float y = genericMaths->Abs(position.Y - platformPos.Y);
                const FVector2D platformDist = {x, y};

                // Calculate the difference in distance with the current result
                FVector2D resultPos = {result->GetActorLocation().X, result->GetActorLocation().Y};
                const float resultX = genericMaths->Abs(position.X - resultPos.X);
                const float resultZ = genericMaths->Abs(position.Y - resultPos.Y);
                const FVector2D resultDist = {resultX, resultZ};

                // If this platform isn't as far away as the current result
                if (platformDist < resultDist)
                {
                    // Update the result
                    result = platform;
                }
            }
        }
    }

    return result;
}
