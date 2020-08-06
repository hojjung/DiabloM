// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Delegate.h"
#include "Datas/ItemDataTable.h"



class DIABLOM_API Inventory :public IItemHolder
{
public:
	~Inventory();
	

protected:
	int m_nXGridCount;

	int m_nYGridCount;

	TArray<FItemInstance> m_ItemAry;

	//TMap<FName, TArray<FItemInstance>> m_ItemDic;

	int m_nCurrentEmptyIndex;

	FOnItemSlotChanged m_OnSlotChanged;
public:
	virtual FOnItemSlotChanged& GetItemChangeCallback() override
	{
		return m_OnSlotChanged;
	}
public:
	void InitInven(int xCount,int yCount);
	//
	virtual bool CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd) override;

	virtual void SetItem(int index, FItemInstance& itemWantAdd) override;

	void AddItemStack(int index );

	virtual void RemoveItem(FItemInstance& itemWantErase)override;
	virtual void RemoveItemByIndex(int index) override;

	void RemoveItemStack(int index);

	virtual bool AddItem(int droppedIndex, FItemInstance& itemWantAdd ) override;
	virtual bool SwapMove(FItemInstance &Drop, FItemInstance &Drag) override;

	void StackMove(FItemInstance &Drop, FItemInstance &Drag, IItemHolder* preItemHolder);

	void PrintInven();
	void GetInvenSize(int& x, int & y);

	FItemInstance& GetItemRef(int index);

	FORCEINLINE const TArray<FItemInstance>& GetItemAry() const
	{
		return m_ItemAry;
	}

	void SetItemAry(TArray<FItemInstance>& loadedAry);
};
