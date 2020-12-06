#include "DefaultMenu.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Item/DiaInvenGridPanel.h"
#include "Item/DiaInvenGridSlot.h"
#include "Managers/DiabloGameInstance.h"
#include "Item/EquipmentSystem.h"
#include "Item/Inventory.h"
#include "Item/ItemDrop.h"
#include "Item/ItemPopupInfo.h"
#include "Shop/DiaShopGridSlot.h"
#include "Skills/DiaSkillPopup.h"
#include "Village/ShopKeeper.h"


UDefaultMenu*  UDefaultMenu::Get=nullptr;

void UDefaultMenu::Init(ADiabloPlayerController* playerCon, APlayerDiabloCharacter* playerChar,
                        UEquipmentSystem* equipment, UInventory* inven, TArray<UInventory*>* aryStorage)
{
    Get=this;
    m_bIsPopupOpened = false;
    m_bIsStorageOpened = false;
    m_bIsSkillOpened=false;
    m_PlayerCon = playerCon;
    m_PlayerChar = playerChar;
    m_Equipment = equipment;
    m_Inven = inven;
    m_Storage = aryStorage;

    m_InvenGridPanel->Init(m_Inven);
    SetPopupDelegate(m_InvenGridPanel->GetArySlots());

    m_EquipPanel->Init(m_Equipment);
    SetPopupDelegate(m_EquipPanel->GetArySlots());

    m_StoragePanel->Init(m_Storage);

    for (TArray<UDiaInvenGridSlot*>* ArySlot1 : m_StoragePanel->GetArySlots2())
    {
        SetPopupDelegate(*ArySlot1);
    }

    InitPopup();

    m_StatPanel->Init(playerChar);

    CloseStorage();

    m_ShopPanel->Init();

    for (TArray<UDiaShopGridSlot*>* ArySlot1 : m_ShopPanel->GetArySlots3())
    {
        SetPopupDelegateToShopPanel(ArySlot1);
    }

    CloseShopMenu();

    m_PlayerChar->GetOnGoldChanged().AddUObject(m_InvenGridPanel, &UDiaInvenGridPanel::UpdateGold);
    //Skill
    m_SkillPanel->Init(Cast<UPlayerDiabloAbilitySystemComp>(m_PlayerChar->GetAbilitySystemComponent()));
    
    CloseSkillPanel();

    m_SkillPopup->Init(Cast<UPlayerDiabloAbilitySystemComp>( playerChar->GetAbilitySystemComponent()));
    
    for(auto* LearnBtn: m_SkillPanel->GetAllSkillLearnBtn())
    {
        LearnBtn->m_OnClicked.AddUObject(this,&UDefaultMenu::OpenSkillPopup);
        LearnBtn->m_OnDragDetect.BindUObject(this,&UDefaultMenu::CloseSkillPopup);
    }

    //CloseSkillPopup();
}

void UDefaultMenu::InitPopup()
{
    m_AryItemPopup.Reset();
    m_AryItemPopup.Add(m_ItemPopup1);
    m_AryItemPopup.Add(m_ItemPopup2);
    m_AryItemPopup.Add(m_ItemPopup3);

    for (auto* PP : m_AryItemPopup)
    {
        PP->GetOnActionEnd().AddUObject(this, &UDefaultMenu::CloseItemPopup);
    }
}

UDefaultMenu::~UDefaultMenu()
{
    UDefaultMenu::Get=nullptr;
}

void UDefaultMenu::SetPopupDelegate(const TArray<UDiaInvenGridSlot*>& arySlots)
{
    for (auto* SlotS : arySlots)
    {
        SlotS->m_OnClicked.AddUObject(this, &UDefaultMenu::OpenItemPopup);
        SlotS->m_OnDragDetect.BindUObject(this, &UDefaultMenu::CloseItemPopup);
    }
}
void UDefaultMenu::SetPopupDelegateToShopPanel(const TArray<UDiaShopGridSlot*>* ArySlot1)
{
    for (UDiaShopGridSlot* SlotS : *ArySlot1)
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

    if(m_bIsSkillOpened)
    {
        CloseSkillPanel();
    }

    if (m_bIsStorageOpened)
    {
        CloseStorage();
    }

    if (m_bIsShopOpened)
    {
        CloseShopMenu();
    }
}

void UDefaultMenu::CompareItem(UItemPopupInfo* wantEquip, UItemPopupInfo* equippedOld)
{
    FOptionSpec WantEquipOption = wantEquip->GetSelectedItem().m_AryOptions[0];
    FOptionSpec EquippedOption = equippedOld->GetSelectedItem().m_AryOptions[0];

    if (WantEquipOption.m_OptionID != EquippedOption.m_OptionID)
    {
        return;
    }

    wantEquip->CompareItem(EquippedOption.m_fValue, WantEquipOption.m_fValue); //
}

