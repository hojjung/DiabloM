#include "MagicStoneDgManager.h"
#include "Kismet/GameplayStatics.h"
#include "Datas/DungeonDataTable.h"
#include "Managers/DiabloGameInstance.h"


UMagicStoneDgManager::UMagicStoneDgManager()
{
	m_nDgLevel = 0;
	static ConstructorHelpers::FClassFinder<UCameraShake> FoundCamshake(
		TEXT("Blueprint'/Game/Blueprints/BP_StompShake.BP_StompShake_C'"));
	m_ClassShake = FoundCamshake.Class;
	static ConstructorHelpers::FObjectFinder<UDataTable> FoundDgTable(
		TEXT("DataTable'/Game/DataTables/Dungeon/MagicDragonDgTable.MagicDragonDgTable'"));

	m_MagicDgTable = FoundDgTable.Object;
}

void UMagicStoneDgManager::Init()
{
	m_MagicDgTable->GetAllRows(TEXT("MagicDgManager-No Table Data"), m_DgDataRow);
}

void UMagicStoneDgManager::RequestMoveMagicStoneDg(int dgLevel)
{
	m_nDgLevel = dgLevel;

	m_CurrentDgData = m_DgDataRow[m_nDgLevel];

	UGameplayStatics::OpenLevel(UDiabloGameInstance::Get->GetWorld(),TEXT("MagicDragonNest"), true);

	m_LevelLoadHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(
		this, &UMagicStoneDgManager::OnLevelLoaded);
}

void UMagicStoneDgManager::OnLevelLoaded(UWorld* world)
{
	PRINTF("MagicStoneManager! World:%s", *world->GetMapName());

	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(m_LevelLoadHandle);

	UGameplayStatics::GetPlayerController(UDiabloGameInstance::Get->GetWorld(), 0)->ClientPlayCameraShake(m_ClassShake);

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

	m_nSuccessBounty = FMath::RandRange(m_CurrentDgData->m_nPrizeMagicStoneMin,m_CurrentDgData->m_nPrizeMagicStoneMax);

	m_nFailBounty  = m_CurrentDgData->m_nPrizeMagicStoneMin / 4;

	StartMagicDg();
}


void UMagicStoneDgManager::Tick(float delta_seconds)
{
	//서버에서 받아온거
	if (!m_bIsMatchStarted)
	{
		return;
	}

	m_fTimer += delta_seconds;

	float TimeRemain = MAGICDGTIME - m_fTimer;

	m_OnTick.ExecuteIfBound(TimeRemain);

	if (m_fTimer > MAGICDGTIME)
	{
		EndMagicDgFail();
	}
}


void UMagicStoneDgManager::StartMagicDg()
{
	APlayerDiabloCharacter* PlChar = UDiabloGameInstance::Get->GetPlChar();

	PlChar->FocusTarget(m_SpawnedMagicDragon);

	m_fTimer = 0.f;
	
	m_bIsMatchStarted = true;
}

void UMagicStoneDgManager::EndMagicDgSuccess(AMonsterPawn* dragonDead)
{
	PRINTF("사냥성공");
	
	AddMagicStones(m_nSuccessBounty);
	
	m_bIsMatchStarted = false;

	//UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this,&UMagicStoneDgManager::MoveStageLevelToNormalDungeon, 2.2f,false);

	m_OnBattleEnd.ExecuteIfBound(true);	
}

void UMagicStoneDgManager::EndMagicDgFail()
{
	PRINTF("사냥실패");

	AddMagicStones(m_nFailBounty);

	m_bIsMatchStarted = false;
	
	//UDiabloGameInstance::Get->GetWorld()->GetTimerManager().SetTimer(m_TimerHandle_OnTimer, this,&UMagicStoneDgManager::MoveStageLevelToNormalDungeon, 2.2f,false);

	m_OnBattleEnd.ExecuteIfBound(false);
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
