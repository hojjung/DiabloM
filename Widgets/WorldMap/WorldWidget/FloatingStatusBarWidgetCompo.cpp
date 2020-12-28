#include "FloatingStatusBarWidgetCompo.h"
#include "Widgets/WorldMap/DefaultMenu/MaterialProgressBar.h"

void UFloatingStatusBarWidgetCompo::BeginPlay()
{
	Super::BeginPlay();

}

void UFloatingStatusBarWidgetCompo::SetHealthPercentage(float HealthPercentage)
{
	UUserWidget* WidgetObj = GetUserWidgetObject();

	if(!WidgetObj)
	{
		return;
	}
	UMaterialProgressBar* WidgetProgressBar=Cast<UMaterialProgressBar>(WidgetObj);
	WidgetProgressBar->SetProgressValue(HealthPercentage);
}//Hide show할때마다 다시 위젯 만드는ㄷㅅ
