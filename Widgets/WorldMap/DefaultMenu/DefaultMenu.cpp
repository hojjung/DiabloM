#include "DefaultMenu.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"
#include "Item/EquipmentSystem.h"
#include "Item/Inventory.h"
#include "Components/CanvasPanel.h"
#include "Item/Stash.h"
#include "Widgets/WorldMap/DefaultMenu/DiaInvenGridPanel.h"


void UDefaultMenu::Init(ADiabloPlayerController* playerCon, APlayerDiabloCharacter* playerChar,
                        UEquipmentSystem* equipment, UInventory* inven)
{
    m_bIsPopupOpened = false;

    m_PlayerCon = playerCon;
    m_PlayerChar = playerChar;
    m_Equipment = equipment;
    m_Inven = inven;

    m_InvenGridPanel->Init(m_Inven);

    m_EquipPanel->Init(m_Equipment);

    m_AryItemPopup.Reset();
    m_AryItemPopup.Add(m_ItemPopup1);
    m_AryItemPopup.Add(m_ItemPopup2);
    m_AryItemPopup.Add(m_ItemPopup3);

    for(auto* PP : m_AryItemPopup)
    {
        PP->GetOnActionEnd().AddUObject(this,&UDefaultMenu::CloseItemPopup);
    }
    
    
    SetPopupDelegate(m_EquipPanel->GetArySlots());
    SetPopupDelegate(m_InvenGridPanel->GetArySlots());

    
}


void UDefaultMenu::SetPopupDelegate(const TArray<UDiaInvenGridSlot*>& arySlots)
{
    for (auto* SlotS : arySlots)
    {
        SlotS->m_OnClicked.AddUObject(this, &UDefaultMenu::OpenItemPopup);
        SlotS->m_OnDragDetect.BindUObject(this, &UDefaultMenu::CloseItemPopup);
    }
}

void UDefaultMenu::OpenMainMenu()
{
    this->SetVisibility((ESlateVisibility::SelfHitTestInvisible));
}

void UDefaultMenu::CloseMainMenu()
{
    this->SetVisibility((ESlateVisibility::Hidden));
    CloseItemPopup();
}

void UDefaultMenu::OpenItemPopup(const FGeometry& geo, FItemInstance& itemInst)
{
    if (m_bIsPopupOpened)
    {
        CloseItemPopup();

        return;
    }

    bool IsEquipable = itemInst.m_ItemData->m_bEquipable;
    //클래스type상 장착 가능해야함
    bool IsStashOpen=false;
    int Count = 0;
    
    if (Cast<UInventory>(itemInst.m_Holder))
    {
        if (!IsEquipable)
        {
            if(IsStashOpen)
            {
                m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Deposite, itemInst);
                m_AryItemPopup[Count]->SetPanelPosition(geo);
            }
            else
            {
                m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::None, itemInst);
                m_AryItemPopup[Count]->SetPanelPosition(geo);
            }
        }
        else
        {
            m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Equip, itemInst);
            m_AryItemPopup[Count]->SetPanelPosition(geo);
            Count++;
            //오른손 왼손으로 고정될것이 아니라
            //해당 장비를 끼울수있는 슬롯이 최대2개까지 나와야한다.
            auto& ArySlots=m_Equipment->GetArySlotPtr();

            for(int i=0; i<ArySlots.Num();i++)
            {
                if(m_Equipment->CheckSlotValid(i,itemInst)&& !ArySlots[i]->m_Item.IsEmpty())
                {
                    m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Unequip, m_Equipment->GetItem(i));
                    m_AryItemPopup[Count]->SetPanelPosition(geo,Count);
                    Count++;
                    if(Count>=m_AryItemPopup.Num())
                    {
                        break;
                    }
                }
            }
            
        }
    }
    else if (Cast<UEquipmentSystem>(itemInst.m_Holder))
    {
          m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Unequip, itemInst);
          m_AryItemPopup[Count]->SetPanelPosition(geo,Count);
    }
    else if (Cast<UStash>(itemInst.m_Holder))
    {
        m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Withdraw, itemInst);
        m_AryItemPopup[Count]->SetPanelPosition(geo,Count);
    }

    m_bIsPopupOpened = true;
}


void UDefaultMenu::CloseItemPopup()
{
    m_bIsPopupOpened = false;

    float Delay = 0.f;
    for (auto* Pop : m_AryItemPopup)
    {
        if(Pop->GetVisibility()==ESlateVisibility::Hidden)
        {
            continue;;
        }
        
        Pop->PlayHideInfoAnim(Delay);
        
        Delay += 0.1f;
    }
}
