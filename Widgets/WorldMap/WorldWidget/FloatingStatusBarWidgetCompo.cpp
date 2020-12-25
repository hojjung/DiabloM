#include "FloatingStatusBarWidgetCompo.h"
#include "Widgets/WorldMap/DefaultMenu/MaterialProgressBar.h"

void UFloatingStatusBarWidgetCompo::BeginPlay()
{
	Super::BeginPlay();

}

void UFloatingStatusBarWidgetCompo::SetHealthPercentage(float HealthPercentage)
{
	Cast<UMaterialProgressBar>( GetUserWidgetObject())->SetProgressValue(HealthPercentage);
}//Hide show할때마다 다시 위젯 만드는ㄷㅅ
