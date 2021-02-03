#include "QuadtreeNode.h"
#include "DrawDebugHelpers.h"
#include "GenericPlatformMath.h"

QuadtreeNode::QuadtreeNode(): m_eNodePosition(), m_bHasBeenShowed(false)
{
    m_Elements.Reserve(100);
    m_bHasBeenShowed = false;
    m_Siblings.Init(nullptr,4);
    m_bVisible=false;
}

/** Shallow copies a QuadtreeNode. */
QuadtreeNode::QuadtreeNode(const QuadtreeNode& copy)
{
    SetParentNode(copy.m_ParentNode);
    m_BoundingBox = copy.m_BoundingBox;
    m_Elements = copy.m_Elements;
    m_ChildNodes = copy.m_ChildNodes;
    m_bHasBeenShowed = false;
    m_bVisible=false;
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

void QuadtreeNode::SetParentNode(TSharedPtr<QuadtreeNode> parentNode)
{
    this->m_ParentNode = parentNode;
}

TSharedPtr<QuadtreeNode> QuadtreeNode::GetParentNode()
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
    FVector2D position = {element->GetActorLocation().X, element->GetActorLocation().Y};
    
    if(element->GetCurrentNode())
    {
        element->GetCurrentNode()->RemoveElement(element);
        element->SetNode(nullptr);
    }
    
    for (auto childNode : m_ChildNodes)
    {
        if (childNode->m_BoundingBox->IsInside(position))
        {
            if (childNode->HasChildNodes())
            {
                childNode->AddElement(element);
                return;
            }
            else
            {
                childNode->m_Elements.Add(*element);
                element->SetNode(childNode.Get());
                return;
            }
        }
    }

    if(m_BoundingBox->IsInside(position))
    {
        m_Elements.Add(*element);
        element->SetNode(this);
    }
}

void QuadtreeNode::RemoveElement(ITickHideable* element)
{
    TWeakInterfacePtr<ITickHideable> RemoveWant(*element);
    
    m_Elements.Remove(RemoveWant);
}


TArray<TWeakInterfacePtr<ITickHideable>> QuadtreeNode::GetMyElements()
{
    return m_Elements;
}

void QuadtreeNode::DrawBoxAroundNode(UWorld* world, FColor colour)
{
    FVector centre = {m_BoundingBox->GetCenter().X, m_BoundingBox->GetCenter().Y, 100.f};
    FVector extent = {m_BoundingBox->GetExtent().X, m_BoundingBox->GetExtent().Y, 0.0f};

    DrawDebugBox(world, centre, extent, colour,false,-1,1,10.f);
   //DrawDebugBox(world, centre, extent, m_bVisible? FColor::Red:colour,false,-1,m_bVisible?1:0,m_bVisible?20.f:0); DrawDebugBox(world, centre, extent, m_bVisible? FColor::Red:colour,false,-1,m_bVisible?1:0,m_bVisible?20.f:0);

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

bool QuadtreeNode::IsPositionInsideNode(FVector position)
{
    FVector2D pos2d = {position.X, position.Y};
    for (auto childNode : m_ChildNodes)
    {
        if (childNode->m_BoundingBox->IsInside(pos2d))
        {
            return childNode->IsPositionInsideNode(position);
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
        if (childNode->IsPositionInsideNode(position))
        {
            return childNode;
        }
    }

    return nullptr;
}

TArray<TWeakInterfacePtr<ITickHideable>> QuadtreeNode::GetAllElements()
{
    TArray<TWeakInterfacePtr<ITickHideable>> result;

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
        m_bVisible=false;
        
        for (TWeakInterfacePtr<ITickHideable> Eles : GetAllElements())
        {
            if(!Eles.IsValid())
            {
                continue;
            }
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
        m_bVisible=true;
        
        if (!m_bHasBeenShowed)
        {
            m_bHasBeenShowed = true;
        }
        
        for (TWeakInterfacePtr<ITickHideable> Eles : GetAllElements())
        {
            if(!Eles.IsValid())
            {
                continue;
            }
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
    TSharedPtr<QuadtreeNode> parentNode = GetParentNode();

    // If the parent node isn't nullptr
    if (parentNode.IsValid())
    {
        // Grab an instance of the grandparent
        TSharedPtr<QuadtreeNode> parentOfParent = parentNode->GetParentNode();

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


ITickHideable* QuadtreeNode::GetNearestActor(FVector2D& position)//문제는 몬스터와 아이템이 구분되야함,일단 가까운것부터 처리?
{
    TUniquePtr<FGenericPlatformMath> genericMaths(MakeUnique<FGenericPlatformMath>());

    TWeakInterfacePtr<ITickHideable> result;

    // If the maths object we defined was valid
    if (genericMaths.IsValid())
    {
        // Explore each platform element
        for (TWeakInterfacePtr<ITickHideable> platform : GetRootNode()->GetAllElements())
        {
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
    
    return result.Get();
}

ITickHideable* QuadtreeNode::GetNearestActor(FVector2D&& position)
{
    return GetNearestActor(position);
}
