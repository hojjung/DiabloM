#include "Quadtree.h"


Quadtree::Quadtree(): m_MaxDistance(0)
{
    m_RootNode = nullptr;
}

Quadtree::Quadtree(const int maxDistance, FVector2D min, FVector2D max)
{
    m_MaxDistance = maxDistance;
    m_RootNode = CreateNode(nullptr, min, max);
    m_RootNode->SetNodePosition(ENodePosition::Root);
    InitialiseNodes(m_RootNode, min, max);
}

Quadtree::~Quadtree()
{
    PRINTF("Quadtree Die");
}

TSharedPtr<QuadtreeNode> Quadtree::GetRootNode()
{
    return m_RootNode;
}

void Quadtree::AddElement(ITickHideable* platform)
{
    m_RootNode->AddElement(platform);
}

void Quadtree::DrawBoxes(UWorld* world)
{
    m_RootNode->DrawBoxAroundNode(world, FColor().Blue);
}

TSharedPtr<QuadtreeNode> Quadtree::GetNode(FVector position)
{
    if (m_RootNode->IsPositionInsideNode(position))
    {
        return m_RootNode->GetNode(position);
    }

    return nullptr;
}

TSharedPtr<QuadtreeNode> Quadtree::GetMinNode(FVector2D position)
{
   return m_RootNode->GetMinNode(position);
}

int Quadtree::GetMaxDistance() const
{
    return m_MaxDistance;
}

void Quadtree::TickTryShowActors(FVector&& position)
{
    FVector2D pos2D(position);
    
    TSharedPtr<QuadtreeNode> NodeEntered = GetMinNode(pos2D);
    
    if(!NodeEntered)//바운더리 밖임
    {
        if(m_CurrentNode)//해제
        {
            m_CurrentNode->HideActors();
            
            m_CurrentNode=nullptr;
        }
        if(m_OldNode)
        {
            m_OldNode->HideActors();

            m_OldNode=nullptr;
        }
        return;
    }

    if(NodeEntered!=m_CurrentNode)
    {
        if(m_CurrentNode)
        {
            if(m_OldNode)
            {
                m_OldNode->HideActors();                
            }
            
            m_OldNode=m_CurrentNode;
        }
        
        m_CurrentNode=NodeEntered;

        m_CurrentNode->ShowActors();
    }
}


void Quadtree::InitialiseNodes(TSharedPtr<QuadtreeNode> parentNode, FVector2D min, FVector2D max)
{
    // Bottom right node
    FVector2D brMin = min;
    FVector2D brMax = {max.X - ((max.X - min.X) / 2.0f), max.Y - ((max.Y - min.Y) / 2.0f)};
    TSharedPtr<QuadtreeNode> br = CreateNode(parentNode, brMin, brMax);
    br->SetNodePosition(ENodePosition::BottomRight);

    // Bottom left node
    FVector2D blMin = {max.X - ((max.X - min.X) / 2.0f), min.Y};
    FVector2D blMax = {max.X, min.Y + ((max.Y - min.Y) / 2.0f)};
    TSharedPtr<QuadtreeNode> bl = CreateNode(parentNode, blMin, blMax);
    bl->SetNodePosition(ENodePosition::BottomLeft);

    // Top right node
    FVector2D trMin = {min.X, max.Y - ((max.Y - min.Y) / 2.0f)};
    FVector2D trMax = {max.X - ((max.X - min.X) / 2.0f), max.Y};
    TSharedPtr<QuadtreeNode> tr = CreateNode(parentNode, trMin, trMax);
    tr->SetNodePosition(ENodePosition::TopRight);

    // Top left node
    FVector2D tlMin = {max.X - ((max.X - min.X) / 2.0f), max.Y - ((max.Y - min.Y) / 2.0f)};
    FVector2D tlMax = {max.X, max.Y};
    TSharedPtr<QuadtreeNode> tl = CreateNode(parentNode, tlMin, tlMax);
    tl->SetNodePosition(ENodePosition::TopLeft);

    if (bl->GetDistance() < m_MaxDistance &&
        br->GetDistance() < m_MaxDistance &&
        tl->GetDistance() < m_MaxDistance &&
        tr->GetDistance() < m_MaxDistance)
    {
        InitialiseNodes(br, brMin, brMax);
        InitialiseNodes(bl, blMin, blMax);
        InitialiseNodes(tr, trMin, trMax);
        InitialiseNodes(tl, tlMin, tlMax);
    }
}

TSharedPtr<QuadtreeNode> Quadtree::CreateNode(TSharedPtr<QuadtreeNode> parent, FVector2D min, FVector2D max)
{
    // Bottom left node
    TSharedPtr<QuadtreeNode> newNode = MakeShared<QuadtreeNode>();
    if (parent.IsValid())
    {
        parent->AddChildNode(newNode);
    }
    newNode->SetParentNode(TWeakPtr<QuadtreeNode>(parent));
    
    newNode->SetBoundingBox(min, max);
    
    return newNode;
}
