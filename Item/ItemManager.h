// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DiabloM.h"
#include "ItemHolder.h"
#include "Datas/ItemDataTable.h"
#include "Datas/ShopItemTable.h"
#include "Item/DroppedItem.h"
#include "ItemManager.generated.h"

class UDiabloGameInstance;

UCLASS()
class DIABLOM_API UItemManager : public UObject, public IItemHolder
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    UDiabloGameInstance* m_GameInstance;

    int m_nCurrentIndex;

    FOnItemSlotChanged m_OnItemGroundChanged;

    TArray<FItemTier*> m_AryItemTier;

    float m_fTierMaxRate;
protected:
    bool CreateRandomOption(const FItemData& itemData, TArray<FOptionSpec>& outOption,int TierMaxOption,float bonus,int level);

    bool CreateRandomOptionWithUnique(const FUniqueEquipData& itemData, TArray<FOptionSpec>& outOption,int TierMaxOption,float bonus,int level);    

    void CreateIntAryForShuffle(int maxAryLen, TArray<FOptionHandle>& outOptionAry);

public:
    void Init(UDiabloGameInstance* gameInstance);

    FItemInstance CreateItemInstance(FName id,float magicItemBonus=0.f,float rareItemBonus=0.f,float epicItemBonus=0.f,int itemLevel=1);

    FItemInstance CreateUniqueItem(const FUniqueEquipData* unique_item, int item_level);
    
    FItemInstance CreateItemManual(const FShopItemSell& item_sell);
    
    virtual FOnItemSlotChanged& GetItemChangeCallback() override
    {
        return m_OnItemGroundChanged;
    }

    const FItemTier& GetDefaultTierRoll() const;


private:
    virtual bool AddItem(int droppedIndex, FItemInstance& itemWantAdd) override;
    virtual void RemoveItem(FItemInstance& itemWantErase) override;
    virtual void RemoveItemByIndex(int index) override;

    virtual bool CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd) override;

    virtual void SetItem(int droppedIndex, FItemInstance& itemWantAdd) override;

    virtual bool SwapMove(FItemInstance& Drop, FItemInstance& Drag) override;
};
