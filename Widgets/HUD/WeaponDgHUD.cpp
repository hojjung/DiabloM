// My First Hack n Slash


#include "WeaponDgHUD.h"

#include "ConstructorHelpers.h"

AWeaponDgHUD::AWeaponDgHUD()
{
	ConstructorHelpers::FClassFinder<UWeaponDgCanvas> FoundW(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WeaponDgLevel/WB_WeaponDgCanvas.WB_WeaponDgCanvas_C'"));

	m_ClassWidget = FoundW.Class;
}

void AWeaponDgHUD::BeginPlay()
{
	Super::BeginPlay();
	
	m_Canvas = CreateWidget<UWeaponDgCanvas>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}	
}

void AWeaponDgHUD::ReqeustPopupText(FText str)
{
}
