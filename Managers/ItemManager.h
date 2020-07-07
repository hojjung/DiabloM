// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "Datas/ItemDataTable.h"
#include "Objs/Interfaces/ItemHolder.h"



class UDiabloGameInstance;

class DIABLOM_API ItemManager:public IItemHolder
{
public:
	ItemManager();
	~ItemManager();

protected:
	UDiabloGameInstance* m_GameInstance;

	TArray<FItemInstance*> m_AryItemInWorld;

	int m_nCurrentIndex;

	FOnItemSlotChanged m_OnItemGroundChanged;

protected:
	bool CreateRandomOption(int level, const FItemData& itemData,TArray<FOptionValue>& outOption);

	void CreateIntAryForShuffle(int maxAryLen,TArray<int>& outIndexAry);

	FOptionValue CreateRandomOptionValue(int indexRandomd,const FItemData& itemData);

public:
	void Init(UDiabloGameInstance* gameInstance);

	FItemInstance CreateItemInstance(FName itemID);

	virtual bool AddItem(int droppedIndex, FItemInstance& itemWantAdd) override;
	virtual void RemoveItem(FItemInstance& itemWantErase) override;
	virtual void RemoveItemByIndex(int index) override;

	virtual bool CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd) override;

	virtual void SetItem(int droppedIndex, FItemInstance& itemWantAdd) override;

	virtual bool SwapMove(FItemInstance &Drop, FItemInstance &Drag) override;

	virtual FOnItemSlotChanged& GetItemChangeCallback() override
	{
		return m_OnItemGroundChanged;
	}
};
