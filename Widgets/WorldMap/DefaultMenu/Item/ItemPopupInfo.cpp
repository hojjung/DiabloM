
#include "ItemPopupInfo.h"
#include "DiaEquipmentPanel.h"
#include "DiaInvenGridPanel.h"
#include "DiaInvenGridSlot.h"
#include "Animation/UMGSequencePlayer.h"
#include "Characters/DiabloPlayerController.h"
#include "Managers/DiabloGameMode.h"
#include "Widgets/WorldMap/DefaultMenu/DefaultMenu.h"
#include "Widgets/WorldMap/DefaultMenu/Storage/DiaStorageGridPanel.h"


void UItemPopupInfo::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    m_AryOptions.Reset(13);
    m_AryOptions.Emplace(m_SubOption1);
    m_AryOptions.Emplace(m_SubOption2);
    m_AryOptions.Emplace(m_SubOption3);
    m_AryOptions.Emplace(m_SubOption4);
    m_AryOptions.Emplace(m_SubOption5);
    m_AryOptions.Emplace(m_SubOption6);
    m_AryOptions.Emplace(m_SubOption7);
    m_AryOptions.Emplace(m_SubOption8);
    m_AryOptions.Emplace(m_SubOption9);
    m_AryOptions.Emplace(m_SubOption10);
    m_AryOptions.Emplace(m_SubOption11);

    m_SelectedItem.ClearData();
    m_UseButton->OnClicked.AddDynamic(this, &UItemPopupInfo::UseItem);
    m_EquipButton->OnClicked.AddDynamic(this, &UItemPopupInfo::EquipItem);
    m_UnequipButton->OnClicked.AddDynamic(this, &UItemPopupInfo::UnequipItem);
    m_DepositeButton->OnClicked.AddDynamic(this, &UItemPopupInfo::DepositeItem);
    m_WithdrawButton->OnClicked.AddDynamic(this, &UItemPopupInfo::WithdrawItem);
    m_SellButton->OnClicked.AddDynamic(this, &UItemPopupInfo::SellItem);
    m_BuyButton->OnClicked.AddDynamic(this, &UItemPopupInfo::BuyItem);

    UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);
    m_InitPos = PanelSlot->GetPosition();


    m_AnimEndDele.BindDynamic(this,&UItemPopupInfo::HideInfoPanel);
}

FReply UItemPopupInfo::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FReply Rep = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    PlayHideInfoAnim();
    m_OnActionEnd.Broadcast();
    return Rep;
}

void UItemPopupInfo::PopupBtnToggle(EPopupType popup)
{
    m_UseButton->SetVisibility(ESlateVisibility::Hidden);
    m_EquipButton->SetVisibility(ESlateVisibility::Hidden);
    m_UnequipButton->SetVisibility(ESlateVisibility::Hidden);
    m_WithdrawButton->SetVisibility(ESlateVisibility::Hidden);
    m_DepositeButton->SetVisibility(ESlateVisibility::Hidden);
    m_SellButton->SetVisibility(ESlateVisibility::Hidden);
    m_BuyButton->SetVisibility(ESlateVisibility::Hidden);
    
 switch (popup)
    {
    case EPopupType::None: break;
    case EPopupType::Deposite:m_DepositeButton->SetVisibility(ESlateVisibility::Visible); break;
    case EPopupType::Withdraw:m_WithdrawButton->SetVisibility(ESlateVisibility::Visible); break;
    case EPopupType::Equip:m_EquipButton->SetVisibility(ESlateVisibility::Visible); break;
    case EPopupType::Unequip:m_UnequipButton->SetVisibility(ESlateVisibility::Visible); break;
    case EPopupType::Sell:m_SellButton->SetVisibility(ESlateVisibility::Visible); break;
    case EPopupType::Buy:m_BuyButton->SetVisibility(ESlateVisibility::Visible); break;
    default: ;
    }
}

void UItemPopupInfo::UseItem()
{
    //item use
    //m_SelectedItem->m_ItemData->
    PRINTF("Use[!");
    PlayHideInfoAnim();
    m_OnActionEnd.Broadcast();
}

void UItemPopupInfo::EquipItem()
{
    PRINTF("POPUP Equip");

    if (m_SelectedItem.IsEmpty())
    {
        return;
    }
    auto& Arys = UDiaEquipmentPanel::GetEquipWidgetInst->GetArySlots();


    for (UDiaInvenGridSlot* EquipSlot : Arys)
    {
        if (EquipSlot->IsSlotEmpty())
        {
            if (UDiaEquipmentPanel::GetEquipWidgetInst->EquipItem(EquipSlot->GetIndex(), m_SelectedItem))
            {
                PlayHideInfoAnim();
                m_OnActionEnd.Broadcast();
                //끼운 아이템을 삭제
                return;
            }
        }
    }
    for (auto* EquipSlot : Arys)
    {
        if (UDiaEquipmentPanel::GetEquipWidgetInst->EquipItem(EquipSlot->GetIndex(), m_SelectedItem))
        {
            PlayHideInfoAnim();
            m_OnActionEnd.Broadcast();
            //끼운 아이템을 삭제
            return;
        }
    }
}

