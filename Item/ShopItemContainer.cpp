#include "ShopItemContainer.h"
#include "Inventory.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Datas/ItemDataTable.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/WorldMap/DefaultMenu/DiaInvenGridPanel.h"

UShopItemContainer::~UShopItemContainer()
{
    m_nCurrentEmptyIndex = -1;
    m_nXGridCount = -1;
    m_nYGridCount = -1;
}

bool UShopItemContainer::BuyItemGiveGoldShop(const FItemInstance& itemInst)
{
    return ADiabloPlayerController::Get->GetPlayerPawn()->SpendGold(itemInst.m_fBuyCost);
}

bool UShopItemContainer::BuyItemGiveGoldShopOneStack(const FItemInstance& itemInst)
{
    return ADiabloPlayerController::Get->GetPlayerPawn()->SpendGold(itemInst.m_fBuyCost);
}

void UShopItemContainer::SellItemGiveGoldPlayer(const FItemInstance& itemInst)
{
    ADiabloPlayerController::Get->GetPlayerPawn()->EarnGold(itemInst.GetFullStackSellValue());
}

void UShopItemContainer::SellItemGiveGoldPlayerOneStack(const FItemInstance& itemInst)
{
    ADiabloPlayerController::Get->GetPlayerPawn()->EarnGold(itemInst.GetOneStackSellValue());
}

void UShopItemContainer::InitShopStorage(int xCount, int yCount)
{
    m_nCurrentEmptyIndex = 0;

    m_nXGridCount = xCount;

    m_nYGridCount = yCount;

    int CountTotal = m_nXGridCount * m_nYGridCount;

    m_ItemAry.Reserve(CountTotal);

    m_ItemAry.Init(FItemInstance(), CountTotal);
}

bool UShopItemContainer::CheckSlotValid(int droppedIndex, FItemInstance& itemWantAdd)
{
    return &m_ItemAry[droppedIndex];
}

void UShopItemContainer::SetItem(int droppedIndex, FItemInstance& itemWantAdd)
{
    m_ItemAry[droppedIndex] = itemWantAdd;
    m_ItemAry[droppedIndex].m_nGridIndex = droppedIndex;
    m_ItemAry[droppedIndex].m_Holder = this;
    //m_ItemAry[droppedIndex].m_fBuyCost=(m_ItemAry[droppedIndex].GetSellValue());
    m_OnSlotChanged.Broadcast(droppedIndex, m_ItemAry[droppedIndex]);
}


void UShopItemContainer::RemoveItem(FItemInstance& itemWantErase)
{
    RemoveItemByIndex(itemWantErase.m_nGridIndex);
}

void UShopItemContainer::RemoveItemByIndex(int index) //Sell
{
    m_ItemAry[index].ClearData();
    m_OnSlotChanged.Broadcast(index, m_ItemAry[index]);
}

void UShopItemContainer::RemoveItemStack(int index)
{
    m_ItemAry[index].m_nCurrentStack--;

    if (m_ItemAry[index].m_nCurrentStack <= 0)
    {
        RemoveItemByIndex(index);
    }

    m_OnSlotChanged.Broadcast(index, m_ItemAry[index]);
}

bool UShopItemContainer::AddItemStack(int index)
{
    

    m_ItemAry[index].m_nCurrentStack++;

    m_OnSlotChanged.Broadcast(index, m_ItemAry[index]);

    return true;
}

bool UShopItemContainer::AddItem(int droppedIndex, FItemInstance& itemWantAdd) //빌드후 여기도
{
    if (this == static_cast<UShopItemContainer*>(itemWantAdd.m_Holder) && droppedIndex == itemWantAdd.m_nGridIndex)
    {
        PRINTF("Prevent MySelf");

        return false;
    }

    if (CheckSlotValid(droppedIndex, itemWantAdd) &&
        m_ItemAry[droppedIndex].m_ItemID == NAME_None)
    {
        SellItemGiveGoldPlayer(itemWantAdd);
        SetItem(droppedIndex, itemWantAdd);

        if (itemWantAdd.m_Holder)
        {
            itemWantAdd.m_Holder->RemoveItem(itemWantAdd);
        }
        return true;
    }
    ///
    int DragIndex = itemWantAdd.m_nGridIndex;

    FItemInstance DropOldItem = m_ItemAry[droppedIndex];

    //Stack
    bool Result = false;
    //safe
    if (itemWantAdd.GetIsStackable() && itemWantAdd.CheckCanStack() &&
        DropOldItem.GetIsStackable() && DropOldItem.CheckCanStack() &&
        DropOldItem.m_TierID == itemWantAdd.m_TierID &&
        DropOldItem.m_ItemID == itemWantAdd.m_ItemID) //스왑방지코드
    {
        PRINTF("Stack");
        StackMove(DropOldItem, itemWantAdd, itemWantAdd.m_Holder);
    }
    else
    {
        PRINTF("SWap");
        SwapMove(DropOldItem, itemWantAdd);
    }

    return Result;
}

