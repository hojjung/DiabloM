#include "DiaStorageGridPanel.h"

#include "DiaDragDrop.h"
#include "DiaInvenGridSlot.h"
#include "Widgets/WorldMap/DefaultMenu/DefaultMenu.h"
#include "Item/Inventory.h"
#include "Datas/ItemDataTable.h"
#include "Widgets/CommonElement/HoverToggle.h"

UDiaStorageGridPanel* UDiaStorageGridPanel::GetStorageWidgetInst = nullptr;

UDiaStorageGridPanel::UDiaStorageGridPanel(const FObjectInitializer& objInit): Super(objInit)
{
    m_ClassGridSlot = UDiaInvenGridSlot::StaticClass();
    m_nCurrentSelectedPanelIndex = 0;
    m_nPreSelectedPanelIndex = 0;
}

void UDiaStorageGridPanel::Init(TArray<UInventory*>* itemContainer)
{
    m_AryBtns.Reset();
    m_AryBtns.Emplace(m_BtnPanel1->m_Toggle);
    m_AryBtns.Emplace(m_BtnPanel2->m_Toggle);
    m_AryBtns.Emplace(m_BtnPanel3->m_Toggle);
    m_AryBtns.Emplace(m_BtnPanel4->m_Toggle);
    m_AryBtns.Emplace(m_BtnPanel5->m_Toggle);

    m_BtnPanel1->m_Toggle->OnCheckStateChanged.AddDynamic(this, &UDiaStorageGridPanel::Open1);
    m_BtnPanel2->m_Toggle->OnCheckStateChanged.AddDynamic(this, &UDiaStorageGridPanel::Open2);
    m_BtnPanel3->m_Toggle->OnCheckStateChanged.AddDynamic(this, &UDiaStorageGridPanel::Open3);
    m_BtnPanel4->m_Toggle->OnCheckStateChanged.AddDynamic(this, &UDiaStorageGridPanel::Open4);
    m_BtnPanel5->m_Toggle->OnCheckStateChanged.AddDynamic(this, &UDiaStorageGridPanel::Open5);

    // m_BtnPanel1->OnHovered.AddDynamic(this,&UDiaStorageGridPanel::Open1Wrap);
    // m_BtnPanel2->OnHovered.AddDynamic(this,&UDiaStorageGridPanel::Open2Wrap);
    // m_BtnPanel3->OnHovered.AddDynamic(this,&UDiaStorageGridPanel::Open3Wrap);
    // m_BtnPanel4->OnHovered.AddDynamic(this,&UDiaStorageGridPanel::Open4Wrap);
    // m_BtnPanel5->OnHovered.AddDynamic(this,&UDiaStorageGridPanel::Open5Wrap);

    m_AryGridPanels.Reset();
    m_AryGridPanels.Emplace(m_SlotGridPanel1);
    m_AryGridPanels.Emplace(m_SlotGridPanel2);
    m_AryGridPanels.Emplace(m_SlotGridPanel3);
    m_AryGridPanels.Emplace(m_SlotGridPanel4);
    m_AryGridPanels.Emplace(m_SlotGridPanel5);

    for (UGridPanel* GridPanel : m_AryGridPanels)
    {
        GridPanel->SetVisibility(ESlateVisibility::Collapsed);
    }
    m_SlotGridPanel1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    m_AryArySlots.Reset();
    m_AryArySlots.Emplace(&m_ArySlot1);
    m_AryArySlots.Emplace(&m_ArySlot2);
    m_AryArySlots.Emplace(&m_ArySlot3);
    m_AryArySlots.Emplace(&m_ArySlot4);
    m_AryArySlots.Emplace(&m_ArySlot5);

    m_AryStorages = itemContainer;
    UDiaStorageGridPanel::GetStorageWidgetInst = this;

    int X = 0;
    int Y = 0;

    int MaxIter = m_AryStorages->Num();
    int Iter = 0;

    while (Iter < MaxIter)
    {
        UInventory* CurrentInven = (*m_AryStorages)[Iter];
        CurrentInven->GetInvenSize(X, Y);
        SetGrid(Iter, X, Y);
        Iter++;
    }
//
    (*m_AryStorages)[0]->GetItemChangeCallback().AddUObject(this, &UDiaStorageGridPanel::UpdateSlot1);
    for (int i = 0; i < (*m_AryStorages)[0]->GetItemAry().Num(); i++)
    {
        UpdateSlot1(i, (*m_AryStorages)[0]->GetItemRef(i));
    }
    //
    (*m_AryStorages)[1]->GetItemChangeCallback().AddUObject(this, &UDiaStorageGridPanel::UpdateSlot2);
    for (int i = 0; i < (*m_AryStorages)[1]->GetItemAry().Num(); i++)
    {
        UpdateSlot2(i, (*m_AryStorages)[1]->GetItemRef(i));
    }
    //
    (*m_AryStorages)[2]->GetItemChangeCallback().AddUObject(this, &UDiaStorageGridPanel::UpdateSlot3);
    for (int i = 0; i < (*m_AryStorages)[2]->GetItemAry().Num(); i++)
    {
        UpdateSlot3(i, (*m_AryStorages)[2]->GetItemRef(i));
    }
    //
    (*m_AryStorages)[3]->GetItemChangeCallback().AddUObject(this, &UDiaStorageGridPanel::UpdateSlot4);
    for (int i = 0; i < (*m_AryStorages)[3]->GetItemAry().Num(); i++)
    {
        UpdateSlot4(i, (*m_AryStorages)[3]->GetItemRef(i));
    }
    (*m_AryStorages)[4]->GetItemChangeCallback().AddUObject(this, &UDiaStorageGridPanel::UpdateSlot5);
    for (int i = 0; i < (*m_AryStorages)[4]->GetItemAry().Num(); i++)
    {
        UpdateSlot5(i, (*m_AryStorages)[4]->GetItemRef(i));
    }





    
    m_BtnPanel1->m_Toggle->SetCheckedState(ECheckBoxState::Checked);
    //Open1(true);
}

