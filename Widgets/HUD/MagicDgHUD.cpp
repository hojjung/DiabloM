#include "MagicDgHUD.h"

#include "MagicDgCanvas.h"


AMagicDgHUD::AMagicDgHUD()
{
	ConstructorHelpers::FClassFinder<UMagicDgCanvas> FoundW(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/MagicDgLevel/WB_MagicDgCanvas.WB_MagicDgCanvas_C'"));

	m_ClassWidget = FoundW.Class;
}

void AMagicDgHUD::BeginPlay()
{
	Super::BeginPlay();
	
	m_Canvas = CreateWidget<UMagicDgCanvas>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}	
}

void AMagicDgHUD::ReqeustPopupText(FText str)
{
	//m_Canvas->ReqeustPopupText(str);
}
