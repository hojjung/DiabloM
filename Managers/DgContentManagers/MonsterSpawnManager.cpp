#include "MonsterSpawnManager.h"
#include "DungeonManager.h"
#include "Managers/DiabloGameInstance.h"

void UMonsterSpawnManager::MoveToNormalDungeon()
{
	UDiabloGameInstance::Get->m_DungeonManager->OpenLevel(UDiabloGameInstance::Get->m_NormalDgManager);
}