void UItemPopupInfo::UnequipItem()
{
    PRINTF("UnEquip");
    if (m_SelectedItem.IsEmpty())
    {
        return;
    }

    if (UDiaInvenGridPanel::GetInvenWidgetInst->AddItemAuto(m_SelectedItem))
    {
        PlayHideInfoAnim();
        m_OnActionEnd.Broadcast();
    }
}

void UItemPopupInfo::DepositeItem()
{
    if (m_SelectedItem.IsEmpty())
    {
        return;
    }

    if (UDiaStorageGridPanel::GetStorageWidgetInst->AddItemAuto(m_SelectedItem))
    {
        PlayHideInfoAnim();
        m_OnActionEnd.Broadcast();
    }
}

void UItemPopupInfo::WithdrawItem()
{
    if (m_SelectedItem.IsEmpty())
    {
        return;
    }

    if (UDiaInvenGridPanel::GetInvenWidgetInst->AddItemAuto(m_SelectedItem))
    {
        PlayHideInfoAnim();
        m_OnActionEnd.Broadcast();
    }
}

void UItemPopupInfo::BuyItem()
{
    if (m_SelectedItem.IsEmpty())
    {
        return;
    }

    if(ADiabloPlayerController::Get->GetMainCanvas()->GetMainMenu()->GetShopPanelWidget()->BuyItemAuto(m_SelectedItem))
    {
        PlayHideInfoAnim();
        m_OnActionEnd.Broadcast();
    }
    else
    {
        PRINTF("Bought but no space");
    }
}

void UItemPopupInfo::SellItem()
{
    if (m_SelectedItem.IsEmpty())
    {
        return;
    }

    if(ADiabloPlayerController::Get->GetMainCanvas()->GetMainMenu()->GetShopPanelWidget()->SellItemAuto(-1,m_SelectedItem))
    {
        PlayHideInfoAnim();
        m_OnActionEnd.Broadcast();
    }
}

void UItemPopupInfo::SetIcon(const FItemInstance& itemInst)
{
    FSlateBrush BrushWant;

    BrushWant.SetImageSize(FVector2D(128.f, 128.f));
    BrushWant.SetResourceObject(itemInst.m_ItemData->m_ItemIcon);

    m_ImageItemVisualIcon->SetBrush(BrushWant);
}

void UItemPopupInfo::SetColorTier(const FItemInstance& itemInst)
{
    const FLinearColor ColorW = itemInst.m_ItemTier->m_TierColor;
    m_ImageItemTierColorSmall->SetColorAndOpacity(ColorW);
    m_ImageItemTierColorLarge->SetColorAndOpacity(ColorW);
    m_TextItemName->SetColorAndOpacity(ColorW);
    m_TextItemTier->SetColorAndOpacity(ColorW);
    m_TextItemType->SetColorAndOpacity(ColorW);
}

