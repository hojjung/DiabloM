// My First Hack n Slash


#include "InitLevelHUD.h"

#include "ConstructorHelpers.h"

AInitLevelHUD::AInitLevelHUD()
{
	//WidgetBlueprint'/Game/Blueprints/NewWidget/WB_NicknameSet.WB_NicknameSet'
	ConstructorHelpers::FClassFinder<USetNickname> FoundW(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/StartMenu/WB_NicknameSet.WB_NicknameSet_C'"));

	m_ClassWidget = FoundW.Class;
}

void AInitLevelHUD::BeginPlay()
{
	Super::BeginPlay();
	//SetInputMode(FInputModeUIOnly());
	m_Canvas = CreateWidget<USetNickname>(GetWorld(), m_ClassWidget);

	if(m_Canvas)
	{
		m_Canvas->AddToViewport();
	}

	m_Canvas->HideNicknameSet();
}

void AInitLevelHUD::ReqeustPopupText(FText str)
{
	m_Canvas->ReqeustPopupText(str);
}
