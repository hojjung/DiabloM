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

void UDungeonManager::SetDungeonLevel(const FString& currentDG)//need split
{
	DungeonDataTable->GetAllRows("",m_AryDgDataTable);
	
	TArray<FString> AryDg;
	
	currentDG.ParseIntoArray(AryDg,TEXT(":"));

	int StageCurrentLevel = FCString::Atoi(*AryDg[0]);

	m_nMyMaxStageLevel = FCString::Atoi(*AryDg[1]);

	if(m_AryDgDataTable.MyRangeCheck(StageCurrentLevel))
	{
		m_CurrentDg = m_AryDgDataTable[StageCurrentLevel];
		m_nCurrentStageLevel=StageCurrentLevel;
	}
	else
	{
		m_CurrentDg = m_AryDgDataTable[0];
		m_nCurrentStageLevel=0;
	}
}

void UDungeonManager::SelectDungeon(int index)
{
	m_CurrentDg = m_AryDgDataTable[index];
}

void UDungeonManager::LevelUpDungeon()
{
	int NextLevel = m_nCurrentStageLevel+1;
	
	if(NextLevel>=m_AryDgDataTable.Num())
	{
		return;//MAXStage
	}

	PRINTF("DGM_NotCancel?");
	
	m_nCurrentStageLevel++;
	
	SelectDungeon(m_nCurrentStageLevel);

	if(m_nCurrentStageLevel>m_nMyMaxStageLevel)
	{
		//new record
		m_nMyMaxStageLevel=m_nCurrentStageLevel;
		UDiabloGameInstance::Get->m_PlayfabManager->OnStageComplete();
	}

	m_OnDgOpen.Broadcast(m_nCurrentStageLevel);
	
	OpenLevel();
}

void UDungeonManager::LoadLevelComplete(UWorld* world)
{
	m_MonsterManager->StartSpawn(world,m_CurrentDg);
}
