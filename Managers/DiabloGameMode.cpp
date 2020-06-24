#include "DiabloGameMode.h"
#include "Characters/PlayerDiabloCharacter.h"
#include "Characters/DiabloPlayerController.h"

ADiabloGameMode::ADiabloGameMode()
{
	PlayerControllerClass = ADiabloPlayerController::StaticClass();
	DefaultPawnClass = APlayerDiabloCharacter::StaticClass();
}