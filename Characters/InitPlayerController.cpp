#include "InitPlayerController.h"

#include "ConstructorHelpers.h"

AInitPlayerController::AInitPlayerController()
{
	//WidgetBlueprint'/Game/Blueprints/NewWidget/WB_NicknameSet.WB_NicknameSet'
	ConstructorHelpers::FClassFinder<USetNickname> FoundW(TEXT("WidgetBlueprint'/Game/Blueprints/NewWidget/WB_NicknameSet.WB_NicknameSet_C'"));

	m_ClassWidget = FoundW.Class;
}

void AInitPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	m_Canvas = CreateWidget<USetNickname>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}
}