void UDefaultMenu::OpenItemPopup(const FGeometry& geo, FItemInstance& itemInst)
{
    if (m_bIsPopupOpened)
    {
        if (
            m_AryItemPopup[0]->GetSelectedItem().m_nGridIndex == itemInst.m_nGridIndex
            && m_AryItemPopup[0]->GetSelectedItem().m_Holder == itemInst.m_Holder
            && m_AryItemPopup[0]->GetSelectedItem().m_ItemData == itemInst.m_ItemData)
        {
            CloseItemPopup();
            return;
        }
        CloseItemPopup();
    }

    bool IsEquipable = itemInst.m_ItemData->m_bEquipable;

    bool IsStashOpen = m_bIsStorageOpened;

    bool IsShopOpen = m_bIsShopOpened;

    int Count = 0;

    if (itemInst.m_Holder == m_Inven) //인벤 클릭일때
    {
        if (!IsEquipable) //장착 불가 아이템
        {
            if (IsStashOpen) //창고가 열려있으며
            {
                m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Deposite, itemInst);
            }
            else if (IsShopOpen) //상점이 열려있으면
            {
                m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Sell, itemInst);
            }
            else //열려있지 않으면
            {
                m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::None, itemInst);
            }

            m_AryItemPopup[Count]->SetPanelPosition(geo);
        }
        else //장착 가능 아이템
        {
            if (IsStashOpen) //창고가 열려있으며
            {
                m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Deposite, itemInst);
            }
            else if (IsShopOpen) //상점이 열려있으면
            {
                m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Sell, itemInst);
            }
            else
            {
                m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Equip, itemInst);
            }

            m_AryItemPopup[Count]->SetPanelPosition(geo);

            Count++;
            //오른손 왼손으로 고정될것이 아니라
            //해당 장비를 끼울수있는 슬롯이 최대2개까지 나와야한다.
            auto& ArySlots = m_Equipment->GetArySlotPtr();

            for (int i = 0; i < ArySlots.Num(); i++)
            {
                if (m_Equipment->CheckSlotValid(i, itemInst) && !ArySlots[i]->m_Item.IsEmpty())
                {
                    m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Unequip, m_Equipment->GetItem(i));
                    m_AryItemPopup[Count]->SetPanelPosition(geo, Count);
                    Count++;
                    if (Count >= m_AryItemPopup.Num())
                    {
                        break;
                    }
                }
            }


            if (Count >= 2)
            {
                CompareItem(m_AryItemPopup[0], m_AryItemPopup[1]);
            }
        }
    }
    else if (Cast<UEquipmentSystem>(itemInst.m_Holder)) //장비칸 클릭일때
    {
        m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Unequip, itemInst);
        m_AryItemPopup[Count]->SetPanelPosition(geo, Count);
    }
    else if (IsStashOpen) //창고 클릭일때
    {
        m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Withdraw, itemInst);
        m_AryItemPopup[Count]->SetPanelPosition(geo, Count, false);
        PRINTF("StashOpen");
    }
    else if (IsShopOpen) //창고 클릭일때
    {
        m_AryItemPopup[Count]->ShowInfoPanel(EPopupType::Buy, itemInst);
        m_AryItemPopup[Count]->SetPanelPosition(geo, Count, false);
        PRINTF("ShopOpen");
    }


    m_bIsPopupOpened = true;
}


void UDefaultMenu::CloseItemPopup()
{
    m_bIsPopupOpened = false;

    float Delay = 0.f;
    for (UItemPopupInfo* Pop : m_AryItemPopup)
    {
        if (Pop->GetVisibility() == ESlateVisibility::Hidden)
        {
            continue;
        }

        Pop->PlayHideInfoAnim(Delay);

        Delay += 0.1f;
    }
}

void UDefaultMenu::OpenStorage()
{
    m_StoragePanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_StatPanel->SetVisibility(ESlateVisibility::Collapsed);
    m_bIsStorageOpened = true;
}

void UDefaultMenu::CloseStorage()
{
    m_StoragePanel->SetVisibility(ESlateVisibility::Collapsed);
    m_StatPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_bIsStorageOpened = false;
}

void UDefaultMenu::OpenShopMenu(AShopKeeper* shopKeeper)
{
    m_StatPanel->SetVisibility(ESlateVisibility::Collapsed);
    m_ShopPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_ShopPanel->UpdatePanel(shopKeeper);
    m_bIsShopOpened = true;
}

void UDefaultMenu::CloseShopMenu()
{
    m_StatPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_ShopPanel->SetVisibility(ESlateVisibility::Collapsed);
    m_ShopPanel->ClearPanel();
    m_bIsShopOpened = false;
}

UDiaShopPanel* UDefaultMenu::GetShopPanelWidget()
{
    return m_ShopPanel;
}

void UDefaultMenu::OpenSkillPanel()
{
    m_SkillPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_TalentPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_InvenGridPanel->SetVisibility(ESlateVisibility::Collapsed);
    m_EquipPanel->SetVisibility(ESlateVisibility::Collapsed);
    m_StatPanel->SetVisibility(ESlateVisibility::Collapsed);
    m_ItemDropPanel->SetVisibility(ESlateVisibility::Hidden);
    m_bIsSkillOpened=true;
}

void UDefaultMenu::CloseSkillPanel()
{
    m_SkillPanel->SetVisibility(ESlateVisibility::Collapsed);
    m_TalentPanel->SetVisibility(ESlateVisibility::Collapsed);
    m_InvenGridPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_EquipPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_StatPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_ItemDropPanel->SetVisibility(ESlateVisibility::Visible);
    m_bIsSkillOpened=false;
    m_SkillPopup->HidePopup();
}

void UDefaultMenu::OpenSkillPopup(const FGeometry& geo,FSkillDataSpec& skillSpec)
{
    m_SkillPopup->SetPopupSkillData(&skillSpec,geo);
}

void UDefaultMenu::CloseSkillPopup()
{
    m_SkillPopup->PlayHideInfoAnim();
}
