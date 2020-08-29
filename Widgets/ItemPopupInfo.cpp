#include "ItemPopupInfo.h"
#include "DiaEquipmentPanel.h"
#include "DiaInvenGridPanel.h"
#include "DiaInvenGridSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "WidgetLayoutLibrary.h"
#include "Components/VerticalBoxSlot.h"
#include "Animation/UMGSequencePlayer.h"
#include "Kismet/KismetTextLibrary.h"
#include "Managers/ItemManager.h"
#include "Characters/DiabloPlayerController.h"



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

    FString ItemTypeName = "EItemType";

    m_ItemTypeString = FindObject<UEnum>(ANY_PACKAGE, *ItemTypeName);

    GetUseButton()->OnClicked.AddDynamic(this, &UItemPopupInfo::UseItem);
    //GetEquipButton()->OnClicked.AddDynamic(this,&UItemPopupInfo::EquipItem);

    m_SelectedItem = nullptr;

    //NativeOnMouseButtonDoubleClick()
}

FReply UItemPopupInfo::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    auto Rep = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    PlayHideInfoAnim();

    return Rep;
}

void UItemPopupInfo::UseItem()
{
    //item use
    //m_SelectedItem->m_ItemData->
    PRINTF("Use[!");
}

void UItemPopupInfo::EquipItem()
{
    PRINTF("Equip");

    if (!m_SelectedItem)
    {
        return;
    }
    auto& Arys = UDiaEquipmentPanel::GetEquipWidgetInst->GetArySlots();

    for (auto* EquipSlot : Arys)
    {
        if (UDiaEquipmentPanel::GetEquipWidgetInst->EquipItem(EquipSlot->GetIndex(), *m_SelectedItem))
        {
            PlayHideInfoAnim();
            //끼운 아이템을 삭제
            return;
        }
    }
}

void UItemPopupInfo::UnequipItem()
{
    PRINTF("UnEquip");
    if (!m_SelectedItem)
    {
        return;
    }
    //inven 에 공간 먼저 확인

    int RemoveWantIndex=m_SelectedItem->m_nGridIndex;
    
    if (!UDiaInvenGridPanel::GetInvenWidgetInst->AddItemAuto(*m_SelectedItem))
    {
        return;
    }
    
    //UDiaEquipmentPanel::GetEquipWidgetInst->UnequipItem(RemoveWantIndex);
}


//NativeOnInitialized
void UItemPopupInfo::NativePreConstruct()
{
    Super::NativePreConstruct();
}

FText UItemPopupInfo::GetItemTypeTxt(EItemType typeV) const
{
    return FText::FromString(m_ItemTypeString->GetNameStringByIndex(static_cast<uint8>(typeV)));
}

void UItemPopupInfo::SetIcon(const FItemInstance& itemInst)
{
    FSlateBrush BrushWant;

    BrushWant.SetImageSize(FVector2D(64.f, 64.f));
    BrushWant.SetResourceObject(ItemManager::GetItemData(itemInst).m_ItemIcon);

    m_ImageItemVisualIcon->SetBrush(BrushWant);
}

void UItemPopupInfo::SetColorTier(const FItemInstance& itemInst)
{
    const auto ColorW = ItemManager::GetItemData(itemInst).GetItemTier().m_TierColor;
    m_ImageItemTierColorSmall->SetColorAndOpacity(ColorW);
    m_ImageItemTierColorLarge->SetColorAndOpacity(ColorW);
    m_TextItemName->SetColorAndOpacity(ColorW);
    m_TextItemTierAndType->SetColorAndOpacity(ColorW);
}

