#include "PlayerCreateController.h"
#include "Managers/DiabloCheatManager.h"
APlayerCreateController::APlayerCreateController()
{
    APlayerController::SetVirtualJoystickVisibility(false);
    
    bAutoManageActiveCameraTarget=false;

    bShowMouseCursor=true;
    
    m_ClassStartMenu=UStartMenuCanvas::StaticClass();
    
    CheatClass = UDiabloCheatManager::StaticClass();

    APlayerController::SetVirtualJoystickVisibility(false);
}

void APlayerCreateController::BeginPlay()
{
    Super::BeginPlay();
    
    InitWidget();
}

void APlayerCreateController::InitWidget()
{
    m_StartCanvas = CreateWidget<UStartMenuCanvas>(this, m_ClassStartMenu, "StartMenu00");
    m_StartCanvas->AddToViewport();
    m_StartCanvas->InitStartMenu();

}

APlayerVisual* APlayerCreateController::GetPlayerVisual()
{
    return GetPawn<APlayerVisual>();
}
