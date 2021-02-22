#include "MyHUD.h"
#include "ConstructorHelpers.h"
#include "MainCanvas.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FClassFinder<UMainCanvas> FoundWW(
        TEXT("WidgetBlueprint'/Game/Blueprints/NewWidget/WB_MainCanvas.WB_MainCanvas_C'"));
	m_ClassWidget = FoundWW.Class;
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	
	m_Canvas = CreateWidget<UMainCanvas>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}
}

