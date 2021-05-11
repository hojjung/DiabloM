
#include "PetDgHUD.h"

APetDgHUD::APetDgHUD()
{
	ConstructorHelpers::FClassFinder<UPetDgCanvas> FoundW(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/PetDgLevel/WB_PetDgCanvas.WB_PetDgCanvas_C'"));

	m_ClassWidget = FoundW.Class;
}
//
void APetDgHUD::BeginPlay()
{
	Super::BeginPlay();

	m_Canvas = CreateWidget<UPetDgCanvas>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}
}

void APetDgHUD::ReqeustPopupText(FText str)
{
	
}
