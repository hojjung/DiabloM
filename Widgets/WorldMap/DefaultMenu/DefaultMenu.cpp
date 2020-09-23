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
            
            FItemInstance RightWeapon = m_Equipment->GetItem(ESlotsEquipAry::WeaponRight);
            
            if (!RightWeapon.IsEmpty())
            {
                Count++;
                m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Unequip, RightWeapon);
                m_AryItemPopup[Count]->SetPanelPosition(geo,Count);
            }

            FItemInstance LeftWeapon = m_Equipment->GetItem(ESlotsEquipAry::WeaponLeft);

            if (!LeftWeapon.IsEmpty())
            {
                Count++;
                m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Unequip, LeftWeapon);
                m_AryItemPopup[Count]->SetPanelPosition(geo,Count);
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
