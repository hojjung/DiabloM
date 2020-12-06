#include "DiaShopPanel.h"
#include "DiaShopGridSlot.h"
#include "Datas/ItemDataTable.h"
#include "Managers/DiabloGameInstance.h"
#include "Item/ShopItemContainer.h"
#include "Widgets/WorldMap/DefaultMenu/DefaultMenu.h"

UDiaShopPanel::UDiaShopPanel(const FObjectInitializer& objInit): Super(objInit)
{
    m_ClassGridSlot = UDiaShopGridSlot::StaticClass();
    m_nCurrentSelectedPanelIndex = 0;
}


void UDiaShopPanel::Init()
{
    m_AryBtns.Reset();
    m_AryBtns.Emplace(m_BtnPanel1);
    m_AryBtns.Emplace(m_BtnPanel2);
    m_AryBtns.Emplace(m_BtnPanel3);

    m_BtnPanel1->OnCheckStateChanged.AddDynamic(this, &UDiaShopPanel::Open1);
    m_BtnPanel2->OnCheckStateChanged.AddDynamic(this, &UDiaShopPanel::Open2);
    m_BtnPanel3->OnCheckStateChanged.AddDynamic(this, &UDiaShopPanel::Open3);

    m_AryGridPanels.Reset();
    m_AryGridPanels.Emplace(m_SlotGridPanel1);
    m_AryGridPanels.Emplace(m_SlotGridPanel2);
    m_AryGridPanels.Emplace(m_SlotGridPanel3);

    for (UGridPanel* GridPanel : m_AryGridPanels)
    {
        GridPanel->SetVisibility(ESlateVisibility::Collapsed);
    }
    m_SlotGridPanel1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    m_AryArySlots.Reset();
    m_AryArySlots.Emplace(&m_ArySlot1);
    m_AryArySlots.Emplace(&m_ArySlot2);
    m_AryArySlots.Emplace(&m_ArySlot3);


    int Iter = 0;
    while (Iter < 2)
    {
        SetGrid(Iter,SHOP_X,SHOP_Y);

        Iter++;
    }
    SetGrid(Iter,SHOPRESELL_X,SHOPRESELL_Y);


    m_BtnSellAllCheap->OnClicked.AddDynamic(this,&UDiaShopPanel::SellAllCheapItem);
}

void UDiaShopPanel::SetGrid(int indexPanel, int x, int y)
{
    int InvenX = x;
    int InvenY = y;

    m_AryArySlots[indexPanel]->Reserve(InvenX * InvenY);

    int Index = 0;
    //(int X = 0; X < InvenX; X++)
    for (int Y = 0; Y < InvenY; Y++)
    {
        for(int X = 0; X < InvenX; X++)
        {
            m_AryGridPanels[indexPanel]->SetColumnFill(X, 1);
            m_AryGridPanels[indexPanel]->SetRowFill(Y, 1);

            UDiaShopGridSlot* SlotCreated = CreateWidget<UDiaShopGridSlot>(this, m_ClassGridSlot);
            UGridSlot* ChildSlot = m_AryGridPanels[indexPanel]->AddChildToGrid(SlotCreated);

            ChildSlot->SetColumn(X);
            ChildSlot->SetRow(Y);

            SlotCreated->InitSlot(Index);
            
            if(indexPanel==2)//Only for ReSell
            {
                SlotCreated->m_OnDropIndex.BindUObject(this, &UDiaShopPanel::SellItem);
            }
            else
            {
                SlotCreated->m_OnDropIndex.BindUObject(this, &UDiaShopPanel::SellItemAuto);
            }

            m_AryArySlots[indexPanel]->Add(SlotCreated);

            Index++;
        }
    }
}


void UDiaShopPanel::UpdateSlot(int index, FItemInstance& itemInst)
{
    (*m_AryArySlots[m_nCurrentSelectedPanelIndex])[index]->SetSlot(itemInst);
}

void UDiaShopPanel::UpdateForReSellSlot(int index, FItemInstance& itemInst)
{
    if(!itemInst.IsEmpty())
    {
        itemInst.m_fBuyCost=itemInst.GetFullStackSellValue();
    }
        
    (*m_AryArySlots[2])[index]->SetSlot(itemInst);
}


void UDiaShopPanel::AddItemStack(int index)
{
    (*m_PtrAryStorages)[m_nCurrentSelectedPanelIndex]->AddItemStack(index);
}

bool UDiaShopPanel::AddItemAuto(FItemInstance& itemWantAdd)
{
    return (*m_PtrAryStorages)[m_nCurrentSelectedPanelIndex]->AddItemAuto(itemWantAdd);
}