bool UShopItemContainer::AddItemAuto(FItemInstance& item_instance)
{
    int Result = GetEmptyIndex();

    if (Result == -1)
    {
        return false;
    }

    return AddItem(Result, item_instance);
}

bool UShopItemContainer::RemoveItemBecauseSell(FItemInstance& itemWantAdd)
{
    float Value = itemWantAdd.m_fBuyCost;
    
    if(!BuyItemGiveGoldShop(itemWantAdd))
    {
        return  false;
    }

    if(!UDiaInvenGridPanel::GetInvenWidgetInst->AddItemAuto(itemWantAdd))
    {
        //Fail No Space
        ADiabloPlayerController::Get->GetPlayerPawn()->EarnGold(Value);

        return false;
    }
    
    RemoveItem(itemWantAdd);

    return true;
}

bool UShopItemContainer::RemoveItemBecauseSellStack(FItemInstance& itemWantAdd)
{
    float Value = itemWantAdd.m_fBuyCost;
    
    if(!BuyItemGiveGoldShopOneStack(itemWantAdd))
    {
        return  false;
    }

    RemoveItemStack(itemWantAdd.m_nGridIndex);

    FItemInstance Item=itemWantAdd;
    
    Item.m_nCurrentStack=1;
    
    if(!UDiaInvenGridPanel::GetInvenWidgetInst->AddItemAuto(Item))
    {
        //Fail No Space
        ADiabloPlayerController::Get->GetPlayerPawn()->EarnGold(Value);

        return false;
    }
    
    RemoveItem(itemWantAdd);
    

    return true;
}

bool UShopItemContainer::SwapMove(FItemInstance& Drop, FItemInstance& Drag)
{
    int DropIndex = Drop.m_nGridIndex;
    int DragIndex = Drag.m_nGridIndex;

    if (!Drag.m_Holder->CheckSlotValid(DragIndex, Drop))
    {
        return false;
    }

    if(!BuyItemGiveGoldShop(Drop))
    {
        return false;
    }
    SellItemGiveGoldPlayer(Drag);
    
    Drag.m_Holder->SetItem(DragIndex, Drop);
    Drop.m_Holder->SetItem(DropIndex, Drag);

    return true;
}

void UShopItemContainer::StackMove(FItemInstance& Drop, FItemInstance& Drag, IItemHolder* preItemHolder)
{
    int DiffStackCount = Drop.GetMaxStack() - Drop.m_nCurrentStack;

    int Count = FMath::Min(DiffStackCount, Drag.m_nCurrentStack);

    while (Count--)
    {
        SellItemGiveGoldPlayerOneStack(Drop);
        
        AddItemStack(Drop.m_nGridIndex);
        
        Drag.m_nCurrentStack--;
    }

    if (Drag.m_nCurrentStack <= 0)
    {
        preItemHolder->RemoveItem(Drag);
    }
}

void UShopItemContainer::PrintInven()
{
    PRINTF("----ShopPrint----");

    for (FItemInstance& Item : m_ItemAry)
    {
        if (Item.m_ItemID == NAME_None)
        {
            continue;
        };
        PRINTF("ItemName: %s,Index: %d, Stack:%d",
               *UItemDataTable::GetItemData(Item.m_ItemID).m_ShowingName.ToString(), Item.m_nGridIndex,
               Item.m_nCurrentStack);
    }
}

void UShopItemContainer::GetInvenSize(int& x, int& y)
{
    x = m_nXGridCount;
    y = m_nYGridCount;
}

FItemInstance& UShopItemContainer::GetItemRef(int index)
{
    return m_ItemAry[index];
}

const TArray<FItemInstance>& UShopItemContainer::GetItemAry() const
{
    return m_ItemAry;
}

void UShopItemContainer::SetItemAry(TArray<FItemInstance>& loadedAry)
{
    for (int i = 0; i < m_ItemAry.Num(); i++)
    {
        if (loadedAry.Num() <= i)
        {
            //inven size changed
            return;
        }
        if (!loadedAry[i].m_ItemData)
        {
            continue;
        }

        AddItem(i, loadedAry[i]);
    }
}

int UShopItemContainer::GetEmptyIndex()
{
    for (int i = 0; i < m_ItemAry.Num(); i++)
    {
        if (m_ItemAry[i].IsEmpty())
        {
            return i;
        }
    }
    return -1;
}
