#include "MinimapWidget.h"

#include "GridFlowMiniMap.h"
#include "Characters/DiabloPlayerController.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Managers/DiabloGameMode.h"

void UMinimapWidget::SetMaxMinCornerMap()
{
    m_PlayerScreenPos=nullptr;
    
    UCanvasPanelSlot* OverlayCanvas = Cast<UCanvasPanelSlot>(m_OverlayMinimapWrap->Slot);
    FVector2D OverlaySize = OverlayCanvas->GetSize();
    
    float X = (OverlaySize.X * m_MinimapCornerSmall->RenderTransform.Scale.X)/4.f;
    
    float Y = (OverlaySize.Y * m_MinimapCornerSmall->RenderTransform.Scale.Y)/4.f;
    
    m_fMaxX=X;
    m_fMinX=m_fMaxX*-1.f;
    m_fMaxY=Y;
    m_fMinY=m_fMaxY*-1.f;

    m_PlayerScreenPos = &UGridFlowMiniMap::Get->m_PlayerScreenPos;
}

void UMinimapWidget::SetMinimapMat(UMaterialInterface* material_interface)
{
    m_MatMinimapInst=material_interface;

    SetMaxMinCornerMap();
    
}

void UMinimapWidget::UpdateMinimapWidget()
{
    if(!m_MatMinimapInst)
    {
        return;
    }
    
    FSlateBrush Brush = UWidgetBlueprintLibrary::MakeBrushFromMaterial(m_MatMinimapInst, 512, 512);

    m_MinimapCenterFull->SetBrush(Brush);
    
    m_MinimapCornerSmall->SetBrush(Brush);
}

void UMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateMinimapWidget();

    if(m_PlayerScreenPos)
    {
        UCanvasPanelSlot* OverlayCanvas = Cast<UCanvasPanelSlot>(m_OverlayMinimapWrap->Slot);
        FVector2D OverlaySize = OverlayCanvas->GetSize();

        FVector2D ScreenMax1Pos=(*m_PlayerScreenPos);
        ScreenMax1Pos*=2.f;
        ScreenMax1Pos.X-=1;
        ScreenMax1Pos.Y-=1;
        
        float WidthX = OverlaySize.X;
        float WidthY = OverlaySize.Y;

        FVector2D Pos = FVector2D(ScreenMax1Pos.X*WidthX,ScreenMax1Pos.Y*WidthY);
        Pos*=-1.f;
        m_MinimapCornerSmall->SetRenderTranslation(Pos*2);//this value come from editor scale,it is 4,because upper 2 multiple already did
    }
}