bool UDiaShopPanel::SellItem(int index, FItemInstance& itemWantAdd)
{
    PRINTF("Sell1");
    if((*m_PtrAryStorages)[2]->AddItem(index, itemWantAdd))
    {
        return true;
    }

    return false;
}

bool UDiaShopPanel::SellItemAuto(int forDele,FItemInstance& itemWantAdd)
{
    PRINTF("Sell2");
    if((*m_PtrAryStorages)[2]->AddItemAuto(itemWantAdd))
    {
        return true;
    }

    return false;
}

void UDiaShopPanel::SellAllCheapItem()
{
    PRINTF("SellAll");

    for(FItemInstance& ItemInst : ADiabloPlayerController::Get->GetInven()->GetItemAry())
    {
        if(ItemInst.IsEmpty()||ItemInst.IsHighValue())
        {
            continue;
        }
        if(!SellItemAuto(-1,ItemInst))
        {
            break;
        }
    }

}


bool UDiaShopPanel::BuyItemAuto(FItemInstance& itemWantAdd)
{
    PRINTF("Buy2");
    return (*m_PtrAryStorages)[m_nCurrentSelectedPanelIndex]->RemoveItemBecauseSell(itemWantAdd);
}

void UDiaShopPanel::Open1(bool bOpen)
{
    UDefaultMenu::Get->CloseItemPopup();
    if (m_nCurrentSelectedPanelIndex == 0)
    {
        return;
    }

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Unchecked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::Collapsed);

    m_nCurrentSelectedPanelIndex = 0;

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Checked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    
}

void UDiaShopPanel::Open2(bool bOpen)
{
    UDefaultMenu::Get->CloseItemPopup();
    if (m_nCurrentSelectedPanelIndex == 1)
    {
        return;
    }

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Unchecked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::Collapsed);

    m_nCurrentSelectedPanelIndex = 1;

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Checked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    
}

void UDiaShopPanel::Open3(bool bOpen)
{
    UDefaultMenu::Get->CloseItemPopup();
    if (m_nCurrentSelectedPanelIndex == 2)
    {
        return;
    }

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Unchecked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::Collapsed);

    m_nCurrentSelectedPanelIndex = 2;

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Checked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UDiaShopPanel::UpdateShop(int panelIndex, TArray<FItemInstance>& itemAdd)
{
    if (panelIndex < 0 || panelIndex > 2 || itemAdd.Num() < 1)
    {
        return;
    }

    for (FItemInstance& ItemEle : itemAdd)
    {
        AddItemAuto(ItemEle);
    }
}

void UDiaShopPanel::UpdatePanel(AShopKeeper* shop_keeper)
{
    m_PtrAryStorages = shop_keeper->GetShopItemContainer();

    int Iter = 0;

    while (Iter < 3)
    {
        UShopItemContainer* CurrentInven = (*m_PtrAryStorages)[Iter];
        m_nCurrentSelectedPanelIndex = Iter;

        if (Iter < 2)
        {
            CurrentInven->GetItemChangeCallback().AddUObject(this, &UDiaShopPanel::UpdateSlot);
        }
        else//Last ShopPanel
        {
            CurrentInven->GetItemChangeCallback().AddUObject(this, &UDiaShopPanel::UpdateForReSellSlot);
        }

        for (int i = 0; i < CurrentInven->GetItemAry().Num(); i++)
        {
            UpdateSlot(i, CurrentInven->GetItemRef(i));
        }
        
        Iter++;
    }
    m_nCurrentSelectedPanelIndex=0;

    m_BtnPanel1->SetCheckedState(ECheckBoxState::Checked);
}

void UDiaShopPanel::ClearPanel()
{
    if (!m_PtrAryStorages)
    {
        return;
    }

    int MaxIter = m_PtrAryStorages->Num();
    int Iter = 0;

    FItemInstance Empty;
    m_nCurrentSelectedPanelIndex=0;

    while (Iter < MaxIter)
    {
        UShopItemContainer* CurrentInven = (*m_PtrAryStorages)[Iter];

        CurrentInven->GetItemChangeCallback().Clear();

        for (int i = 0; i < CurrentInven->GetItemAry().Num(); i++)
        {
            UpdateSlot(i, Empty);
        }

        Iter++;
        
        m_nCurrentSelectedPanelIndex++;
    }
    m_nCurrentSelectedPanelIndex=0;
  

    m_BtnPanel2->SetCheckedState(ECheckBoxState::Unchecked);
    m_BtnPanel3->SetCheckedState(ECheckBoxState::Unchecked);
    m_BtnPanel1->SetCheckedState(ECheckBoxState::Checked);
    
    m_PtrAryStorages = nullptr;
}