void UItemPopupInfo::HideAllSubOptions()
{
    for (UImageAndText* Options : m_AryOptions)
    {
        Options->SetString(FText());
        Options->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UItemPopupInfo::HideFlavorText()
{
    m_TextFlavor->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemPopupInfo::SetPanelPosition(const FGeometry& theInstigator, int countSpace, bool bLeft)
{
    //TODO: canvas 에 맞춰 왼쪽 오른쪽 조절

    FGeometry&& Geo = UWidgetLayoutLibrary::GetPlayerScreenWidgetGeometry(GetOwningPlayer());

    UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);

    UCanvasPanel* CanvasPanelParent = Cast<UCanvasPanel>(PanelSlot->Parent);

    FVector2D ClickedItemSlot = CanvasPanelParent->GetCachedGeometry().AbsoluteToLocal(
            theInstigator.GetAbsolutePosition()) +
        theInstigator.GetLocalSize() / 2.0f;

    if (bLeft)
    {
        ClickedItemSlot.X -= (GetDesiredSize().X / 2.0f) + (theInstigator.GetLocalSize().X / 2.0f) + (GetDesiredSize().X*countSpace);
    }
    else
    {
        ClickedItemSlot.X += (GetDesiredSize().X / 2.0f) + (theInstigator.GetLocalSize().X / 2.0f) + (GetDesiredSize().X*countSpace);
    }

    float ScreenY = Geo.GetAbsoluteSize().Y;

    float PopupSizeY = (GetDesiredSize().Y * Geo.Scale) / 2.0f + 50.f;

    float ScreenTopToItem = ClickedItemSlot.Y * Geo.Scale;

    float ScreenBottomToItem = ScreenY - ClickedItemSlot.Y * Geo.Scale;

    float ReverseScale = 1.f / Geo.Scale;


    if (ScreenTopToItem < PopupSizeY)
    {
        float Diff = PopupSizeY - ScreenTopToItem;

        ClickedItemSlot.Y += Diff * ReverseScale;
        ClickedItemSlot.Y -= 50.f;
    }
    else if (ScreenBottomToItem < PopupSizeY) //아래 공간이 팝업창보다 작을때
    {
        float Diff = PopupSizeY - FMath::Abs(ScreenBottomToItem);

        ClickedItemSlot.Y -= Diff * ReverseScale;

        //	ClickedItemSlot.Y -= 50.f;
    }

    PanelSlot->SetPosition(ClickedItemSlot);
}

void UItemPopupInfo::ShowInfoPanel(EPopupType popupType, FItemInstance& itemInst)
{
    UnbindAllFromAnimationFinished(m_FadeAnimation);
    
    //SetRenderOpacity(1.f);
    m_BGForTouch->SetVisibility(ESlateVisibility::Visible);
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    PlayAnimation(m_FadeAnimation);
    HideAllSubOptions();

    SetIcon(itemInst);
    SetColorTier(itemInst);
    SetItemText(itemInst,popupType);

    if(itemInst.m_ItemData->m_ItemType.GetRow<FItemType>("")->m_bEquipable)
    {
        SetOptionTexts(itemInst);
    }
    else
    {
        m_MainOption->SetString(FText());
        
        m_MainOption->SetVisibility(ESlateVisibility::Collapsed);
        
        HideAllSubOptions();
    }

    SetFlavorText(itemInst);

    m_SelectedItem = itemInst;

    PopupBtnToggle(popupType);

    ForceLayoutPrepass();
}


void UItemPopupInfo::SetItemText(const FItemInstance& itemInst,EPopupType popupType)
{
    //m_SelectedItem
    m_TextItemName->SetText(itemInst.m_ItemData->m_ShowingName);

    FText ItemTier = itemInst.m_ItemTier->m_ShowingName;

    FText ItemTypeT = itemInst.m_ItemData->m_ItemType.GetRow<FItemType>("")->m_ShowingName;
    
    m_TextItemTier->SetText(ItemTier);

    m_TextItemType->SetText(ItemTypeT);

    switch (popupType)
    {
      case EPopupType::Buy:
          m_TextSellValue->SetString(UKismetTextLibrary::Conv_IntToText(itemInst.m_fBuyCost));
              break;
        default:
          m_TextSellValue->SetString(UKismetTextLibrary::Conv_IntToText(itemInst.GetFullStackSellValue()));
    }
    
    
}

float UItemPopupInfo::SetFlavorText(const FItemInstance& itemInst)
{
    m_TextFlavor->SetText(itemInst.m_ItemData->m_FlavorText);
    m_TextFlavor->ForceLayoutPrepass();
    return m_TextFlavor->GetDesiredSize().Y;
}

float UItemPopupInfo::SetOptionTexts(const FItemInstance& itemInst)
{
    m_MainOption->HideCompare();
    m_MainOption->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_MainOption->SetIcon(itemInst.m_AryOptions[0].m_DataOption->m_OptionIcon);
    m_MainOption->SetString(itemInst.m_AryOptions[0].GetOptionText());
    //옵션의 종류가 같을때만?
    //미장착이라면 무조건 높게?
    //m_MainOptionAttack->ShowCompare()
    //TODO Set Attribute From Option,and get and compare

    float OptionSizeY = 0.f;

    int OptionCount = itemInst.m_AryOptions.Num();

    int i = 1;

    if (OptionCount > 1)
    {
        while (i < OptionCount)
        {
            float Value = itemInst.m_AryOptions[i].m_fValue;
            m_AryOptions[i]->SetIcon(itemInst.m_AryOptions[i].m_DataOption->m_OptionIcon);
            m_AryOptions[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
            m_AryOptions[i]->SetString(itemInst.m_AryOptions[i].GetOptionText());
            m_AryOptions[i]->ForceLayoutPrepass();
            OptionSizeY += m_AryOptions[i]->GetDesiredSize().Y;
            i++;
        }
    }

    return OptionSizeY;
}

void UItemPopupInfo::PlayHideInfoAnim(float delay)
{
    if (m_SelectedItem.IsEmpty())
    {
        return;
    }
    
    
    
    BindToAnimationFinished(m_FadeAnimation, m_AnimEndDele);

    m_BGForTouch->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    
    m_SelectedItem.ClearData();
    
    PlayAnimationReverse(m_FadeAnimation);
}

void UItemPopupInfo::HideInfoPanel()
{
    PRINTF("HideInfoPanel");
    
    m_SelectedItem.ClearData();
    
    SetVisibility(ESlateVisibility::Collapsed);
    
    m_UseButton->SetVisibility(ESlateVisibility::Hidden);
    
    m_EquipButton->SetVisibility(ESlateVisibility::Hidden);
    
    UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);
    
    PanelSlot->SetPosition(m_InitPos);

    //UnbindAllFromAnimationFinished(m_FadeAnimation);
}

void UItemPopupInfo::CompareItem(float origin, float wantCompareOther)
{
    m_MainOption->ShowCompare(origin, wantCompareOther);
}
