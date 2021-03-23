#include "GameLevelHUD.h"
#include "ConstructorHelpers.h"
#include "MainCanvas.h"

AGameLevelHUD::AGameLevelHUD()
{
	static ConstructorHelpers::FClassFinder<UMainCanvas> FoundWW(
    TEXT("WidgetBlueprint'/Game/Blueprints/Widget/WB_MainCanvas.WB_MainCanvas_C'"));
	m_ClassWidget = FoundWW.Class;
}

void AGameLevelHUD::BeginPlay()
{
	Super::BeginPlay();
	
	m_Canvas = CreateWidget<UMainCanvas>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}
}

void AGameLevelHUD::ReqeustPopupText(FText str)
{
	m_Canvas->RequestText(str);
}

void AGameLevelHUD::OnPressBackkey()
{
	m_Canvas->OnPressBackkey();
}
