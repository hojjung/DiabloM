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


private:
	void InitialiseNodes(TSharedPtr<QuadtreeNode> parentNode, FVector2D min, FVector2D max);

	TSharedPtr<QuadtreeNode> CreateNode(TSharedPtr<QuadtreeNode> parent, FVector2D min, FVector2D max);
	//
	void NodeShowHide(TSharedPtr<QuadtreeNode>& OldNodeEntered, TSharedPtr<QuadtreeNode>& newNodeEntered);

private:
	TSharedPtr<QuadtreeNode> m_RootNode;

	int m_MaxDistance;

	float MinCellHeight = 0;
	float MinCellWidth = 0;

	float ForMinCellHeight = 0; //대각선
	float ForMinCellWidth = 0;

	TSharedPtr<QuadtreeNode> m_CenterNodeEntered;
	TSharedPtr<QuadtreeNode> m_NorthNodeEntered;
	TSharedPtr<QuadtreeNode> m_SouthNodeEntered;
	TSharedPtr<QuadtreeNode> m_EastNodeEntered;
	TSharedPtr<QuadtreeNode> m_WestNodeEntered;
	TSharedPtr<QuadtreeNode> m_NorthEastNodeEntered;
	TSharedPtr<QuadtreeNode> m_NorthWestNodeEntered;
	TSharedPtr<QuadtreeNode> m_SouthEastNodeEntered;
	TSharedPtr<QuadtreeNode> m_SouthWestNodeEntered;

public:
	void TryShow9Cell(FVector&& centerPosition);
};
