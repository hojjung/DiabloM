#include "MagicStoneDgManager.h"
#include "Kismet/GameplayStatics.h"
#include "Datas/DungeonDataTable.h"
#include "Managers/DiabloGameInstance.h"


UMagicStoneDgManager::UMagicStoneDgManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundDgTable(
		TEXT("DataTable'/Game/DataTables/Dungeon/MagicDragonDgTable.MagicDragonDgTable'"));

	m_MagicDgTable = FoundDgTable.Object;
}

void UMagicStoneDgManager::Init()
{
	m_MagicDgTable->GetAllRows(TEXT("MagicDgManager-No Table Data"), m_DgDataRow);
}

void UMagicStoneDgManager::OnLevelLoadComplete(UWorld* world)
{
	PRINTF("MagicStoneManager! World:%s", *world->GetMapName());

	FMonsterEntity* DragonEntity = m_CurrentDgData->m_Monsters[0].GetRow<FMonsterEntity>("");
	//
	FVector Loc = FVector(290, 290, 102);

	FRotator Rot = FRotator(0, 130, 0);
	//
	FActorSpawnParameters Param;

	Param.bNoFail = true;

	m_SpawnedMagicDragon = world->SpawnActor<AMonsterPawn>(AMonsterPawn::StaticClass(), Loc, Rot, Param);

	m_SpawnedMagicDragon->DataInject(DragonEntity, m_CurrentDgData->GetMobHp(), m_CurrentDgData->GetMobGold(),
									EMonsterType::MagicDragon, 1, 1, 1);

	m_SpawnedMagicDragon->m_OnDeathAnimAfter.AddUObject(this,&UMagicStoneDgManager::EndMagicDgSuccess);

	m_nSuccessBounty = m_CurrentDgData->GetRandomPrize();

	m_nFailBounty  = m_CurrentDgData->m_nPrizeMagicStoneMin / 4;

	m_OnDragonSpawned.ExecuteIfBound(m_SpawnedMagicDragon);
	
	StartDungeon();
}

void UMagicStoneDgManager::StartDungeon()
{
	Super::StartDungeon();

	APlayerDiabloCharacter* PlChar = UDiabloGameInstance::Get->GetPlChar();

	PlChar->FocusTarget(m_SpawnedMagicDragon);

	m_fTimer = 0.f;
	
	m_bIsMatchStarted = true;
}

void UMagicStoneDgManager::EndDungeon(bool b)
{
	Super::EndDungeon(b);

	UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this,&UMagicStoneDgManager::MoveToNormalDungeon, 2.2f,false);	
}

FString UMagicStoneDgManager::GetOpenLevelAssetName()
{
	return m_CurrentDgData->m_DgId.ToString();
}

void UMagicStoneDgManager::RequestMoveMagicStoneDg(int dgLevel)
{
	m_CurrentDgData = m_DgDataRow[dgLevel];
}


void UMagicStoneDgManager::Tick(float delta_seconds)
{
	//서버에서 받아온거
	if (!m_bIsMatchStarted)
	{
		return;
	}

	Super::Tick(delta_seconds);

	m_fTimer += delta_seconds;

	float TimeRemain = MAGICDGTIME - m_fTimer;

	if (m_fTimer > MAGICDGTIME)
	{
		EndMagicDgFail();
	}
}

bool UMagicStoneDgManager::IsBattleStarted()
{
	return m_bIsMatchStarted;
}


void UMagicStoneDgManager::EndMagicDgSuccess(AMonsterPawn* dragonDead)
{
	PRINTF("사냥성공");
	
	AddMagicStones(m_nSuccessBounty);
	
	m_bIsMatchStarted = false;

	EndDungeon(true);
}

void UMagicStoneDgManager::EndMagicDgFail()
{
	PRINTF("사냥실패");

	AddMagicStones(m_nFailBounty);

	m_bIsMatchStarted = false;
	
	EndDungeon(false);
}
void UMagicStoneDgManager::AddMagicStones(int magicStones)
{
	m_nCurrentMagicStone+=magicStones;
}


int UMagicStoneDgManager::GetResultBounty()
{
	return m_nSuccessBounty;
}

int UMagicStoneDgManager::GetFailBounty()
{
	return m_nFailBounty;
}

