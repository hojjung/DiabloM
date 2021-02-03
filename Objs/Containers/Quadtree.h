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
	
	bool CheckActorInVisibleNode(ITickHideable* actor);


private:
	void InitialiseNodes(TSharedPtr<QuadtreeNode> parentNode, FVector2D min, FVector2D max);

	TSharedPtr<QuadtreeNode> CreateNode(TSharedPtr<QuadtreeNode> parent, FVector2D min, FVector2D max);
	//
	void NodeShowHide(TSharedPtr<QuadtreeNode>& OldNodeEntered, TSharedPtr<QuadtreeNode>& newNodeEntered);

	//GetNearestActor

private:
	TSharedPtr<QuadtreeNode> m_RootNode;

	int m_MaxDistance;

	float m_fMinCellHeight = 0;
	float m_fMinCellWidth = 0;

	float m_fForMinCellHeight = 0; //대각선
	float m_fForMinCellWidth = 0;

	TSharedPtr<QuadtreeNode> m_CenterNodeEntered;
	TSharedPtr<QuadtreeNode> m_NorthNodeEntered;
	TSharedPtr<QuadtreeNode> m_SouthNodeEntered;
	TSharedPtr<QuadtreeNode> m_EastNodeEntered;
	TSharedPtr<QuadtreeNode> m_WestNodeEntered;
	TSharedPtr<QuadtreeNode> m_NorthEastNodeEntered;
	TSharedPtr<QuadtreeNode> m_NorthWestNodeEntered;
	TSharedPtr<QuadtreeNode> m_SouthEastNodeEntered;
	TSharedPtr<QuadtreeNode> m_SouthWestNodeEntered;
//////////////////////
	float m_fCellFindLength =0.f;

	TSharedPtr<QuadtreeNode> m_CurrentNode;

	TSharedPtr<QuadtreeNode> m_OldRearNode;

	TSharedPtr<QuadtreeNode> m_OldForwardNode;

public:
	void TryShow9Cell(AActor* mover);//플레이어의 현재와 전방,이전셀 3개만

	void TryShow3Cell(AActor* mover);

	void HideAllNode();

	ITickHideable* GetNearestActor(FVector2D& position);

	ITickHideable* GetNearestActor(FVector2D&& position);
};
