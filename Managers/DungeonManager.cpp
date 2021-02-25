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

void UDungeonManager::OpenLevel()
{
	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(),m_CurrentDg->m_DgId,true,"Listen");
}

void UDungeonManager::SetDungeonLevel(const FString& dgUnlockAry)//need split
{
	UDungeonDataTable::GetDungeonTable->GetAllRows("",UDungeonDataTable::AryDgData);
	
	TArray<FString> AryDg;
	
	int Len = dgUnlockAry.ParseIntoArray(AryDg,TEXT(":"));
	
	m_AryDgUnlocked.Reserve(Len);

	for(int i=0; i< Len; i++)
	{
		int IsUnlocked = FCString::Atoi(*AryDg[i]);

		m_AryDgUnlocked.Add(IsUnlocked);

		if(IsUnlocked>1)//selected
		{
			m_CurrentDg = UDungeonDataTable::AryDgData[i]; 
		}
	}

	if(!m_CurrentDg)
	{
		m_CurrentDg = UDungeonDataTable::AryDgData[0];
		PRINTF("DgManager-NoDgData");
		return;
	}
	
	
}

void UDungeonManager::LoadLevelComplete(UWorld* world)
{
	m_MonsterManager->StartSpawn(world,m_CurrentDg);
}