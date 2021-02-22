#include "DungeonManager.h"
#include "GameplayTagContainer.h"
#include "Engine/LevelStreaming.h"
#include "UObject/UObjectGlobals.h"


UDungeonManager::UDungeonManager(const FObjectInitializer& objInit):Super(objInit)
{
	m_CurrentDg = nullptr;
	m_MonsterManager = nullptr;

}

void UDungeonManager::Init(UMonsterSpawnManager*  mMang)
{
	//FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UMyGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UDungeonManager::LoadLevelComplete);
	m_MonsterManager = mMang; 
}

void UDungeonManager::LoadCurrentDungeonLevel(FName levelIDName,UObject* wrldctxt)
{
	m_CurrentDg = UDungeonDataTable::GetDungeonDataPtr(levelIDName);

	if(!m_CurrentDg)
	{
		PRINTF("DgManager-NoDgData");
		return;
	}
	
	UGameplayStatics::OpenLevel(wrldctxt,m_CurrentDg->m_DgId,true,"Listen");
}

void UDungeonManager::LoadLevelComplete(UWorld* world)
{
	m_MonsterManager->StartSpawn(world,m_CurrentDg);
}