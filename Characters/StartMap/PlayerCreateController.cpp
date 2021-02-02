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
    APlayerController::SetVirtualJoystickVisibility(false);
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

void APlayerCreateController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("Exit", EInputEvent::IE_Pressed, this, &APlayerCreateController::ExitGame);
    InputComponent->BindAction("AndroidBack", EInputEvent::IE_Pressed, this, &APlayerCreateController::OnDeviceBackKey);
}

void APlayerCreateController::OnDeviceBackKey()
{
    if(m_StartCanvas->m_bIsCreationOpened)
    {
        m_StartCanvas->m_CharCreate->BackCancel();
    }
    else if(m_StartCanvas->m_bIsSelectionOpened)
    {
        m_StartCanvas->ShowMainMenu();
    }
    else
    {
        ExitGame();
    }
    //ExitGame();
}
void APlayerCreateController::ExitGame()
{
    PRINTF("Exit");
    UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, true);
}
