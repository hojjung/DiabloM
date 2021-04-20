#include "DungeonManager.h"

#include "DiabloGameInstance.h"
#include "GameplayTagContainer.h"
#include "JsonSerializer.h"
#include "UObject/UObjectGlobals.h"


UDataTable* UDungeonManager::DungeonDataTable = nullptr;
UDataTable* UDungeonManager::DropDataTable = nullptr;
UDataTable* UDungeonManager::MonsterEntityTable = nullptr;
UDataTable* UDungeonManager::GoldDungeonDataTable = nullptr;

UDungeonManager::UDungeonManager(const FObjectInitializer& objInit): Super(objInit)
{
	m_CurrentDg = nullptr;
	m_MonsterManager = nullptr;

	static ConstructorHelpers::FObjectFinder<UDataTable> FoundDungeon(
		TEXT("DataTable'/Game/DataTables/Dungeon/DungeonData.DungeonData'"));

	DungeonDataTable = FoundDungeon.Object;

	//DataTable'/Game/DataTables/Dungeon/GoldDungeonData.GoldDungeonData'

	static ConstructorHelpers::FObjectFinder<UDataTable> FoundGoldDungeon(
        TEXT("DataTable'/Game/DataTables/Dungeon/GoldDungeonData.GoldDungeonData'"));

	GoldDungeonDataTable = FoundGoldDungeon.Object;
}

void UDungeonManager::Init(UMonsterSpawnManager* mMang)
{
	m_MonsterManager = mMang;
	
}

void UDungeonManager::SetDungeonData(const FString& dgJsonStr)
{
	DungeonDataTable->GetAllRows("", m_AryDgDataTable);

	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(dgJsonStr);
	
	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;	
	}
	
	int StageCurrentLevel = JsonObject->GetIntegerField(TEXT("CurrentStageLevel"));

	SetMaxStageLevel(JsonObject->GetIntegerField(TEXT("MaxStageLevel")));

	if (StageCurrentLevel < 0 || StageCurrentLevel >= m_AryDgDataTable.Num())
	{
		StageCurrentLevel = 0;
	}

	SelectNormalDungeon(StageCurrentLevel);
	//
	GoldDungeonDataTable->GetAllRows("", m_AryGoldDgDataTable);
}

void UDungeonManager::OpenLevel()
{
	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(), m_CurrentDg->m_DgId, true);
}

void UDungeonManager::SelectNormalDungeon(int index)
{
	SetCurrentStageLevel(index);
	m_CurrentDg = m_AryDgDataTable[GetCurrentStage()];
}

void UDungeonManager::SelectGoldDungeon(int index)
{
	m_CurrentGoldDg =  m_AryGoldDgDataTable[index];
	//GoldDg Ticket Count
	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(), m_CurrentGoldDg->m_DgId, true);
}

void UDungeonManager::LevelUpDungeon()
{
	m_nCurrentStageLevel = GetCurrentStage();

	int NextLevel = m_nCurrentStageLevel + 1;

	if (NextLevel >= m_AryDgDataTable.Num())
	{
		return; //MAXStage
	}

	m_nCurrentStageLevel++;

	SetCurrentStageLevel(m_nCurrentStageLevel);

	SelectNormalDungeon(m_nCurrentStageLevel);

	if (m_nCurrentStageLevel > GetMaxStage())
	{
		PRINTF("DgManager-LevelUpDungeon HighScore");

		SetMaxStageLevel(m_nCurrentStageLevel);
		UDiabloGameInstance::Get->m_QuestManager->AddQuestCount(EQuestType::StageLv);
		UDiabloGameInstance::Get->m_PlayfabManager->OnStageComplete();
		m_OnDungeonMaxUpdate.Broadcast();
	}

	m_OnDgOpen.Broadcast(m_nCurrentStageLevel);
	
	UDiabloGameInstance::Get->m_PlayfabManager->UploadUserTitleData01();
	
	OpenLevel();
}

BigInt UDungeonManager::GetCurrentDungeonBounty()
{
	return m_CurrentDg->GetMobGold();
}

BigInt UDungeonManager::GetMaxDungeonBounty()
{
	return m_AryDgDataTable[GetMaxStage()]->GetMobGold();
}

int UDungeonManager::GetMaxStage() const
{
	int CachedStage = m_nSafeMaxStageLevel ^ 1423;

	if (m_nMyMaxStageLevel != CachedStage)
	{
		PRINTF("1Cheated!!!!!");
		UDiabloGameInstance::Get->m_PlayfabManager->RequestCheatAlert();
		return -1;
	}

	return CachedStage;
}

int UDungeonManager::GetCurrentStage() const
{
	int CachedStage = m_nSafeCurrentStageLevel ^ 666;

	if (m_nCurrentStageLevel != CachedStage)
	{
		PRINTF("2Cheated!!!!!");
		UDiabloGameInstance::Get->m_PlayfabManager->RequestCheatAlert();
		return -1;
	}

	return CachedStage;
}

void UDungeonManager::SetMaxStageLevel(int stageLv)
{
	m_nMyMaxStageLevel = stageLv;
	m_nSafeMaxStageLevel = m_nMyMaxStageLevel ^ 1423;
}

void UDungeonManager::SetCurrentStageLevel(int stageLv)
{
	m_nCurrentStageLevel = stageLv;
	m_nSafeCurrentStageLevel = m_nCurrentStageLevel ^ 666;
}

int UDungeonManager::GetLevelBonus()
{
	if (m_nCurrentStageLevel < 8)
	{
		return 1;
	}
	else if (m_nCurrentStageLevel < 16)
	{
		return 2;
	}
	else if (m_nCurrentStageLevel < 24)
	{
		return 4;
	}
	else if (m_nCurrentStageLevel < 32)
	{
		return 8;
	}
	else if (m_nCurrentStageLevel < 40)
	{
		return 16;
	}
	else if (m_nCurrentStageLevel < 48)
	{
		return 32;
	}
	else if (m_nCurrentStageLevel < 56)
	{
		return 64;
	}
	else if (m_nCurrentStageLevel < 64)
	{
		return 128;
	}
	else if (m_nCurrentStageLevel < 72)
	{
		return 256;
	}
	else if (m_nCurrentStageLevel < 80)
	{
		return 512;
	}
	else
	{
		return 1024;
	}
}

FString UDungeonManager::GetDgDataStr()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("CurrentStageLevel"), GetCurrentStage());

	JsonObject->SetNumberField(TEXT("MaxStageLevel"), GetMaxStage());

	return PlayFab::FJsonKeeper(JsonObject).toJSONString();
}

void UDungeonManager::LoadLevelComplete(UWorld* world)
{
	m_MonsterManager->StartSpawn(world, m_CurrentDg);
}
