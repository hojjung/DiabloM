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
	TMap<FName, TArray<FItemInstance>> m_ItemContainer;

	int m_nXGridCount;

	int m_nYGridCount;

	int m_nCurrentEmptyIndex;

	TArray<bool> m_GridItem;

public:
	FOnItemSlotChanged m_OnSlotChanged;

public:
	void InitInven(int xCount,int yCount);
	bool AddItem(FItemInstance itemWantAdd);
	bool AddToList(TArray<FItemInstance>& FoundItemList, FItemInstance &itemWantAdd);
	void SetEmptyIndex();
	bool RemoveItemByName(FName id, int itemCount = 1);//퀘스트 등의 제거
	void RemoveItemInstance(FItemInstance itemWantRemove);//전체 때어냄.드래그
	void PrintInven();
	void GetInvenSize(int& x, int & y);
protected:
	bool CheckItemExist(TArray<FItemInstance>*& foundItemList, FName id, bool stackable, int countWant = 1);
	bool TryGetStackable(TArray<FItemInstance>& itemList, FItemInstance*& outItemInst);
};
