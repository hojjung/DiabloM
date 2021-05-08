#include "DungeonManager.h"
#include "GameplayTagContainer.h"
#include "JsonSerializer.h"
#include "Managers/DiabloGameInstance.h"
#include "UObject/UObjectGlobals.h"


void UDungeonManager::Init()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UDungeonManager::OnLevelLoadComplete);

	m_PlayerUpgradeManager = UDiabloGameInstance::Get->m_PlayerUpgradeManager;
}

void UDungeonManager::OpenLevel(UMonsterSpawnManager* currentDgManager)
{
	m_CurrentSpawnManager = currentDgManager;
	
	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(),*m_CurrentSpawnManager->GetOpenLevelAssetName(), true);

	m_OnLevelLoadComplete.BindUObject(m_CurrentSpawnManager,&UMonsterSpawnManager::OnLevelLoadComplete);
}

AUnitPawn* UDungeonManager::GetNearEnemy(const FVector& loc)
{
	if(!m_CurrentSpawnManager)
	{
		return nullptr;
	}
	
	return m_CurrentSpawnManager->GetNearestEnemy(loc);
}

void UDungeonManager::Tick(float deltaTime)
{
	m_PlayerUpgradeManager->Tick(deltaTime);
	
	if(!m_CurrentSpawnManager)
	{
		return;
	}
	
	m_CurrentSpawnManager->Tick(deltaTime);
}

void UDungeonManager::OnLevelLoadComplete(UWorld* world)
{
	m_OnLevelLoadComplete.ExecuteIfBound(world);
	m_OnLevelLoadComplete.Unbind();

}
