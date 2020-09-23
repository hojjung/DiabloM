// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "ItemHolder.h"
#include "Datas/ItemDataTable.h"
#include "Item/DroppedItem.h"
#include "ItemManager.generated.h"

class UDiabloGameInstance;

UCLASS()
class DIABLOM_API UItemManager : public UObject, public IItemHolder
{
    GENERATED_BODY()

public:
    ~UItemManager();

protected:
    UPROPERTY()
    UDiabloGameInstance* m_GameInstance;

    int m_nCurrentIndex;

    FOnItemSlotChanged m_OnItemGroundChanged;

    TArray<FItemTier*> m_AryItemTier;

    float m_fTierMaxRate;
protected:
    bool CreateRandomOption(const FItemData& itemData, TArray<FOptionSpec>& outOption,int TierMaxOption,int level);

    void CreateIntAryForShuffle(int maxAryLen, TArray<FOptionHandle>& outOptionAry);

    FOptionSpec CreateRandomOptionValue(int indexRandomd, const FItemData& itemData);

public:
    void Init(UDiabloGameInstance* gameInstance);

    FItemInstance CreateItemInstance(FName itemID, int level = 1);

    ADroppedItem* CreateItemActor(FItemInstance& itemWantAdd, FVector posWant);

    virtual bool AddItem(int droppedIndex, FItemInstance& itemWantAdd) override;
    virtual void RemoveItem(FItemInstance& itemWantErase) override;
    virtual void RemoveItemByIndex(int index) override;

    virtual bool CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd) override;

    virtual void SetItem(int droppedIndex, FItemInstance& itemWantAdd) override;

    virtual bool SwapMove(FItemInstance& Drop, FItemInstance& Drag) override;

    virtual FOnItemSlotChanged& GetItemChangeCallback() override
    {
        return m_OnItemGroundChanged;
    }

    const FItemTier& GetDefaultTierRoll() const;
};
