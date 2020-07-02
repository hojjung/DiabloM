// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemHolder.generated.h"

struct FItemInstance;
UINTERFACE(MinimalAPI)
class UItemHolder : public UInterface
{
	GENERATED_BODY()
};


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemSlotChanged, int, const FItemInstance&);

class DIABLOM_API IItemHolder
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RemoveItem(FItemInstance& itemWantErase) = 0;
	virtual void RemoveItemByIndex(int index) = 0;
	virtual bool AddItem(int droppedIndex, FItemInstance& itemWantAdd) = 0;

	virtual bool CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd)=0;

	virtual void SetItem(int droppedIndex, FItemInstance& itemWantAdd) = 0;

	virtual bool SwapMove(FItemInstance &Drop, FItemInstance &Drag) = 0;

	virtual FOnItemSlotChanged& GetItemChangeCallback()  =0;


};
