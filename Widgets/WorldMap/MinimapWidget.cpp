// My First Hack n Slash


#include "MinimapWidget.h"


#include "GridFlowMiniMap.h"
#include "Managers/DiabloGameMode.h"

void UMinimapWidget::UpdateMinimap(UMaterialInterface* material_interface)
{
    m_MatMinimapInst=material_interface;

}

void UMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

   // ADiabloGameMode::Get->GetMinimapManager()->UpdateMaterial(m_MatMinimapInst);
}
