#include "StartMapGameMode.h"
#include "PlayFabServerDataModels.h"
#include "Characters/InitPlayerController.h"
#include "Core/PlayFabClientAPI.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"
#include "Widgets/InitLevelHUD.h"

AStartMapGameMode::AStartMapGameMode()
{
	PlayerControllerClass = AInitPlayerController::StaticClass();
	//DefaultPawnClass = APlayerDiabloCharacter::StaticClass();
	//GameStateClass = ADiaGameState::StaticClass();
	//PlayerStateClass = ADiaPlayerState::StaticClass();
	HUDClass = AInitLevelHUD::StaticClass();

}

void AStartMapGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	if(!UDiabloGameInstance::Get->m_PlayfabManager->m_bIsNicknameSet)
	{
		return;	
	}
	//m_bIsNicknameSet

	if(!UDiabloGameInstance::Get->m_PlayfabManager->m_bIsLoginCompleted)
	{
		return;	
	}
	
	if(!m_bLock)
	{
		UDiabloGameInstance::Get->m_DungeonManager->OpenLevel();

		m_bLock=true;
	}
}
