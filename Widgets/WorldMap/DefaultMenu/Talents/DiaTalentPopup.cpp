#include "DiaTalentPopup.h"
#include "AbilitySystem/Components/PlayerDiabloAbilitySystemComp.h"

#define LOCTEXT_NAMESPACE "DiaTalentPopup"

UDiaTalentPopup::UDiaTalentPopup(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
    m_PlayerDiaComp = nullptr;
    m_CurrentTalent = nullptr;
}

void UDiaTalentPopup::Init(UPlayerDiabloAbilitySystemComp* plDiaComp)
{
    m_PlayerDiaComp = plDiaComp;

    m_BtnLearn->OnClicked.AddDynamic(this, &UDiaTalentPopup::LearnTalent);

    m_AnimEndDele.BindDynamic(this, &UDiaTalentPopup::HidePopup);

    UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);

    m_InitPos = PanelSlot->GetPosition();

    m_TextRequirePointLimit->SetText(LOCTEXT("RequirePointLog",
                                             "Spend more points in this tree to \n unlock this talent"));
}

void UDiaTalentPopup::SetTalentIcon()
{
    m_ImageTalentIcon->SetBrush(m_CurrentTalent->m_TechIcon);
}

void UDiaTalentPopup::SetTalentName()
{
    m_TextTalentName->SetText(m_CurrentTalent->m_TalentShowingName);
}

void UDiaTalentPopup::SetTalentRank()
{
    m_TextTalentRank->SetText(m_CurrentTalent->GetLevelFormatText());
}

void UDiaTalentPopup::SetTalentDesc()
{
    m_TextTalentDesc->SetText(m_CurrentTalent->GetTalentDesc());

    m_TextTalentDesc->ForceLayoutPrepass();
}

void UDiaTalentPopup::SetRequirePointAlertShow()
{
    if(!m_CurrentTalent->IsLevelupable()&&!m_PlayerDiaComp->IsTalentUnlock(m_CurrentTalent))
    {
        m_OverlayRequireLimit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        m_TextRequirePointLimit->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        //부족한 포인트 계산해서 말해줄것?
    }
    else
    {
        m_OverlayRequireLimit->SetVisibility(ESlateVisibility::Collapsed);
        m_TextRequirePointLimit->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UDiaTalentPopup::SetAdditionalInfoShow()
{
    if (m_CurrentTalent->m_TextSkillAdditionalEffect.IsEmpty())
    {
        m_TextAdditionalInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        m_TextAdditionalInfo->SetText(m_CurrentTalent->m_TextSkillAdditionalEffect);
    }
    else
    {
        m_TextAdditionalInfo->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UDiaTalentPopup::SetTalentLevelupPreview()
{
    if (!m_CurrentTalent->IsLevelupable()&&!m_PlayerDiaComp->IsTalentUnlock(m_CurrentTalent))
    {
        m_OverlayNextPreview->SetVisibility(ESlateVisibility::Collapsed);
        
        m_BtnLearn->SetVisibility(ESlateVisibility::Collapsed);

        return;
    }

    if(m_CurrentTalent->m_nCurrentTalentLevel<=0)
    {
        m_OverlayNextPreview->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        m_OverlayNextPreview->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }

    m_TextPreviewTalentEffect->SetText(m_CurrentTalent->GetLevelupPreviewFormatText());
    
    m_BtnLearn->SetVisibility(ESlateVisibility::Visible);
    
    m_TextPreviewTalentEffect->ForceLayoutPrepass();

    m_OverlayNextPreview->ForceLayoutPrepass();
    //
}

void UDiaTalentPopup::SetTalentSpecData(UDiaTechnologyAsset* selectedTalentData)
{
    m_CurrentTalent = selectedTalentData;

    check(m_CurrentTalent);

    if (m_PlayerDiaComp->GetTalentPoint() <= 0) //cant learn
    {
        m_BtnLearn->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        m_BtnLearn->SetVisibility(ESlateVisibility::Visible);
    }

    UnbindAllFromAnimationFinished(m_FadeAnimation);

    m_BGForTouch->SetVisibility(ESlateVisibility::Visible);

    SetVisibility(ESlateVisibility::SelfHitTestInvisible);

    PlayAnimation(m_FadeAnimation);

    SetTalentIcon();
    SetTalentName();
    SetTalentRank();
    SetTalentDesc();
    SetRequirePointAlertShow();
    //있을때
    SetAdditionalInfoShow();
    //
    SetTalentLevelupPreview();

    ForceLayoutPrepass();
}

void UDiaTalentPopup::SetTalentPopupWidget(UDiaTechnologyAsset* selectedSkillData, const FGeometry& geo)
{
    if (m_CurrentTalent)
    {
        PlayHideInfoAnim();
        
        return;
    }

    SetTalentSpecData(selectedSkillData);

    SetPanelPosition(geo);
}

void UDiaTalentPopup::SetPanelPosition(const FGeometry& instigatorGeo)
{
    FGeometry&& Geo = UWidgetLayoutLibrary::GetPlayerScreenWidgetGeometry(GetOwningPlayer());

    UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);

    UCanvasPanel* CanvasPanelParent = Cast<UCanvasPanel>(PanelSlot->Parent);

    FVector2D ClickedItemSlot = CanvasPanelParent->GetCachedGeometry().AbsoluteToLocal(
        instigatorGeo.GetAbsolutePosition()) + instigatorGeo.GetLocalSize() / 2.0f;

    ClickedItemSlot.X += (GetDesiredSize().X / 2.0f) + (instigatorGeo.GetLocalSize().X / 2.0f);

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
    }

    PanelSlot->SetPosition(ClickedItemSlot);
}

FReply UDiaTalentPopup::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FReply Rel = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    PlayHideInfoAnim();
    return Rel;
}

void UDiaTalentPopup::LearnTalent()
{
    if (m_CurrentTalent)
        m_PlayerDiaComp->LevelupTalent(m_CurrentTalent);
}

void UDiaTalentPopup::PlayHideInfoAnim(float delay)
{
    m_CurrentTalent = nullptr; //prevent double touch

    BindToAnimationFinished(m_FadeAnimation, m_AnimEndDele);

    m_BGForTouch->SetVisibility(ESlateVisibility::HitTestInvisible);

  PlayAnimationReverse(m_FadeAnimation);
}

void UDiaTalentPopup::HidePopup()
{
    PRINTF("HideInfoPanel");

    m_CurrentTalent = nullptr;

    SetVisibility(ESlateVisibility::Collapsed);

    UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);

    PanelSlot->SetPosition(m_InitPos);
}
#undef LOCTEXT_NAMESPACE
