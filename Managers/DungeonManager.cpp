#include "DungeonManager.h"

#include "DiabloGameInstance.h"
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
	m_MonsterManager = mMang; 
}

void UDungeonManager::SetDungeonLevel(const FString& levelIDName)//need split
{
	m_CurrentDg = UDungeonDataTable::GetDungeonDataPtr(levelIDName);

	if(!m_CurrentDg)
	{
		PRINTF("DgManager-NoDgData");
		return;
	}
	
	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(),m_CurrentDg->m_DgId,true,"Listen");
}

void UDungeonManager::LoadLevelComplete(UWorld* world)
{
	m_MonsterManager->StartSpawn(world,m_CurrentDg);
}