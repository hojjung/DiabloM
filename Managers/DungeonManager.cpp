#include "DungeonManager.h"

#include "DiabloGameInstance.h"
#include "GameplayTagContainer.h"
#include "Engine/LevelStreaming.h"
#include "UObject/UObjectGlobals.h"


UDataTable* UDungeonManager::DungeonDataTable = nullptr;
UDataTable* UDungeonManager::DropDataTable = nullptr;
UDataTable* UDungeonManager::MonsterEntityTable = nullptr;

UDungeonManager::UDungeonManager(const FObjectInitializer& objInit):Super(objInit)
{
	m_CurrentDg = nullptr;
	m_MonsterManager = nullptr;
	
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundDungeon(TEXT("DataTable'/Game/DataTables/Dungeon/DungeonData.DungeonData'"));
	
	DungeonDataTable = FoundDungeon.Object;
}

void UDungeonManager::Init(UMonsterSpawnManager*  mMang)
{
	m_MonsterManager = mMang; 
}

void UDungeonManager::OpenLevel()
{
	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(),m_CurrentDg->m_DgId,true);
}

void UDungeonManager::SetDungeonLevel(const FString& dgUnlockAry)//need split
{
	DungeonDataTable->GetAllRows("",m_AryDgDataTable);
	
	TArray<FString> AryDg;
	
	int Len = dgUnlockAry.ParseIntoArray(AryDg,TEXT(":"));
	
	m_AryDgUnlocked.Reserve(Len);

	for(int i=0; i< Len; i++)
	{
		int IsUnlocked = FCString::Atoi(*AryDg[i]);

		m_AryDgUnlocked.Add(1);//0319

		if(IsUnlocked>1)//selected
		{
			m_CurrentDg = m_AryDgDataTable[i]; 
		}
	}

	if(!m_CurrentDg)
	{
		m_CurrentDg = m_AryDgDataTable[0];
		PRINTF("DgManager-NoDgData");
		return;
	}
}

void UDungeonManager::SelectDungeon(int index)
{
	m_CurrentDg = m_AryDgDataTable[index];
}

void UDungeonManager::LoadLevelComplete(UWorld* world)
{
	m_MonsterManager->StartSpawn(world,m_CurrentDg);
}
