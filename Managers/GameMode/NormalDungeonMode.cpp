#include "NormalDungeonMode.h"
#include "Managers/DiabloGameInstance.h"
#include "Widgets/HUD/GameLevelHUD.h"


ANormalDungeonMode::ANormalDungeonMode()
{
	HUDClass = AGameLevelHUD::StaticClass();
}

void ANormalDungeonMode::StartPlay()
{
	Super::StartPlay();

	m_PlManager = UDiabloGameInstance::Get->m_PlayfabManager;
	
	m_ChatManager = UDiabloGameInstance::Get->m_ChatManager;
}

void ANormalDungeonMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	m_PlManager->TickTryUpdateUserData(DeltaSeconds);
	m_ChatManager->Tick(DeltaSeconds);
}
