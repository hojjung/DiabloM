#include "InitLevelHUD.h"

#include "ConstructorHelpers.h"
#include "GameStartCanvas.h"

AInitLevelHUD::AInitLevelHUD()
{
	ConstructorHelpers::FClassFinder<UGameStartCanvas> FoundW(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/StartMenu/WB_GameStartCanvas.WB_GameStartCanvas_C'"));

	m_ClassWidget = FoundW.Class;
}

void AInitLevelHUD::BeginPlay()
{
	Super::BeginPlay();
	
	m_Canvas = CreateWidget<UGameStartCanvas>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}
}

void AInitLevelHUD::ReqeustPopupText(FText str)
{
	m_Canvas->ReqeustPopupText(str);
}
