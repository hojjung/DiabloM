#include "StartMapGameMode.h"


#include "PlayFabServerDataModels.h"
#include "Core/PlayFabClientAPI.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/DiabloGameInstance.h"
#include "Managers/DungeonManager.h"

void AStartMapGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!UDiabloGameInstance::Get->m_PlayfabManager->m_bLoginProcessEnd)
	{
		return;	
	}

	FName currentPlayerClass = UDiabloGameInstance::Get->m_PlayfabManager->GetLoadedPlayerClassID();
	UDiabloGameInstance::Get->m_PlayerClassManager->LoadPlayerClass(currentPlayerClass);
	//
	FName currentDungeonID = UDiabloGameInstance::Get->m_PlayfabManager->GetLoadedDungeonID();
	UDiabloGameInstance::Get->m_DungeonManager->LoadCurrentDungeonLevel(currentDungeonID,this);
}
