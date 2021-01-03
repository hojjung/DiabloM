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
    float CellCount = FMath::Pow(2, maxDistance);
    FVector2D Diff = max -min; 
    MinCellHeight=Diff.Y/CellCount;
    MinCellWidth =Diff.X /CellCount;

    ForMinCellHeight =MinCellHeight * 1.0f;
    ForMinCellWidth =MinCellWidth * 1.0f;
}

Quadtree::~Quadtree()
{
    PRINTF("Quadtree Die");
    m_RootNode.Reset();
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
    //
    bl->m_Siblings[ENodePosition::TopLeft] =tl;
    bl->m_Siblings[ENodePosition::BottomLeft] =bl;
    bl->m_Siblings[ENodePosition::BottomRight] =br;
    bl->m_Siblings[ENodePosition::TopRight] =tr;
    //
    tl->m_Siblings[ENodePosition::TopLeft] =tl;
    tl->m_Siblings[ENodePosition::BottomLeft] =bl;
    tl->m_Siblings[ENodePosition::BottomRight] =br;
    tl->m_Siblings[ENodePosition::TopRight] =tr;
    //
    tr->m_Siblings[ENodePosition::TopLeft] =tl;
    tr->m_Siblings[ENodePosition::BottomLeft] =bl;
    tr->m_Siblings[ENodePosition::BottomRight] =br;
    tr->m_Siblings[ENodePosition::TopRight] =tr;
    //
    br->m_Siblings[ENodePosition::TopLeft] =tl;
    br->m_Siblings[ENodePosition::BottomLeft] =bl;
    br->m_Siblings[ENodePosition::BottomRight] =br;
    br->m_Siblings[ENodePosition::TopRight] =tr;
    //
    

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
    newNode->SetParentNode(TSharedPtr<QuadtreeNode>(parent));
    
    newNode->SetBoundingBox(min, max);
    
    return newNode;
}

void Quadtree::NodeShowHide(TSharedPtr<QuadtreeNode>& OldNodeEntered,TSharedPtr<QuadtreeNode>& newNodeEntered)
{
    if(!newNodeEntered)
    {
        return;
    }

    OldNodeEntered = newNodeEntered;
     
    OldNodeEntered->ShowActors();
}

void Quadtree::TryShow9Cell(FVector&& centerPosition)
{
    FVector2D CenterPos2D(centerPosition);
    FVector2D NorthPos2D=CenterPos2D;
    FVector2D SouthPos2D=CenterPos2D;
    FVector2D EastPos2D=CenterPos2D;
    FVector2D WestPos2D=CenterPos2D;
    FVector2D NorthEastPos2D=CenterPos2D;
    FVector2D NorthWestPos2D=CenterPos2D;
    FVector2D SouthEastPos2D=CenterPos2D;
    FVector2D SouthWestPos2D=CenterPos2D;

    

    NorthPos2D.Y+=MinCellHeight;
    SouthPos2D.Y-=MinCellHeight;

    EastPos2D.X+=MinCellWidth;
    WestPos2D.X-=MinCellWidth;

    //대각선
    NorthEastPos2D.Y+=ForMinCellHeight;
    NorthEastPos2D.X+=ForMinCellWidth;

    NorthWestPos2D.Y+=ForMinCellHeight;
    NorthWestPos2D.X-=ForMinCellWidth;

    SouthWestPos2D.Y-=ForMinCellHeight;
    SouthWestPos2D.X-=ForMinCellWidth;

    SouthEastPos2D.Y-=ForMinCellHeight;
    SouthEastPos2D.X+=ForMinCellWidth;

    TSharedPtr<QuadtreeNode> CenterNodeEntered    = GetMinNode(CenterPos2D);
    TSharedPtr<QuadtreeNode> NorthNodeEntered     = GetMinNode(NorthPos2D);
    TSharedPtr<QuadtreeNode> SouthNodeEntered     = GetMinNode(SouthPos2D);
    TSharedPtr<QuadtreeNode> EastNodeEntered      = GetMinNode(EastPos2D);
    TSharedPtr<QuadtreeNode> WestNodeEntered      = GetMinNode(WestPos2D);
    TSharedPtr<QuadtreeNode> NorthEastNodeEntered = GetMinNode(NorthEastPos2D);
    TSharedPtr<QuadtreeNode> NorthWestNodeEntered = GetMinNode(NorthWestPos2D);
    TSharedPtr<QuadtreeNode> SouthEastNodeEntered = GetMinNode(SouthEastPos2D);
    TSharedPtr<QuadtreeNode> SouthWestNodeEntered = GetMinNode(SouthWestPos2D);

    if(CenterNodeEntered!=m_CenterNodeEntered)
    {
        if(m_CenterNodeEntered)
        {
            m_CenterNodeEntered->HideActors();
            m_CenterNodeEntered=nullptr;
        }
        if(m_NorthNodeEntered)
        {
            m_NorthNodeEntered->HideActors();
            m_NorthNodeEntered=nullptr;
        }
        if(m_EastNodeEntered)
        {
            m_EastNodeEntered->HideActors();
            m_EastNodeEntered=nullptr;
        }
        if(m_WestNodeEntered)
        {
            m_WestNodeEntered->HideActors();
            m_WestNodeEntered=nullptr;
        }
        if(m_NorthEastNodeEntered)
        {
            m_NorthEastNodeEntered->HideActors();
            m_NorthEastNodeEntered=nullptr;
        }
        if(m_NorthWestNodeEntered)
        {
            m_NorthWestNodeEntered->HideActors();
            m_NorthWestNodeEntered=nullptr;
        }
        if(m_SouthEastNodeEntered)
        {
            m_SouthEastNodeEntered->HideActors();
            m_SouthEastNodeEntered=nullptr;
        }
        if(m_SouthWestNodeEntered)
        {
            m_SouthWestNodeEntered->HideActors();
            m_SouthWestNodeEntered=nullptr;
        }
        if(m_SouthNodeEntered)
        {
            m_SouthNodeEntered->HideActors();
            m_SouthNodeEntered=nullptr;
        }//문제 없는데
    

        NodeShowHide(m_CenterNodeEntered,CenterNodeEntered);
        PRINTF("Hide1");
        NodeShowHide(m_NorthNodeEntered,NorthNodeEntered);
        PRINTF("Hide2");
        NodeShowHide(m_EastNodeEntered,EastNodeEntered);
        PRINTF("Hide3");
        NodeShowHide(m_WestNodeEntered,WestNodeEntered);
        PRINTF("Hide4");
        NodeShowHide(m_SouthNodeEntered,SouthNodeEntered);
        PRINTF("Hide5");
        NodeShowHide(m_SouthWestNodeEntered,SouthWestNodeEntered);
        PRINTF("Hide6");
        NodeShowHide(m_SouthEastNodeEntered,SouthEastNodeEntered);
        PRINTF("Hide7");
        NodeShowHide(m_NorthWestNodeEntered,NorthWestNodeEntered);
        PRINTF("Hide8");
        NodeShowHide(m_NorthEastNodeEntered,NorthEastNodeEntered);
        PRINTF("Hide9");
    }
}

 
 