// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item/ItemHolder.h"
#include "Village/VillageActor.h"
#include "Storage.generated.h"

/**
 * 
 */
UCLASS()
class DIABLOM_API AStorage : public AVillageActor, public IItemHolder
{
	GENERATED_BODY()

public:
	AStorage();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* m_MeshStorage;
	
protected:
	FOnItemSlotChanged m_ItemSlotChanged;

	float m_fCurrentGold;
	
public:
	virtual void RemoveItem(FItemInstance& itemWantErase) override;
	virtual void RemoveItemByIndex(int index)override;
	virtual bool AddItem(int droppedIndex, FItemInstance& itemWantAdd) override;
	virtual bool CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd)override;
	virtual void SetItem(int droppedIndex, FItemInstance& itemWantAdd) override;
	virtual bool SwapMove(FItemInstance &Drop, FItemInstance &Drag)override;
	virtual FOnItemSlotChanged& GetItemChangeCallback()  override;
};
