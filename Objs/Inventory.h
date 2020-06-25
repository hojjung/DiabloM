// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Delegate.h"
#include "Datas/ItemDataTable.h"



class DIABLOM_API Inventory
{
public:
	~Inventory();
	DECLARE_DELEGATE_TwoParams(FOnItemSlotChanged, int,const FItemInstance&);

protected:
	int m_nXGridCount;

	int m_nYGridCount;

	TArray<FItemInstance> m_ItemAry;

	//TMap<FName, TArray<FItemInstance>> m_ItemDic;

	int m_nCurrentEmptyIndex;

public:
	FOnItemSlotChanged m_OnSlotChanged;

public:
	void InitInven(int xCount,int yCount);

	//
	void AddItem(int index, FItemInstance& itemWantAdd);

	void AddItemStack(int index );

	void RemoveItem(int index);

	void RemoveItemStack(int index);

	void SwapItemIndex(int aIndex,int bIndex);

	bool StackMoveItem(int increaseIndex, int decreaseIndex);

	bool OnDropIndexHeaped(int droppedSlot, int draggedDDO);
	void SwapMove(int draggedDDO, FItemInstance &Drop, int droppedSlot, FItemInstance &Drag);
	void StackMove(FItemInstance &Drop, FItemInstance &Drag, int droppedSlot, int draggedDDO);
	//드랍으로 인덱스가 겹칠때

	//
	bool AddItemAuto(FItemInstance& itemWantAdd);

	bool AddItemToIndex(int index, FItemInstance& itemWantAdd);
	//
	FItemInstance RemoveItemFromIndex(int index);

	//TArray<FItemInstance> RemoveItemAuto(FName itemId,int wantCount);
	//
	bool AddToList(TArray<FItemInstance>& FoundItemList, FItemInstance &itemWantAdd);
	void SetEmptyIndex();
	bool RemoveItemByName(FName id, int itemCount = 1);//퀘스트 등의 제거
	void RemoveItemInstance(FItemInstance itemWantRemove);//전체 때어냄.드래그
	void PrintInven();
	void GetInvenSize(int& x, int & y);

	FItemInstance& GetItemRef(int index);
protected:
	bool CheckItemExist(TArray<FItemInstance>*& foundItemList, FName id, bool stackable, int countWant = 1);
	bool TryGetStackable(TArray<FItemInstance>& itemList, FItemInstance*& outItemInst);
};
