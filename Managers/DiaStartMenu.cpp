// My First Hack n Slash


#include "DiaStartMenu.h"

#include "DiabloGameInstance.h"
#include "PlayfabManager.h"
#include "GameFramework/PlayerState.h"

void ADiaStartMenu::StartPlay()
{
	Super::StartPlay();
	
	PRINTF("Player ID:%d", UGameplayStatics::GetPlayerController(this,0)->PlayerState->GetUniqueID());

	UDiabloGameInstance::Get->GetPlayfabManager()->Init();
}