void UDiaStorageGridPanel::SetGrid(int indexPanel, int x, int y)
{
    int InvenX = x;
    int InvenY = y;

    m_AryArySlots[indexPanel]->Reserve(InvenX * InvenY);

    int Index = 0;
    //(int X = 0; X < InvenX; X++)
    for (int Y = 0; Y < InvenY; Y++)
    {
        for (int X = 0; X < InvenX; X++)
        {
            m_AryGridPanels[indexPanel]->SetColumnFill(X, 1);
            m_AryGridPanels[indexPanel]->SetRowFill(Y, 1);

            UDiaInvenGridSlot* SlotCreated = CreateWidget<UDiaInvenGridSlot>(this, m_ClassGridSlot);
            UGridSlot* ChildSlot = m_AryGridPanels[indexPanel]->AddChildToGrid(SlotCreated);

            ChildSlot->SetColumn(X);
            ChildSlot->SetRow(Y);

            SlotCreated->InitSlot(Index);
            SlotCreated->m_OnDropIndex.BindUObject(this, &UDiaStorageGridPanel::AddItem);

            m_AryArySlots[indexPanel]->Add(SlotCreated);

            Index++;
        }
    }
}


void UDiaStorageGridPanel::UpdateSlot1(int index, FItemInstance& itemInst)
{
    (*m_AryArySlots[0])[index]->SetSlot(itemInst);
}
void UDiaStorageGridPanel::UpdateSlot2(int index, FItemInstance& itemInst)
{
    (*m_AryArySlots[1])[index]->SetSlot(itemInst);
}
void UDiaStorageGridPanel::UpdateSlot3(int index, FItemInstance& itemInst)
{
    (*m_AryArySlots[2])[index]->SetSlot(itemInst);
}
void UDiaStorageGridPanel::UpdateSlot4(int index, FItemInstance& itemInst)
{
    (*m_AryArySlots[3])[index]->SetSlot(itemInst);
}
void UDiaStorageGridPanel::UpdateSlot5(int index, FItemInstance& itemInst)
{
    (*m_AryArySlots[4])[index]->SetSlot(itemInst);
}



bool UDiaStorageGridPanel::AddItem(int index, FItemInstance& itemWantAdd)
{
    return (*m_AryStorages)[m_nCurrentSelectedPanelIndex]->AddItem(index, itemWantAdd);
}

bool UDiaStorageGridPanel::AddItemAuto(FItemInstance& itemWantAdd)
{
    return (*m_AryStorages)[m_nCurrentSelectedPanelIndex]->AddItemAuto(itemWantAdd);
}

void UDiaStorageGridPanel::AddItemStack(int index)
{
    (*m_AryStorages)[m_nCurrentSelectedPanelIndex]->AddItemStack(index);
}


void UDiaStorageGridPanel::Open1(bool bOpen)
{
    UDefaultMenu::Get->CloseItemPopup();
    if (m_nCurrentSelectedPanelIndex == 0)
    {
        return;
    }

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Unchecked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::Collapsed);
    m_nPreSelectedPanelIndex = m_nCurrentSelectedPanelIndex;
    m_nCurrentSelectedPanelIndex = 0;

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Checked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UDiaStorageGridPanel::Open2(bool bOpen)
{
    if (m_nCurrentSelectedPanelIndex == 1)
    {
        return;
    }

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Unchecked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::Collapsed);
    m_nPreSelectedPanelIndex = m_nCurrentSelectedPanelIndex;
    m_nCurrentSelectedPanelIndex = 1;

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Checked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UDiaStorageGridPanel::Open3(bool bOpen)
{
    UDefaultMenu::Get->CloseItemPopup();
    if (m_nCurrentSelectedPanelIndex == 2)
    {
        return;
    }

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Unchecked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::Collapsed);

    m_nPreSelectedPanelIndex = m_nCurrentSelectedPanelIndex;
    m_nCurrentSelectedPanelIndex = 2;

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Checked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UDiaStorageGridPanel::Open4(bool bOpen)
{
    UDefaultMenu::Get->CloseItemPopup();
    if (m_nCurrentSelectedPanelIndex == 3)
    {
        return;
    }

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Unchecked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::Collapsed);

    m_nPreSelectedPanelIndex = m_nCurrentSelectedPanelIndex;
    m_nCurrentSelectedPanelIndex = 3;

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Checked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UDiaStorageGridPanel::Open5(bool bOpen)
{
    UDefaultMenu::Get->CloseItemPopup();
    if (m_nCurrentSelectedPanelIndex == 4)
    {
        return;
    }

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Unchecked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::Collapsed);

    m_nPreSelectedPanelIndex = m_nCurrentSelectedPanelIndex;
    m_nCurrentSelectedPanelIndex = 4;

    m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Checked);
    m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
