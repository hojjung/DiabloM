#include "MinimapWidget.h"
#include "GridFlowMiniMap.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DiabloGameMode.h"
#include "Managers/DungeonManager.h"

UMinimapWidget::UMinimapWidget(const FObjectInitializer& objInit):Super(objInit)
{
    m_MinimapWidgetSize = FVector2D(512.f,512.f);
    m_bIsUsingCornerMap=true;
    m_PlayerScreenPos=nullptr;
    static ConstructorHelpers::FObjectFinder<UTexture2D> FondTxt(TEXT("Texture2D'/Game/Sprite/UI/CSL_Character_DeleteBtn.CSL_Character_DeleteBtn'"));
    m_CancelMinimapTexture=FondTxt.Object;
    m_fPortalHoldTime=2.f;
    m_fPortalDelayTime=1.f;
}

void UMinimapWidget::Init()
{
    m_PlayerScreenPos=nullptr;
    m_MinimapCornerSmall->OnMouseButtonDownEvent.BindDynamic(this,&UMinimapWidget::OnCornerMapClicked);
    SetCornerScaleValue(4.f);
    SetOverlaySize();
    m_PortalBtn->OnPressed.AddDynamic(this,&UMinimapWidget::OnBtnPressHold);
    m_PortalBtn->OnClicked.AddDynamic(this,&UMinimapWidget::OnBtnReleaseHold);
    m_DgM=UDiabloGameInstance::Get->GetDungeonManager();
    check(m_DgM);
}

void UMinimapWidget::HideMinimap()
{
    SetFullMinimap();
    m_SelectedUpdateImage=nullptr;
    m_MinimapCenterFull->SetVisibility(ESlateVisibility::Hidden);
}

void UMinimapWidget::ShowMinimap()
{
    SetCornerMinimap();
}


void UMinimapWidget::SetCornerScaleValue(float scaleV)
{
    m_fCornerMapScale=scaleV;
    m_fCornerMapScaleRatio=m_fCornerMapScale/2.f;
}

void UMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateMinimapWidget();

    UpdateCornerMapTransform();
}

FEventReply UMinimapWidget::OnCornerMapClicked(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
    if(!m_DgM->IsDungeonOpened() || !m_DgM->IsPlayerInDg())
    {
        return FEventReply();
    }
    
    if(m_bIsUsingCornerMap)
    {
        SetFullMinimap();
    }
    else
    {
        SetCornerMinimap();
    }

    return FEventReply();
}



FVector2D UMinimapWidget::TransformScreenCoordToMyWidget(const FVector2D& sceenPos)
{
    FVector2D ScreenMaxPos=sceenPos;
    ScreenMaxPos*=2.f;
    ScreenMaxPos.X-=1;
    ScreenMaxPos.Y-=1;

    return ScreenMaxPos;
}


void UMinimapWidget::SetOverlaySize()
{
    UCanvasPanelSlot* OverlayCanvas = Cast<UCanvasPanelSlot>(m_OverlayMinimapWrap->Slot);
    
    m_OverlaySize = OverlayCanvas->GetSize();
}

void UMinimapWidget::SetMinimapMat(UMaterialInterface* material_interface)
{
    m_MatMinimapInst=material_interface;
    m_PlayerScreenPos = &UGridFlowMiniMap::Get->m_PlayerScreenPos;
    SetCornerMinimap();
}

void UMinimapWidget::HideCornerMinimap()
{
    m_MinimapCornerSmall->SetRenderScale(FVector2D(1.f,1.f));
    m_MinimapCornerSmall->SetRenderTranslation(FVector2D(0.f,0.f));
    m_MinimapCornerSmall->SetRenderTransformAngle(135.f);
    m_MinimapCornerSmall->SetBrushFromTexture(m_CancelMinimapTexture);
}

void UMinimapWidget::SetFullMinimap()
{
    m_bIsUsingCornerMap=false;
    m_SelectedUpdateImage = m_MinimapCenterFull;
    HideCornerMinimap();
    m_MinimapCenterFull->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UMinimapWidget::SetCornerMinimap()
{
    m_bIsUsingCornerMap=true;
    m_SelectedUpdateImage = m_MinimapCornerSmall;
    m_MinimapCornerSmall->SetRenderScale(FVector2D(m_fCornerMapScale,m_fCornerMapScale));
    m_MinimapCornerSmall->SetRenderTransformAngle(0.f);
    m_MinimapCenterFull->SetVisibility(ESlateVisibility::Hidden);
}

void UMinimapWidget::UpdateMinimapWidget()
{
    if(!m_MatMinimapInst)
    {
        return;
    }

    if(!m_SelectedUpdateImage)
    {
        return;
    }
    
    FSlateBrush Brush = UWidgetBlueprintLibrary::MakeBrushFromMaterial(m_MatMinimapInst, m_MinimapWidgetSize.X, m_MinimapWidgetSize.Y); // 이걸 키워보는건?

    m_SelectedUpdateImage->SetBrush(Brush);
}

void UMinimapWidget::UpdateCornerMapTransform()
{
    if(m_PlayerScreenPos&&m_bIsUsingCornerMap)
    {
        FVector2D ScreenPos=TransformScreenCoordToMyWidget(*m_PlayerScreenPos);
        
        FVector2D Pos = FVector2D(ScreenPos.X*m_OverlaySize.X,ScreenPos.Y*m_OverlaySize.Y);
        
        Pos*=-1.f;
        
        m_MinimapCornerSmall->SetRenderTranslation(Pos*m_fCornerMapScaleRatio);
    }
}

void UMinimapWidget::OnBtnPressHold()
{
    ADiabloPlayerController::Get->GetPlayerPawn()->UsePortal();
}

void UMinimapWidget::OnBtnReleaseHold()
{
   ADiabloPlayerController::Get->GetPlayerPawn()->CancelPortal();
}

