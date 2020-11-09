#include "DiaShopPanel.h"
#include "DiaInvenGridSlot.h"
#include "Widgets/WorldMap/DefaultMenu/DefaultMenu.h"
#include "Item/Inventory.h"
#include "Datas/ItemDataTable.h"
#include "DiaStorageGridPanel.h"
#include "Managers/DiabloGameInstance.h"

UDiaShopPanel::UDiaShopPanel(const FObjectInitializer& objInit):Super(objInit)
{
	m_ClassGridSlot = UDiaInvenGridSlot::StaticClass();
	m_nCurrentSelectedPanelIndex=0;

}


void UDiaShopPanel::Init()
{
	m_AryBtns.Reset();
	m_AryBtns.Emplace(m_BtnPanel1);
	m_AryBtns.Emplace(m_BtnPanel2);
	m_AryBtns.Emplace(m_BtnPanel3);

	m_BtnPanel1->OnCheckStateChanged.AddDynamic(this,&UDiaShopPanel::Open1);
	m_BtnPanel2->OnCheckStateChanged.AddDynamic(this,&UDiaShopPanel::Open2);
	m_BtnPanel3->OnCheckStateChanged.AddDynamic(this,&UDiaShopPanel::Open3);
	
	m_AryGridPanels.Reset();
	m_AryGridPanels.Emplace(m_SlotGridPanel1);
	m_AryGridPanels.Emplace(m_SlotGridPanel2);
	m_AryGridPanels.Emplace(m_SlotGridPanel3);

	for(UGridPanel* GridPanel : m_AryGridPanels)
	{
		GridPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	m_SlotGridPanel1->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_AryArySlots.Reset();
	m_AryArySlots.Emplace(&m_ArySlot1);
	m_AryArySlots.Emplace(&m_ArySlot2);
	m_AryArySlots.Emplace(&m_ArySlot3);
	

}

void UDiaShopPanel::SetGrid(int indexPanel,int x, int y)
{
	int InvenX = x;
	int InvenY = y;

	m_AryArySlots[indexPanel]->Reserve(InvenX*InvenY);

	int Index = 0;
	for (int X = 0; X < InvenX; X++)
	{
		for (int Y = 0; Y < InvenY; Y++)
		{
			m_AryGridPanels[indexPanel]->SetColumnFill(X, 1);
			m_AryGridPanels[indexPanel]->SetRowFill(Y, 1);
			
			UDiaInvenGridSlot* SlotCreated = CreateWidget<UDiaInvenGridSlot>(this, m_ClassGridSlot);
			UGridSlot* ChildSlot = m_AryGridPanels[indexPanel]->AddChildToGrid(SlotCreated);
			
			ChildSlot->SetColumn(X);
			ChildSlot->SetRow(Y);
			
			SlotCreated->InitSlot(Index);
			SlotCreated->m_OnDropIndex.BindUObject(this,&UDiaShopPanel::AddItem);
			
			m_AryArySlots[indexPanel]->Add(SlotCreated);
			
			Index++;
		}
	}
}


void UDiaShopPanel::UpdateSlot(int index, FItemInstance& itemInst)
{
	(*m_AryArySlots[m_nCurrentSelectedPanelIndex])[index]->SetSlot(itemInst);
}

bool UDiaShopPanel::AddItem(int index, FItemInstance & itemWantAdd)
{
	return (*m_PtrAryStorages)[m_nCurrentSelectedPanelIndex]->AddItem(index, itemWantAdd);
}

bool UDiaShopPanel::AddItemAuto(FItemInstance& itemWantAdd)
{
	return (*m_PtrAryStorages)[m_nCurrentSelectedPanelIndex]->AddItemAuto(itemWantAdd);
}

void UDiaShopPanel::AddItemStack(int index )
{
	(*m_PtrAryStorages)[m_nCurrentSelectedPanelIndex]->AddItemStack(index);
}

void UDiaShopPanel::Open1(bool bOpen)
{
	if(m_nCurrentSelectedPanelIndex==0)
	{
		return;
	}
	
	m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Unchecked);
	m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::Collapsed);
	
	m_nCurrentSelectedPanelIndex=0;
	
	m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Checked);
	m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
void UDiaShopPanel::Open2(bool bOpen)
{
	if(m_nCurrentSelectedPanelIndex==1)
	{
		return;
	}
	
	m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Unchecked);
	m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::Collapsed);
	
	m_nCurrentSelectedPanelIndex=1;
	
	m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Checked);
	m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
void UDiaShopPanel::Open3(bool bOpen)
{
	if(m_nCurrentSelectedPanelIndex==2)
	{
		return;
	}
	
	m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Unchecked);
	m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::Collapsed);
	
	m_nCurrentSelectedPanelIndex=2;
	
	m_AryBtns[m_nCurrentSelectedPanelIndex]->SetCheckedState(ECheckBoxState::Checked);
	m_AryGridPanels[m_nCurrentSelectedPanelIndex]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UDiaShopPanel::UpdateShop(int panelIndex, TArray<FItemInstance>& itemAdd)
{
	if(panelIndex<0 || panelIndex>2 ||itemAdd.Num()<1)
	{
		return;
	}

	for(FItemInstance& ItemEle : itemAdd)
	{
		AddItemAuto(ItemEle);
	}
}

void UDiaShopPanel::UpdatePanel(AShopKeeper* shop_keeper)
{
	m_PtrAryStorages = shop_keeper->GetShopItemContainer();

	int X=0;
	int Y=0;
	
	int MaxIter=m_PtrAryStorages->Num();
	int Iter=0;
	
	while (Iter<MaxIter)
	{
		UInventory* CurrentInven = (*m_PtrAryStorages)[Iter];
		
		CurrentInven->GetInvenSize(X,Y);
		
		CurrentInven->GetItemChangeCallback().AddUObject(this, &UDiaShopPanel::UpdateSlot);

		SetGrid(Iter,X,Y);

		m_nCurrentSelectedPanelIndex=Iter;
		
		for(int i=0; i<CurrentInven->GetItemAry().Num();i++)
		{
			UpdateSlot(i,CurrentInven->GetItemRef(i));
		}
		
		Iter++;
	}

	m_BtnPanel1->SetCheckedState(ECheckBoxState::Checked);
	Open1(true);
}

void UDiaShopPanel::ClearPanel()
{
	int MaxIter=m_PtrAryStorages->Num();
	int Iter=0;

	FItemInstance Empty;
	
	while (Iter<MaxIter)
	{
		UInventory* CurrentInven = (*m_PtrAryStorages)[Iter];
		
		CurrentInven->GetItemChangeCallback().Clear();

		for(int i=0; i<CurrentInven->GetItemAry().Num();i++)
		{
			UpdateSlot(i,Empty);
		}
		
		Iter++;
	}

	m_PtrAryStorages=nullptr;
}
