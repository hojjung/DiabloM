// My First Hack n Slash


#include "PVP_HUD.h"

#include "ConstructorHelpers.h"

APVP_HUD::APVP_HUD()
{
	ConstructorHelpers::FClassFinder<UPVPCanvas> FoundW(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/PVPLevel/WB_PVPCanvas.WB_PVPCanvas_C'"));

	m_ClassWidget = FoundW.Class;
}

void APVP_HUD::BeginPlay()
{
	Super::BeginPlay();
	
	m_Canvas = CreateWidget<UPVPCanvas>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}
}

void APVP_HUD::ReqeustPopupText(FText str)
{
	m_Canvas->ReqeustPopupText(str);
}