void UItemPopupInfo::HideAllSubOptions()
{
    for (auto* Options : m_AryOptions)
    {
        Options->SetString(FText());
        Options->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UItemPopupInfo::HideFlavorText()
{
    m_TextFlavor->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemPopupInfo::SetPanelPosition(const FGeometry& theInstigator)
{
    //TODO: canvas 에 맞춰 왼쪽 오른쪽 조절

    auto Geo = UWidgetLayoutLibrary::GetPlayerScreenWidgetGeometry(GetOwningPlayer());

    auto* PanelSlot = Cast<UCanvasPanelSlot>(Slot);

    auto* CanvasPanelParent = Cast<UCanvasPanel>(PanelSlot->Parent);

    auto ClickedItemSlot = CanvasPanelParent->GetCachedGeometry().AbsoluteToLocal(theInstigator.GetAbsolutePosition()) +
        theInstigator.GetLocalSize() / 2.0f;

    ClickedItemSlot.X -= (GetDesiredSize().X / 2.0f) + (theInstigator.GetLocalSize().X / 2.0f);

    //CanvasPanelParent->ForceLayoutPrepass();

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
        PRINTF("Popup 1");
    }
    else if (ScreenBottomToItem < PopupSizeY) //아래 공간이 팝업창보다 작을때
    {
        float Diff = PopupSizeY - FMath::Abs(ScreenBottomToItem);

        ClickedItemSlot.Y -= Diff * ReverseScale;

        //	ClickedItemSlot.Y -= 50.f;
        PRINTF("Popup 2");
    }


    PanelSlot->SetPosition(ClickedItemSlot);
}

void UItemPopupInfo::ShowInfoPanel(FItemInstance& itemInst)
{
    SetRenderOpacity(1.f);
    m_BGForTouch->SetVisibility(ESlateVisibility::Visible);
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    PlayAnimation(m_FadeAnimation);
    HideAllSubOptions();

    SetIcon(itemInst);
    SetColorTier(itemInst);
    SetItemText(itemInst);

    SetOptionTexts(itemInst);

    SetFlavorText(itemInst);

    m_SelectedItem = &itemInst;

    if (ItemManager::GetItemData(*m_SelectedItem).m_bEquipable)
    {
        m_EquipButton->SetVisibility(ESlateVisibility::Visible);

        IItemHolder* Holder = m_SelectedItem->m_Holder;

        GetEquipButton()->OnClicked.Clear();

        auto* DiaChar= Cast<ADiabloPlayerController>( UGameplayStatics::GetPlayerController(GetWorld(),0));
        
        if (static_cast<EquipmentSystem*>(Holder) ==DiaChar->GetEquipment())
        {
            PRINTF("EquipSys");
            GetEquipButton()->OnClicked.AddDynamic(this, &UItemPopupInfo::UnequipItem);
        }
        else if (static_cast<Inventory*>(Holder) == DiaChar->GetInven())
        {
            PRINTF("Inven");
            GetEquipButton()->OnClicked.AddDynamic(this, &UItemPopupInfo::EquipItem);
        }
        //GetEquipButton()->OnClicked.AddDynamic(this,&UItemPopupInfo::EquipItem);
    }

    ForceLayoutPrepass();
}


void UItemPopupInfo::SetItemText(const FItemInstance& itemInst)
{
    //m_SelectedItem
    m_TextItemName->SetText(ItemManager::GetItemData(itemInst).m_ShowingName);

    FText ItemNameT = ItemManager::GetItemData(itemInst).GetItemTier().m_ShowingName;

    FText ItemTypeT = GetItemTypeTxt(ItemManager::GetItemData(itemInst).m_ItemType);

    FFormatOrderedArguments Args;
    Args.Add(ItemNameT);
    Args.Add(ItemTypeT);

    FTextFormat FormatT = FText::FromString("{0} {1}");

    m_TextItemTierAndType->SetText(FText::Format(FormatT, Args));

    m_TextSellValue->SetString(UKismetTextLibrary::Conv_IntToText(ItemManager::GetItemData(itemInst).m_nSellValue));
}

float UItemPopupInfo::SetFlavorText(const FItemInstance& itemInst)
{
    m_TextFlavor->SetText(ItemManager::GetItemData(itemInst).m_FlavorText);
    m_TextFlavor->ForceLayoutPrepass();
    return m_TextFlavor->GetDesiredSize().Y;
}

float UItemPopupInfo::SetOptionTexts(const FItemInstance& itemInst)
{
    float OptionSizeY = 0.f;

    int OptionCount = itemInst.m_AryOptions.Num();

    int i = 0;

    while (i < OptionCount)
    {
        m_AryOptions[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        m_AryOptions[i]->SetString(
            ItemManager::GetItemData(itemInst).GetOption(i).GetOptionFormat(itemInst.m_AryOptions[i].m_fValue));
        m_AryOptions[i]->ForceLayoutPrepass();
        OptionSizeY += m_AryOptions[i]->GetDesiredSize().Y;
        i++;
    }

    return OptionSizeY;
}

void UItemPopupInfo::PlayHideInfoAnim()
{
    if (!m_SelectedItem)
    {
        return;
    }
    m_BGForTouch->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    m_SelectedItem = nullptr;
    PlayAnimationReverse(m_FadeAnimation);
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UItemPopupInfo::HideInfoPanel,
                                           m_FadeAnimation->GetEndTime(), false);
}

void UItemPopupInfo::HideInfoPanel()
{
    m_SelectedItem = nullptr;
    SetVisibility(ESlateVisibility::Collapsed);
    m_UseButton->SetVisibility(ESlateVisibility::Hidden);
    m_EquipButton->SetVisibility(ESlateVisibility::Hidden);
}
